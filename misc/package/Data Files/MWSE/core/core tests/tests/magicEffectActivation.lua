local inspect = require("inspect")
local log = mwse.Logger.new({ name = "coreTest:magicEffectActivation", level  = mwse.logLevel.info })

-- 
-- Testing for activation/deactivation.
-- 

--- A table of all active effects for all active references.
--- @type table<tes3reference, table<tes3.effect, number>>
local activeEffects = {}

--- Track effects being activated.
--- @param e magicEffectActivatedEventData
local function onMagicEffectActivated(e)
	log:info("Magic effect %s activated on %s", e.effect, e.target)
	local refEffects = table.getset(activeEffects, e.target, {})
	refEffects[e.effect.id] = (refEffects[e.effect.id] or 0) + 1
end
event.register(tes3.event.magicEffectActivated, onMagicEffectActivated)

--- Track effects being deactivated.
--- @param e magicEffectDeactivatedEventData
local function onMagicEffectDeactivated(e)
	log:info("Magic effect %s deactivated on %s", e.effect, e.target)

	local refEffects = activeEffects[e.target]
	if (not refEffects) then
		log:error("Attempting to deactivate effect '%s' from reference '%s': No effects are registered to this reference.", e.effect, e.target)
		os.exit()
	end

	local currentCount = refEffects[e.effect.id]
	if (not currentCount) then
		log:error("Attempting to deactivate effect '%s' from reference '%s': The effect ID isn't registered to the reference.", e.effect, e.target)
		os.exit()
	end

	local newCount = currentCount - 1
	if (newCount < 0) then
		log:error("Attempting to deactivate effect '%s' from reference '%s': The effect count became negative.", e.effect, e.target)
		os.exit()
	elseif (newCount == 0) then
		refEffects[e.effect.id] = nil
		if (table.empty(refEffects)) then
			activeEffects[e.target] = nil
		end
	else
		refEffects[e.effect.id] = newCount
	end
end
event.register(tes3.event.magicEffectDeactivated, onMagicEffectDeactivated)

--- Perform the actual tests. Every frame make sure our monitored event data to the actual active magic effects list.
local function testActiveEffects()
	-- Gather list of all mobiles. Note that the player is not in the main list.
	local allMobiles = table.values(tes3.worldController.mobManager.processManager.allMobileActors)
	assert(table.find(allMobiles, tes3.mobilePlayer) == nil)
	table.insert(allMobiles, tes3.mobilePlayer)

	local calculatedEffects = {}
	for _, mobile in ipairs(allMobiles) do
		if (mobile.reference) then
			local mobileEffects = {}
			for _, effect in ipairs(mobile.activeMagicEffectList) do
				mobileEffects[effect.effectId] = (mobileEffects[effect.effectId] or 0) + 1
			end
			if (not table.empty(mobileEffects)) then
				assert(calculatedEffects[mobile.reference] == nil)
				calculatedEffects[mobile.reference] = mobileEffects
			end
		end
	end

	if (not table.equal(activeEffects, calculatedEffects)) then
		log:error("Active effect activated/deactivated test failed:\n\nTracked: %s\n\nCalculated: %s", inspect(activeEffects), inspect(calculatedEffects))
		os.exit()
	end
end
event.register(tes3.event.simulate, testActiveEffects)

--
-- Testing for adding/removing.
--

--- To test persistence through saved games, we store the states in a player-stored lua table.
--- @return table<string, table<string, number>>
local function getPersistentTestStorage()
	assert(tes3.player and tes3.player.data)
	return table.getset(tes3.player.data, "testMagicEffectStateData", {})
end

--- Track magic effects being added.
--- @param e magicEffectAddedEventData
local function onMagicEffectAdded(e)
	log:info("Magic effect %s added on %s", e.effect, e.target)

	local storage = getPersistentTestStorage()
	local refEffects = table.getset(storage, e.target.id, {})
	refEffects[tostring(e.effect.id)] = (refEffects[tostring(e.effect.id)] or 0) + 1
end
event.register(tes3.event.magicEffectAdded, onMagicEffectAdded)

--- Track magic effects being removed.
--- @param e magicEffectRemovedEventData
local function onMagicEffectRemoved(e)
	log:info("Magic effect %s removed on %s", e.effect, e.target)

	local storage = getPersistentTestStorage()

	local refEffects = storage[e.target.id]
	if (not refEffects) then
		log:error("Attempting to remove effect '%s' from reference '%s': No effects are registered to this reference.", e.effect, e.target)
		os.exit()
	end

	local currentCount = refEffects[tostring(e.effect.id)]
	if (not currentCount) then
		log:error("Attempting to remove effect '%s' from reference '%s': The effect ID isn't registered to the reference.", e.effect, e.target)
		os.exit()
	end

	local newCount = currentCount - 1
	if (newCount < 0) then
		log:error("Attempting to remove effect '%s' from reference '%s': The effect count became negative.", e.effect, e.target)
		os.exit()
	elseif (newCount == 0) then
		refEffects[tostring(e.effect.id)] = nil
		if (table.empty(refEffects)) then
			storage[e.target.id] = nil
		end
	else
		refEffects[tostring(e.effect.id)] = newCount
	end
end
event.register(tes3.event.magicEffectRemoved, onMagicEffectRemoved)

local function testEffectState()
	-- Gather list of all magic effect instances.
	local calculatedEffects = {}
	local allMagicSourceInstances = tes3.getAllMagicSourceInstances()
	for _, sourceInstance in ipairs(allMagicSourceInstances) do
		for effectIndex = 1, 8 do
			local effect = sourceInstance.sourceEffects[effectIndex]
			if (effect and effect.id ~= -1) then
				local effectInstances = sourceInstance:getEffectInstances(effectIndex - 1) --[[@as tes3magicEffectInstance[] ]]
				for _, effectInstance in ipairs(effectInstances) do
					local byRef = table.getset(calculatedEffects, effectInstance.target.id, {})
					local count = table.getset(byRef, tostring(effect.id), 0)
					byRef[tostring(effect.id)] = count + 1
				end
			end
		end
	end

	local trackedEffects = getPersistentTestStorage()
	if (not table.equal(trackedEffects, calculatedEffects)) then
		log:error("Active effect added/removed test failed:\n\nTracked: %s\n\nCalculated: %s", inspect(trackedEffects), inspect(calculatedEffects))
		os.exit()
	end
end
event.register(tes3.event.simulate, testEffectState)
