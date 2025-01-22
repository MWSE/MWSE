# Introduction

This module implements conversion functions between HSV and sRGB color spaces as described in the following blog posts:

- https://bottosson.github.io/posts/colorpicker/
- https://bottosson.github.io/posts/gamutclipping/
- https://bottosson.github.io/posts/oklab/

## Usage

There is `init.lua` that loads compiled `dll` and defines some wrapper Lua functions for easier usage.

```lua
local colorUtils = require("mwse.ui.tes3uiElement.createColorPicker.colorUtils")
```

# Building

To build this module you will need a C++ compiler. I used MSYS2 distribution of GCC.

```bat
mkdir build
C:\\msys64\\mingw32.exe bash
```

To compile, run the following two commands in bash:

```sh
gcc -c src/ColorUtils.cpp -o build/ColorUtils.o -O2
gcc -shared -o build/ColorUtils.dll build/ColorUtils.o

# To copy the built dll to the right place:
cp build/ColorUtils.dll .
```
