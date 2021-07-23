local re = require("re")

local function onLoaded(e)
    
	-- find the position of the first numeral in a string
	mwse.log("%d %d", re.find("the number 423 is odd", "[0-9]+"))  --> 12 14
	
end

event.register("loaded", onLoaded)
