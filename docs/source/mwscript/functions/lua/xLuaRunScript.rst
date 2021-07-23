
xLuaRunScript
========================================================

**Parameters:**

- ``string`` **script**: The name of the script to run. This is relative to *Data Files\\MWSE\\mods*.
- ``varies`` (many) **parameters**: Values that the lua script can access. 

**Returned:**

- ``varies`` (many) **results**: Values that the lua script passes back.

This function allows calling code from an external lua file. This call is blocking. Unlike other MWSE functions, this one requires the author to manage the parameter stack via Lua. This makes for slightly more complicated scripting, but allows for this function to create shared logic. Custom functions can be written out to Lua files and invoked with this method.

Example
--------------------------------------------------------

In this example we will create some values, log them, send them to Lua, confirm that we got the values, send more values back, and confirm our results. This demonstrates how to send information back and forth between the ``xLuaRunScript`` call and Lua.

First, our mwscript:

::

  Begin luaexec
  
  short myShort
  long myLong
  float myFloat
  long myString
  
  short retShort
  long retLong
  float retFloat
  long retString
  
  set myShort to 1
  setx myLong to xRandomLong 0 2147483647
  setx myFloat to xRandomFloat 0 1
  setx myString to xStringBuild "Hello World!"
  
  xLogMessage "[luaexec|mwscript] Sending values: %d, %d, %f, '%s'" myShort myLong myFloat myString
  
  setx retShort retLong retFloat retString to xLuaRunScript "test_mod\\luaexec" myShort myLong myFloat myString
  
  xLogMessage "[luaexec|mwscript] Got results: %d, %d, %f, '%s'" retShort, retLong, retFloat, retString
  
  StopScript luaexec
  
  End

Next, we have our Lua file (*Data Files\\mods\\test_mod\\luaexec.lua*) that gets the data, prints it, and sends more back.

.. code-block:: lua

  local myShort = mwse.stack.popShort()
  local myLong = mwse.stack.popLong()
  local myFloat = mwse.stack.popFloat()
  local myString = mwse.stack.popString()
  print(string.format("[luaexec|lua] Got values: %d, %d, %f, '%s'", myShort, myLong, myFloat, myString))
  
  mwse.stack.pushString("poop")
  mwse.stack.pushFloat(3.14)
  mwse.stack.pushLong(512)
  mwse.stack.pushShort(4)
  print("[luaexec|lua] Sending back values: 4, 512, 3.14, 'poop'")
