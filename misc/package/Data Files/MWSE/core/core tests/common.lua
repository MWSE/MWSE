local common = {}

common.log = require("logging.logger").new({
	name = "MWSE Core Tests",
	logLevel = "TRACE",
	logToConsole = false,
})

--- Gets the parameters after a "--"-prefixed command line argument.
--- @param startToken string
--- @return string[]|nil
function common.getCommandLineParams(startToken)
	local commandLine = os.getCommandLine()
	local index = table.find(commandLine, startToken)
	if (index == nil) then
		return nil
	end

	local results = {}
	for i = index + 1, #commandLine do
		local arg = commandLine[i]
		if (not arg) then
			break
		end

		if arg:startswith("--") then
			break
		end

		table.insert(results, arg)
	end

	return results
end

return common
