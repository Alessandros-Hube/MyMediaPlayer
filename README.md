# My Media Player

## Overview

This project is a media player application developed in 2018 using **Qt Creator** with **Qt version 5.3**. The application allows users to control media playback, including features for volume adjustment, play/pause functionality, and media selection through both GUI and external button inputs via a serial connection.

## Features

- **Media Playback Controls**: Play, pause, stop, and navigate through a playlist.
- **Volume Control**: Increase or decrease the volume using buttons or GUI options.
- **Serial Port Integration**: Interface with an Arduino device to receive button inputs for controlling the media player.
- **Current Position and Duration Display**: Display the current playback position and the total duration of the media.

## Architecture

The project is structured into several main components:

- **Mediacontroll**: This class handles media playback using `QMediaPlayer` and manages the playlist with `QMediaPlaylist`. It provides methods for playback control, volume adjustments, and emits signals to update the UI.

- **SerallPort**: This class manages the serial communication with the Arduino device. It reads button inputs and communicates the corresponding actions to the media controller.

### Protocol Communication with Serial Port

The communication protocol used with the Arduino consists of the following format:

[Version]|[Button Value]


**Version**
- 0x01 -> corresponds to the `sketch_MediaPlayer`

**Button Values for Version 0x01**
- Values between 225 and 227: No button pressed.
- Values between 220 and 224: Left button pressed.
- Values between 210 and 220: Middle button pressed.
- Values between 190 and 195: Right button pressed.

## Requirements

- **Qt Creator**: Version 5.3 or higher.
- **Qt Framework**: Ensure the Qt libraries are properly installed and configured.

## Installation

1. Clone or download the repository.
2. Open the project in Qt Creator.
3. Build the project to compile the application.
4. Connect an Arduino device if you wish to use the button control feature.
5. Run the application.

## Usage

1. **Loading Media**: Use the GUI to open and load media files into the playlist.
2. **Controlling Playback**: Utilize the play/pause, stop, and next/previous buttons to manage media playback.
3. **Adjusting Volume**: Change the volume using the designated buttons or through the GUI.
4. **Using Serial Input**: Ensure the Arduino is connected and configured to send button inputs for controlling playback.

## License

This project is licensed under the terms of the MIT License. See the LICENSE file for details.
