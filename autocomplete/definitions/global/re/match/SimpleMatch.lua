local re = require("re")

local function onLoaded(e)
    
	-- returns all words in a string
	mwse.log("%s %s %s %s", re.match("the number 423 is odd", "({%a+} / .)*"))
	--> the    number    is    odd

	-- returns the first numeral in a string
	mwse.log(re.match("the number 423 is odd", "s <- {%d+} / . s"))
	--> 423
	
end

event.register("loaded", onLoaded)
