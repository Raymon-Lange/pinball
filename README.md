# Pinball

A pinball arcade game built with C++ and Allegro 5.

**Version 0.7** — Raymon Lange

---

## The Story

I wrote this in 2010. I'm genuinely proud of it — a full pinball physics engine, collision maps, flippers, bumpers, targets, the works, all hand-rolled in C++. Not bad for a project that lived and died on a server that went dark in 2011.

That's right — no git. No version control of any kind. Just a folder on a server that I apparently had the good sense to tar up before it disappeared. I found it years later, blew the dust off, and spent some time porting it from Allegro 4 to Allegro 5 so it could actually build on a modern system. It runs. I'm happy about that.

If past-me had used git, this README would have a proper commit history and a neat graph of branches. Instead, you get `versions/version1/` through `versions/version7/` as tarballs. Honestly, respect.

---

## Controls

| Key | Action |
|-----|--------|
| `Space` / `Up Arrow` | Hold to charge plunger, release to shoot |
| `A` / `Left Arrow` | Left flipper |
| `D` / `Right Arrow` | Right flipper |
| `Escape` | Quit |

---

## Download & Install

Pre-built packages are available at **fire-hive.com**.

### Linux

Download `Pinball-0.7.0-Linux.deb` and install:

```bash
sudo dpkg -i Pinball-0.7.0-Linux.deb
sudo apt-get install -f   # installs any missing dependencies
```

Then launch from your application menu or run `Pinball` in a terminal.

Alternatively, download the `.AppImage`, make it executable, and run it directly — no install needed:

```bash
chmod +x Pinball-0.7.0.AppImage
./Pinball-0.7.0.AppImage
```

### Windows

Download `Pinball-0.7.0-win64.exe` and run it. The installer wizard will guide you through setup and create a Start Menu shortcut.

To uninstall, use Add/Remove Programs in Windows Settings.

### macOS

Download `Pinball-0.7.0.dmg`, open it, and drag the Pinball app into your Applications folder.

> **Note:** The app is not signed with an Apple Developer certificate. On first launch, right-click the app and choose **Open** instead of double-clicking, then confirm the Gatekeeper prompt.

---

## Building from Source

### Prerequisites

| Platform | Command |
|----------|---------|
| Linux (Debian/Ubuntu) | `sudo apt install liballegro5-dev cmake build-essential` |
| macOS | `brew install allegro cmake` |
| Windows | Install CMake; install Allegro 5 via [vcpkg](https://vcpkg.io): `vcpkg install allegro5:x64-windows` |

### Build

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/Pinball
```

### Package

**Linux `.deb`:**
```bash
cd build && cpack -G DEB
```

**Windows installer** (run on Windows with [NSIS](https://nsis.sourceforge.io) installed):
```bash
cd build && cpack -G NSIS
```

**macOS `.dmg`** (run on macOS):
```bash
cd build && cpack -G DragNDrop
```

---

## Version History

See `version.txt` for change notes.
