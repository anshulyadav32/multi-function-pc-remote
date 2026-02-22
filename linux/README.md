# PC Remote - Linux Client

Native Linux client for PC Remote using GTK4.

## Dependencies

- GTK4 >= 4.10
- libsoup-3.0 >= 3.0
- json-glib-1.0 >= 1.6
- Meson build system
- Ninja

## Building

### On Ubuntu/Debian:
```bash
sudo apt install meson ninja-build libgtk-4-dev libsoup-3.0-dev libjson-glib-dev
```

### On Fedora:
```bash
sudo dnf install meson ninja-build gtk4-devel libsoup3-devel json-glib-devel
```

### On Arch:
```bash
sudo pacman -S meson ninja gtk4 libsoup3 json-glib
```

### Build and Install:
```bash
cd linux
meson setup builddir
meson compile -C builddir
sudo meson install -C builddir
```

## Running

After installation:
```bash
pcremote
```

Or from the application menu: "PC Remote"

## Features

- 🎵 Media playback control
- 🖱️ Mouse and keyboard input
- 📁 File transfer
- ⚡ System power commands
- 🖥️ Screen viewing

## Integration

The application integrates with:
- GNOME Shell
- KDE Plasma
- Other freedesktop.org compliant desktops

## License

GPL-3.0-or-later
