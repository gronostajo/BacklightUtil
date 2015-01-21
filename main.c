#include <stdbool.h>
#include <windows.h>

typedef int (*htcFunc)(int *);

htcFunc GetBacklightMaxBrightness;
htcFunc GetBacklightMinBrightness;
htcFunc GetOnBatteryBrightnessLevel;
htcFunc GetOnPowerBrightnessLevel;
htcFunc SetOnBatteryBrightnessLevel;
htcFunc SetOnPowerBrightnessLevel;

int __cdecl main(int argc, char *argv[])
{
	HMODULE htcUtil = LoadLibrary(L"HTCUtil.dll");
	if (!htcUtil)
	{
		MessageBox(0, L"Loading HTCUtil failed", L"BacklightUtil", MB_OK | MB_ICONERROR | MB_TOPMOST);
		return 1;
	}

	GetBacklightMaxBrightness = (htcFunc)GetProcAddress(htcUtil, L"HTCUtilGetBacklightMaxBrightness");
	GetBacklightMinBrightness = (htcFunc)GetProcAddress(htcUtil, L"HTCUtilGetBacklightMinBrightness");
	GetOnBatteryBrightnessLevel = (htcFunc)GetProcAddress(htcUtil, L"HTCUtilGetOnBatteryBrightnessLevel");
	SetOnBatteryBrightnessLevel = (htcFunc)GetProcAddress(htcUtil, L"HTCUtilSetOnBatteryBrightnessLevel");
	GetOnPowerBrightnessLevel = (htcFunc)GetProcAddress(htcUtil, L"HTCUtilGetOnPowerBrightnessLevel");
	SetOnPowerBrightnessLevel = (htcFunc)GetProcAddress(htcUtil, L"HTCUtilSetOnPowerBrightnessLevel");

	if (!GetBacklightMaxBrightness ||
		!GetBacklightMinBrightness ||
		!GetOnBatteryBrightnessLevel ||
		!GetOnPowerBrightnessLevel ||
		!SetOnBatteryBrightnessLevel ||
		!SetOnPowerBrightnessLevel)
	{
		MessageBox(0, L"Loading HTCUtil functions failed", L"BacklightUtil", MB_OK | MB_ICONERROR | MB_TOPMOST);
		return 1;
	}

	int result;

	SYSTEM_POWER_STATUS_EX2 pwrStatus;
	result = GetSystemPowerStatusEx2(&pwrStatus, sizeof(pwrStatus), TRUE);
	if (result < sizeof(pwrStatus))
	{
		MessageBox(0, L"Checking power status failed", L"BacklightUtil", MB_OK | MB_ICONERROR | MB_TOPMOST);
		FreeLibrary(htcUtil);
		return 1;
	}

	bool charging = pwrStatus.ACLineStatus == AC_LINE_ONLINE;
	htcFunc getter, setter;
	if (charging)
	{
		getter = GetOnPowerBrightnessLevel;
		setter = SetOnPowerBrightnessLevel;
	}
	else
	{
		getter = GetOnBatteryBrightnessLevel;
		setter = SetOnBatteryBrightnessLevel;
	}

	int min, max, range, current;
	GetBacklightMinBrightness(&min);
	GetBacklightMaxBrightness(&max);
	range = max - min + 1;
	getter(&current);

	if (argc == 1)
	{
		char format[] = "Min: %d\r\nMax: %d\r\nCurrent: %d";

		char *msg;
		msg = malloc(strlen(format) + 4);
		sprintf(msg, format, min, max, current);

		int len = strlen(msg);
		int wlen = len * sizeof(msg) + 2;

		wchar_t *wmsg;
		wmsg = malloc(wlen);

		MultiByteToWideChar(CP_UTF8, 0, msg, len, wmsg, wlen);

		MessageBox(0, wmsg, L"BacklightUtil", MB_OK);
		free(msg);
		free(wmsg);
	}
	else
	{
		char *arg = argv[1];
		char sign = arg[0];

		int newval;
		
		if (sign == '-' || sign == '+')
		{
			int delta = atoi(arg + 1);
			if (sign == '-')
			{
				delta = max - delta;
			}
			newval = current + delta;
		}
		else
		{
			newval = atoi(arg);
		}

		newval = (newval - min) % range + min;
		setter(&newval);
	}

	FreeLibrary(htcUtil);

    return 0;
}
