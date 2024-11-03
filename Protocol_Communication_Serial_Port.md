### Protocol Communication Serial Port

[Version]|[Button Value]

**Version**

- 0x01 -> for the sketch_MediaPlayer

**Button Values for Version 0x01**

The sketch_MediaPlayer continuously sends resistance values.

- The values between 225 and 227 are sent by the Arduino when no button is pressed.
- The values between 220 and 224 are sent by the Arduino when the left button is pressed.
- The values between 210 and 220 are sent by the Arduino when the middle button is pressed.
- The values between 190 and 195 are sent by the Arduino when the right button is pressed.
