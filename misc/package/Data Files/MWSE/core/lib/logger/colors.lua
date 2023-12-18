--[[Logging colors.

Original file made by Merlord.

Updated by herbert100.
]]

-- local function isWindows()
-- 	return false
-- 	-- return type(package) == 'table' and type(package.config) == 'string' and package.config:sub(1,1) == '\\'
-- end

-- local supported = not isWindows() or os.getenv("ANSICON")


local keys = {
	reset = 0,
	bright = 1,
	dim = 2,
	underline = 4,
	blink = 5,
	reverse = 7,
	hidden = 8,
	-- foreground colors
	black = 30,
	red = 31,
	green = 32,
	yellow = 33,
	blue = 34,
	magenta = 35,
	cyan = 36,
	white = 37,
	-- background colors
	blackbg = 40,
	redbg = 41,
	greenbg = 42,
	yellowbg = 43,
	bluebg = 44,
	magentabg = 45,
	cyanbg = 46,
	whitebg = 47,
}

local pattern = "%%{([a-z%s]*)}"
-- local escape_string = string.char(27) .. '[%dm'
local escapeString = string.char(27) .. '[%dm'
local resetStr = escapeString:format(keys.reset)

-- this function is given a string of the form "`key1` `key2` ...", where each `key` is an index of the table `keys`
	-- it will then convert each `key` to the ansicolor delimiter used by the terminal
local function parseColors(str)
	local buffer = {}
	local number
	for word in str:gmatch("[a-z]+") do
		if word ~= "" then 
			number = keys[word]
			assert(number, "Unknown key: " .. word)
			table.insert(buffer, escapeString:format(number))
		end
	end
	return table.concat(buffer, " ")
end

-- public

local colors = {}

function colors.noReset(str)
	if str == nil then return "" end
	return str:gsub(pattern, parseColors)
end

setmetatable(colors, {
	__call = function(_, str)
		if str == nil then return "" end
		return table.concat({
			resetStr,
			str:gsub(pattern, parseColors), 
			resetStr
		})
	end
})
return colors