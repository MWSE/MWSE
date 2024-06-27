local inspect = require("inspect")
local UnitWind = require("unitwind")

local configFile = "mwse_test_mwseLoadConfig"
local configPath = string.format("Data Files\\MWSE\\config\\%s.json", configFile)

-- We use single config file for multiple tests. Clear it before new test.
local function deleteConfigFile()
	os.remove(configPath)
end

local testSuite = UnitWind.new({
	enabled = true,
	highlight = true,
	exitAfter = true,
	beforeAll = deleteConfigFile,
	afterEach = deleteConfigFile,
})

local function tablesEqual(t1, t2)
	-- Check table references and primitive types
	if t1 == t2 then return true end
	local type1 = type(t1)
	local type2 = type(t2)
	if type1 ~= type2 then return false end

	local t1keys = {}
	-- For tables, first check if all of the t1's keys exist and are equal in t2
	-- Then we check that t2 doesn't have any extra keys.
	for key1, value1 in pairs(t1) do
		local value2 = t2[key1]
		if value2 == nil or tablesEqual(value1, value2) == false then
			return false
		end
		t1keys[key1] = true
	end

	for key2, _ in pairs(t2) do
		if not t1keys[key2] then return false end
	end
	return true
end

testSuite:start("Testing mwse.loadConfig recovery of integer keys in stored config files.")

-- A simple config file
testSuite:test("Hash table config", function()
	local configFileContents = {
		setting1 = 12.5,
		setting2 = "apple",
		someFlag = true,
	}
	mwse.saveConfig(configFile, configFileContents)
	local defaults = {
		setting1 = 10.0,
		setting2 = "melon",
		someFlag = true,
	}
	local merged = {
		setting1 = 12.5,
		setting2 = "apple",
		someFlag = true,
	}
	local loaded = mwse.loadConfig(configFile, defaults)
	local failed = not tablesEqual(merged, loaded)

	if failed then
		mwse.log("Hash table config")
		mwse.log("configFileContents = %s", inspect(configFileContents))
		mwse.log("defaults = %s", inspect(defaults))
		mwse.log("loaded = %s", inspect(loaded))
		mwse.log("expected = %s", inspect(merged))
	end
	testSuite:expect(failed).toBe(false)
end)

-- A config file with some integer keys
testSuite:test("Convert integer keys from strings on load", function()
	local configFileContents = {
		setting1 = 12.5,
		setting2 = "apple",
		someFlag = true,
		types = {
			[tes3.crimeType.attack] = false,
			[tes3.crimeType.killing] = false,
			[tes3.crimeType.theft] = true,
		},
	}
	mwse.saveConfig(configFile, configFileContents)
	local defaults = {
		setting1 = 12.5,
		setting2 = "apple",
		someFlag = true,
		types = {
			[tes3.crimeType.attack] = true,
			[tes3.crimeType.killing] = true,
			[tes3.crimeType.theft] = true,
		},
	}
	local merged = {
		setting1 = 12.5,
		setting2 = "apple",
		someFlag = true,
		types = {
			[tes3.crimeType.attack] = false,
			[tes3.crimeType.killing] = false,
			[tes3.crimeType.theft] = true,
		},
	}
	local loaded = mwse.loadConfig(configFile, defaults)
	local failed = not tablesEqual(merged, loaded)

	if failed then
		mwse.log("Convert integer keys from strings on load")
		mwse.log("configFileContents = %s", inspect(configFileContents))
		mwse.log("defaults = %s", inspect(defaults))
		mwse.log("loaded = %s", inspect(loaded))
		mwse.log("expected = %s", inspect(merged))
	end
	testSuite:expect(failed).toBe(false)
end)

-- A more contrived test with some fields missing in the mod's config.json,
-- but present in defaults table.
testSuite:test("Nesting and missing keys in config file", function()
	local configFileContents = {
		key1 = "foo",
		[3] = {
			key2 = "bak",
			-- Missing "key3"
		}
	}
	mwse.saveConfig(configFile, configFileContents)
	local defaults = {
		key1 = "foo",
		[3] = {
			key2 = "bar",
			-- Added later, for example in a new version of the mod
			key3 = "baz",
		},
	}
	local merged = {
		key1 = "foo",
		[3] = {
			key2 = "bak",
			key3 = "baz",
		},
	}
	local loaded = mwse.loadConfig(configFile, defaults)
	local failed = not tablesEqual(merged, loaded)

	if failed then
		mwse.log("Nesting and missing keys in config file")
		mwse.log("configFileContents = %s", inspect(configFileContents))
		mwse.log("defaults = %s", inspect(defaults))
		mwse.log("loaded = %s", inspect(loaded))
		mwse.log("expected = %s", inspect(merged))
	end
	testSuite:expect(failed).toBe(false)
end)

testSuite:finish()
