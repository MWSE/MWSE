local re = require("re")

local function onLoaded(e)

	-- returns a string with all vowels replaced by .
	mwse.log(re.gsub("hello World", "[aeiou]", "."))
	--> h.ll. W.rld
	
end

event.register("loaded", onLoaded)
