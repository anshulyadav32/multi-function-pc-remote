# Multi-Function PC Remote

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

A comprehensive cross-platform remote control solution for PCs with multiple client options.

## 🌟 Features

- 🎵 **Media Control**: Play/pause, volume control, track navigation
- ⌨️ **Keyboard/Mouse Input**: Remote input control over network
- 📁 **File Transfer**: Seamless file sharing between devices
- 💻 **System Commands**: Shutdown, sleep, restart, lock
- 🖥️ **Screen Sharing**: View and control your PC remotely

## 📦 Components

### Desktop Server (Qt/C++)
Cross-platform WebSocket server that runs on Windows, Linux, and macOS.
- **Tech**: Qt6, WebSockets, C++17
- **Platforms**: Windows, Linux, macOS

### Android Client
Native Android app with Material Design 3.
- **Tech**: Kotlin, Jetpack Compose
- **Target**: Android 7.0+ (API 24+)

### iOS Client
Native iOS app with SwiftUI.
- **Tech**: Swift, SwiftUI
- **Target**: iOS 15.0+

### Linux Client (GTK4)
Native Linux client with GNOME integration.
- **Tech**: C, GTK4, libsoup-3.0
- **Target**: Linux with GTK4

### Web Client
Browser-based remote control.
- **Tech**: Vue.js 3, Vite
- **Target**: Modern browsers

## 🚀 Building

### Desktop Server
```bash
cd desktop
mkdir build && cd build
cmake ..
cmake --build .
./pc-remote-server  # or run from build directory
```

### Android Client
```bash
cd android
./gradlew assembleDebug
# APK output: android/app/build/outputs/apk/debug/
```

### iOS Client
```bash
open ios/PCRemote.xcodeproj
# Build in Xcode (Cmd+R)
```

### Linux Client
```bash
cd linux
meson setup builddir
meson compile -C builddir
sudo meson install -C builddir
pcremote  # Run after install
```

### Web Client
```bash
cd web
npm install
npm run dev    # Development server
npm run build  # Production build
```

## 🔧 Configuration

The desktop server runs on **port 8765** by default. Update client connection addresses to match your PC's IP:
- Example: `192.168.1.100:8765`
- The server listens on all network interfaces

## 🏗️ Architecture

```
┌─────────────┐     WebSocket      ┌──────────────┐
│   Clients   │◄──────────────────►│ Desktop      │
│             │     (Port 8765)    │ Server       │
│ - Android   │                    │              │
│ - iOS       │     JSON Commands  │ - Qt/C++     │
│ - Linux     │◄──────────────────►│ - WebSocket  │
│ - Web       │                    │ - Controllers│
└─────────────┘                    └──────────────┘
```

The system uses WebSocket for real-time communication. All commands are sent as JSON messages.

### Communication Protocol

JSON-based messages:
```json
{
  "type": "media|input|file|system|screen",
  "action": "play_pause|next|lock|...",
  "id": 1234567890,
  "data": { /* optional additional data */ }
}
```

## 📝 License

GPL-3.0-or-later - See [LICENSE](LICENSE) file for details.

## 🤝 Contributing

Contributions are welcome! Please read [CONTRIBUTING.md](CONTRIBUTING.md) for development guidelines.

## 🐛 Issues

Report bugs and request features via [GitHub Issues](https://github.com/anshulyadav32/multi-function-pc-remote/issues).
