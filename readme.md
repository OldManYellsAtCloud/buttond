Simple button event listener

### Config
The applicaton expects a file called `/etc/$BINARY_NAME.cfg`, where `$BINARY_NAME` is the same as... well... the name of the binary file that is executed.

The content's format is `.ini`, and looks like this:

```
; events section is mandatory
[events]
; power is a path to the power button's virtual file
power=/dev/input/by-path/platform-gpio-keys-event
; volume is the path to the volume buttons' virtual file
volume=/dev/input/by-path/platform-adc-keys-event

; hw section is mandatory
[hw]
; ts_inhibit is a path to a sysfs file that can be used to block
; unwanted touches on the touchscreen.
ts_inhibit=/sys/devices/platform/ff130000.i2c/i2c-3/3-0014/input/input1/inhibited
```
