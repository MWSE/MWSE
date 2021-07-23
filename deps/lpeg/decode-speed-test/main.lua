--[[
MWSE uses dkjson to encode and decode json. dkjson can optionally use lpeg to spead up decoding.
This mod runs json.decode() for a specified number of iterations (100,000 by default) whenever a 
key (x by default) is pressed, and records the runtime to the log. It also records whether the 
lpeg module is loaded.

To test the speed of dkjson with and without lpeg in mwse, do the following:
Put this mod in "Data Files\MWSE\mods".

With lpeg:
Ensure that lpeg.dll is present in "Data Files\MWSE\core".
Load a game.
Press the x key. The game will pause for a few seconds while the test runs.
Exit the game.
In MWSE.log you should find:
dkjson with lpeg
decoding 100000 iterations: x.xxxs
Record this value for later comparison.

Without lpeg:
Rename or remove lpeg.dll from "Data Files\MWSE\core".
Load a game.
Press the x key. The game will pause for a few seconds while the test runs.
Exit the game.
In MWSE.log you should find:
dkjson without lpeg
decoding 100000 iterations: x.xxxs
Compare this with the value obtained previously.

On a test system, dkjson without lpeg was about 40% slower than with lpeg.
--]] 

-- adapted from dk speedtest code
-- example data taken from
-- http://de.wikipedia.org/wiki/JavaScript_Object_Notation

local str = [[
{
	"Herausgeber": "Xema",
	"Nummer": "1234-5678-9012-3456",
	"Deckung": 26,
	  "Währung": "EUR",
	  "Inhaber": {
		"Name": "Mustermann",
		"Vorname": "Max",
		"männlich": true,
		"Depot": {},
		"Hobbys": [ "Reiten", "Golfen", "Lesen" ],
		"Alter": 42,
		"Kinder": [0],
		"Partner": null
	  }
	}
]]

local function speedtest()

    if package.loaded["lpeg"] ~= nil
	then
		mwse.log("dkjson with lpeg")
	else
		mwse.log("dkjson without lpeg")
	end
	
	local t1, t2
	local iterations = 100000
    t1 = os.clock ()
    for i = 1, iterations do
      json.decode (str)
    end
    t2 = os.clock ()
    mwse.log("decoding %d iterations: %fs", iterations, t2 - t1)
    
end

local function onKeyDown(e)
    speedtest()
end

event.register("keyDown", onKeyDown, { filter = tes3.scanCode.x } )
