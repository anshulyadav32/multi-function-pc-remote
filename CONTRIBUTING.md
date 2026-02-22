# Contributing to Multi-Function PC Remote

Thank you for your interest in contributing!

## Development Environment

### Desktop Application (Qt/C++)
- **Requirements**: Qt 6.x, CMake 3.16+, C++17 compiler
- **Build**: See README.md for build instructions
- **IDE**: Qt Creator, Visual Studio, or any C++ IDE with CMake support

### Android Application
- **Requirements**: Android Studio, JDK 17+, Gradle
- **Build**: Open `android/` directory in Android Studio
- **Target**: Android 7.0+ (API 24+)

### iOS Application
- **Requirements**: Xcode 14+, macOS, Swift 5.7+
- **Build**: Open `ios/PCRemote.xcodeproj` in Xcode
- **Target**: iOS 15.0+

### Linux Client (GTK4)
- **Requirements**: GTK4, libsoup-3.0, json-glib-1.0, Meson, Ninja
- **Build**: See `linux/README.md`
- **Target**: Linux with GTK4 support

### Web Application
- **Requirements**: Node.js 18+, npm
- **Build**: `cd web && npm install && npm run dev`
- **Target**: Modern browsers

## Code Style

- **C/C++**: Follow the existing style, use 4-space indentation
- **Swift**: Follow Swift style guide, use 4-space indentation
- **Kotlin**: Follow Kotlin coding conventions
- **JavaScript/Vue**: Use 2-space indentation, follow Vue.js style guide

## Submitting Changes

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test on the target platform
5. Submit a pull request

## License

All contributions must be licensed under GPL-3.0-or-later.
