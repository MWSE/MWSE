local lpeg = require("lpeg")

local function onLoaded(e)
    
	-- matches a word followed by end-of-string
	p = lpeg.R("az")^1 * -1

	mwse.log(p:match("hello"))        --> 6
	mwse.log(lpeg.match(p, "hello"))  --> 6
	mwse.log(p:match("1 hello"))      --> nil
	
end

event.register("loaded", onLoaded)
