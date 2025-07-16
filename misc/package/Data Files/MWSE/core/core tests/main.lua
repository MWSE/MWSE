local common = require("core tests.common")

local tests = common.getCommandLineParams("--core_tests")
if (tests == nil) then
	return
end

-- If we have no tests defined, have a default list.
if (table.empty(tests)) then
	tests = {
		"dialogueReplacement",
		"events",
		"logging",
		"mwseLoadConfig",
	}
end

common.log:info("Tests to run: %s", table.concat(tests, ", "))
for _, test in ipairs(tests) do
	local testFile = string.format("core tests.tests.%s", test)
	common.log:trace("Initializing test: %s", test)
	pcall(dofile, testFile)
	common.log:trace("Test initialized: %s", test)
end
