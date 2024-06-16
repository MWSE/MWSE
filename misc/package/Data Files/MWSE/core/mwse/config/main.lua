event.register("modConfigReady", function()
	local i18n = mwse.loadTranslations("mwse.config")

	local template = mwse.mcm.createTemplate{ 
		name = "Morrowind Script Extender",
		onClose = function(_)
			local values = {}
			for k, _ in pairs(mwseConfig.getDefaults()) do
				values[k] = mwseConfig[k]
			end
			mwse.saveConfig("MWSE", values)
		end
	}

	local page = template:createSideBarPage()

	local function createButton(configKey)
		page:createYesNoButton{
            label = i18n(configKey .. ".label"),             -- i.e., `i18n("logWarningsWithLuaStack.label")
			description = i18n(configKey .. ".description"), -- i.e., `i18n("logWarningsWithLuaStack.description")
			variable = mwse.mcm.createTableVariable{id = configKey, table = mwseConfig}
		}
	end

	createButton("logWarningsWithLuaStack")
	createButton("runInBackground")
	createButton("letterboxMovies")
	createButton("replaceDialogueFiltering")
	createButton("patchNiFlipController")
	createButton("keepAllNetImmerseObjectsAlive")
	createButton("enableLegacyLuaMods")
	createButton("enableLogColors")
	createButton("enableDependencyChecks")
	createButton("enableLuaErrorNotifications")
	createButton("useSkinnedAccurateActivationRaytests")

	page.sidebar:createInfo{
		label = i18n("notice.label"),
		text = i18n("notice.text"),
	}

	page.sidebar:createInfo{
		label = i18n("credits.label"),
		text = table.concat(
			{
				"Anthony Garcia",
				"Charles Cooley (cdcooley)",
				"Cody Erekson (Fliggerty)",
				"FreshFish",
				"Grant McDorman",
				"Greatness7",
				"Hrnchamd",
				"Merlord",
				"Merzasphor",
				"Michael Wallar (NullCascade)",
				"OperatorJack",
				"Pete Goodfellow (Petethegoat)",
				"Pierre Steeg",
				"Sebastien Levy (MetaBarj0)",
				"Tim Peters",
				"Timeslip",
			}, 
			"\n"
		)
	}
	
	template:register()
end)