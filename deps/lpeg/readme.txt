This is lpeg-1.0.2 from http://www.inf.puc-rio.br/~roberto/lpeg/

Build LuaJIT first:
Open a Developer Command Prompt for VS
cd deps\LuaJIT\src
msvcbuild.bat

To run the included tests:
Copy deps\lpeg\build\Release\lpeg.dll, deps\lpeg\test.lua, and deps\lpeg\re.lua to deps\LuaJIT\src
Open a command prompt.
cd deps\LuaJIT\src
luajit test.lua

To test the speed of json decoding within MWSE, use the decode-speed-test mod in this directory.
Instructions can be found in the comments at the top of main.lua.