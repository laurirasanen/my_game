# my_game

The source code release of my_game.

## Table of contents

- [Status](#Status)
- [License](#License)
- [Assets](#Assets)
- [Compiling](#Compiling)
- [Tools](#Tools)

## Status

In development.

![game screenshot](/docs/screenshots/game_2022-01-05.png)

<p align="center">Testing with Q3 assets.</p>

## License

See [LICENSE](LICENSE) for the BSD 3-Clause License.

Some source code in this release is not covered by the BSD 3-Clause License:

---

BSP loading and rendering based on work by Krzysztof Kondrak.
[https://github.com/kondrak/quake_bsp_vulkan](https://github.com/kondrak/quake_bsp_vulkan)

```
file(s):
  src/bsp/bsp_loader.c
  src/bsp/bsp_map.c
  src/bsp/bsp_patch.c
```

```
MIT License

Copyright (c) 2018 Krzysztof Kondrak

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

---

All projects contained in `third_party` are copyright of their respective owners.

[gunslinger](https://github.com/MrFrenik/gunslinger)

- File(s): `third_party/include/gs/*`
- License: [third_party/include/gs/LICENSE](third_party/include/gs/LICENSE)

## Assets

// TODO

## Compiling

### Windows

- Install MingW

```sh
./proc/win/mingw_dbg.sh
```

### Linux

Tested on Ubuntu 22.04.1 LTS (Desktop).

```sh
sudo apt install gcc mesa-common-dev libxcursor-dev libxrandr-dev libxinerama-dev libxi-dev libc6-dev-i386
./proc/linux/gcc_dbg.sh
```

## Console

Hitting the console key (default: F1) will allow you to change cvars and run commands. Type `help` for more information.

![console screenshot](/docs/screenshots/console_2022-04-18.png)

## Config

The game config is located in `assets/cfg/config.txt`. The game will use values from this file when launching. You can modify any value at runtime via the console. Note that this file does not support custom formatting and will be overwritten when the game exits.

## Tools

### ModelViewer

Tool for checking .md3 models and custom animation.cfg files.  
Type `help` in console for details.

```sh
cd bin
./modelviewer
```

![modelviewer screenshot](/docs/screenshots/modelviewer_2022-03-11.png)
