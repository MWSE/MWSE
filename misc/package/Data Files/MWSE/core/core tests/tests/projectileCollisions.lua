--- @diagnostic disable: undefined-field
--- @diagnostic disable: param-type-mismatch
--- @diagnostic disable: undefined-global

local log = mwse.Logger.new({ name = "coreTest:projectileCollisions", level = mwse.logLevel.info })

local timeoutFrames = 3600

---@class projectileCollisionWatch
---@field name string
---@field event string
---@field count integer

local watchedEvents = {
	{ name = "projectileHitActor", event = tes3.event.projectileHitActor, count = 0 },
	{ name = "projectileHitObject", event = tes3.event.projectileHitObject, count = 0 },
	{ name = "projectileHitTerrain", event = tes3.event.projectileHitTerrain, count = 0 },
	{ name = "spellProjectileHitActor", event = tes3.event.spellProjectileHitActor, count = 0 },
	{ name = "spellProjectileHitObject", event = tes3.event.spellProjectileHitObject, count = 0 },
	{ name = "spellProjectileHitTerrain", event = tes3.event.spellProjectileHitTerrain, count = 0 },
	{ name = "spellProjectileHitWater", event = tes3.event.spellProjectileHitWater, count = 0 },
}

local finished = false
local frameCount = 0

local function allEventsUsed()
	for _, watched in ipairs(watchedEvents) do
		if (watched.count == 0) then
			return false
		end
	end

	return true
end

local function getMissingEvents()
	local missing = {}
	for _, watched in ipairs(watchedEvents) do
		if (watched.count == 0) then
			table.insert(missing, watched.name)
		end
	end

	return missing
end

local function describeEventData(e)
	local parts = {
		string.format("mobile=%s", e.mobile),
		string.format("target=%s", e.target),
		string.format("collisionPoint=%s", e.collisionPoint),
		string.format("position=%s", e.position),
		string.format("velocity=%s", e.velocity),
	}

	if (e.firingReference ~= nil) then
		table.insert(parts, string.format("firingReference=%s", e.firingReference))
	end

	if (e.firingWeapon ~= nil) then
		table.insert(parts, string.format("firingWeapon=%s", e.firingWeapon))
	end

	return table.concat(parts, ", ")
end

local function finish()
	if (finished) then
		return
	end

	finished = true
	event.unregister(tes3.event.simulate, onSimulate)

	local missing = getMissingEvents()
	if (#missing == 0) then
		log:info("Projectile collision test complete after %d frame(s). Every collision event fired at least once.", frameCount)
		return
	end

	log:error("Projectile collision test incomplete after %d frame(s). The following events were never used: %s", frameCount, table.concat(missing, ", "))
	for _, eventName in ipairs(missing) do
		log:error("Event never used: %s", eventName)
	end
end

local function onProjectileCollision(watched, e)
	if (finished) then
		return
	end

	watched.count = watched.count + 1
	log:info("[%s #%d] %s", watched.name, watched.count, describeEventData(e))

	if (allEventsUsed()) then
		finish()
	end
end

for _, watched in ipairs(watchedEvents) do
	event.register(watched.event, function(e)
		onProjectileCollision(watched, e)
	end)
end

local function onSimulate()
	if (finished) then
		return
	end

	frameCount = frameCount + 1
	if (frameCount == 1) then
		log:info("Watching projectile collision events for up to %d frame(s). Fire standard and spell projectiles to trigger each collision event.", timeoutFrames)
	end

	if (frameCount >= timeoutFrames) then
		finish()
	end
end
event.register(tes3.event.simulate, onSimulate)
