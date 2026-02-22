# Multi-Function PC Remote

A comprehensive remote control solution for PCs with desktop, Android, and web interfaces.

## Features

- 🎵 **Media Control**: Play/pause, volume control, track navigation
- ⌨️ **Keyboard/Mouse Input**: Remote input control over network
- 📁 **File Transfer**: Seamless file sharing between devices
- 💻 **System Commands**: Shutdown, sleep, restart, lock
- 🖥️ **Screen Sharing**: View and control your PC remotely

## Components

### Desktop Application (Qt/C++)
Cross-platform desktop server application that runs on Windows, Linux, and macOS.

### Android Application
Native Android app for controlling your PC from your phone or tablet.

### Web Application
Browser-based remote control accessible from any device.

## Building

### Desktop Application
```bash
cd desktop
mkdir build && cd build
cmake ..
cmake --build .
```

### Android Application
```bash
cd android
./gradlew assembleDebug
```

### Web Application
```bash
cd web
npm install
npm run build
```

## Architecture

The system uses WebSocket for real-time communication between the remote client and the PC server. All communication is encrypted using TLS.

## License

GPL-3.0-or-later

## Contributing

Contributions are welcome! Please read CONTRIBUTING.md for details.
