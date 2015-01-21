# BacklightUtil

A simple command-line utility for changing backlight level on HTC devices running Windows Mobile.

On regular Windows CE and Windows Mobile devices backlight level is changed by editing registry values in `HKCU\ControlPanel\BackLight`. However, modern(-ish) HTC devices use proprietary backlight API that causes registry changes to be effective only after restart.

This utility uses API functions from `HTCUtil.dll` to change backlight level without the need for a reboot.

Thanks for [people on the Internets](https://jianwuchen.wordpress.com/2009/12/03/htc-backlight-control/) for reverse engineering HTCUtil.

Developed using [Pelles C 7](http://www.smorgasbordet.com/pellesc/).

## Usage

- `BacklightUtil.exe` (or click the icon) - shows a dialog with current backlight level and range of values supported by device. (It's device-specific)
- `BacklightUtil.exe n` (`n` is a number) - sets backlight to level `n`. For example `BacklightUtil.exe 4` sets brightness level 4.
- `BacklightUtil.exe [-/+]n` (`n` is a number) - increases/decreases backlight by `n` levels. For example `BacklightUtil.exe +1` will increase backlight brightness by one step. Using `+1` on the brightest level will set the darkest one.

Please note that Windows Mobile uses separate backlight level when charging. This utility displays and sets level for the current state only.

## Icons

Two 32x32 icons are included in the output `.exe`: one with plus sign (8002) and one with minus (8003). They can be used as shortcut icons for launchers etc. Just edit the shortcut file, they are plain text files with following format:

    len_of_path#path

For example let's alter following shortcut:

    37#"\Program Files\BacklightUtil.exe" +1

To use the `+` icon append icon's path after `?` and update lenght of path:

    75#"\Program Files\BacklightUtil.exe" +1?\Program Files\BacklightUtil.exe,8002
     
Windows Mobile may cache icons, they are reloaded on reboot.