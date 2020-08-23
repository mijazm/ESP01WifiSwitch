# ESP01WiFiSwitch

This is an arduino sketch that allows you to control up to 4 devices using ESP01 and Alexa voice commands. It is based on fauxmo library, a copy of which is included here
so that any future changes in the library may not break this code. You can try using the original library from [here](https://github.com/simap/fauxmoesp).

## Requirements
- This sketch has been tested with Amazon Echo 3rd Gen, it may or may not work with older or newer ones.
- You will also need to install [ESPAsyncTCP](https://github.com/me-no-dev/AsyncTCP) library by [me-no-dev](https://github.com/me-no-dev)

## Troubleshooting
- Make sure that in your Arduino IDE, Tools > IwIP Variant > is set to 'v2 Higher Bandwidth' otherwise you will not be able to control more than 3 devices.

