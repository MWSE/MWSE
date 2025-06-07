# APIs

## Lua

MWSE exposes it's APIs via Lua programming language. The available version of Lua is 5.1 with many extensions from Lua 5.2 and some from Lua 5.3. To see the extensions available from Lua 5.2 and 5.3 browse under "Extensions from Lua 5.2" and "Extensions from Lua 5.3" in LuaJIT's [extensions page](https://luajit.org/extensions.html).

## Additional MWSE libraries

MWSE implements some Lua modules useful for modding, that aren't available in the global scope. These have to be `require`d before use.

Library | Documentation | Notes
------- | ------------- | -----
`logging.logger` | [Logging guide](https://mwse.github.io/MWSE/guides/logging/) | Logging module.
`Unitwind` | Available at implementation site. | Unit testing module. It's implementation is found in `MWSE\\core\\lib\\unitwind`.

## Third-party libraries

In addition, MWSE ships with some third-party libraries useful for modding. Their APIs aren't covered here. The table below lists all third-party libraries available and links to their respective documentation.

Library | Documentation | Notes
------- | ------------- | ------
[BitOp](https://bitop.luajit.org/index.html) | [BitOp Documentation](https://bitop.luajit.org/api.html) | Module for bitwise operations. It's available in global scope (it doesn't have to be `require`d before use).
[LuaJIT's FFI](https://luajit.org/ext_ffi.html) | [FFI Documentation](https://luajit.org/ext_ffi_api.html) | Foreign functions interface module. Use as `local ffi = require("ffi")`.
[semver.lua](https://github.com/kikito/semver.lua) | [Documentation](https://github.com/kikito/semver.lua?tab=readme-ov-file#documentation) | Utility module for semantic versioning. Use as `local semver = require("semver")`.
[inspect.lua](https://github.com/kikito/inspect.lua) | [Documentation](https://github.com/kikito/inspect.lua?tab=readme-ov-file#examples-of-use) | Module for converting Lua tables to human-readable representation. Useful for debugging. Use as `local inspect = require("inspect")`.
[ansicolors.lua](https://github.com/kikito/ansicolors.lua) | [Documentation](https://github.com/kikito/ansicolors.lua) | Module to print to the console in color. Use as `local ansicolors = require("logging.colors")`.
[luasocket](https://github.com/lunarmodules/luasocket) | [Documentation](https://lunarmodules.github.io/luasocket/introduction.html) | Network support module. Use as `local socket = require("socket")`.
[luasec](https://github.com/lunarmodules/luasec) | [Documentation](https://github.com/lunarmodules/luasec/wiki) | Lua binding for OpenSSL library. Use as `local ssl = require("ssl")`.
