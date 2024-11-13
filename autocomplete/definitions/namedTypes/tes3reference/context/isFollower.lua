local followPackage = {
	[tes3.aiPackage.follow] = true,
	-- Depending on your needs, you can also include player's escortees.
	-- In the base game, AiEscort package is quite rare. Only White Guar
	-- has that package and with player as the targetActor.
	[tes3.aiPackage.escort] = true,
}

--- This function returns `true` if a given mobile has
--- follow ai package with player as its target
---@param mobile tes3mobileNPC|tes3mobileCreature
---@return boolean isFollower
local function isFollower(mobile)
	local planner = mobile.aiPlanner
	if not planner then
		return false
	end

	local package = planner:getActivePackage()
	if not package then
		return false
	end

	if followPackage[package.type] then
		local target = package.targetActor

		if target.objectType == tes3.objectType.mobilePlayer then
			return true
		end
	end
	return false
end

--- With the above function we can build a function that
--- creates a table with all of the player's followers
---@return tes3reference[] followerList
local function getFollowers()
	local followers = {}

	for _, mobile in pairs(tes3.mobilePlayer.friendlyActors) do
		---@cast mobile tes3mobileNPC|tes3mobileCreature
		if isFollower(mobile) then
			table.insert(followers, mobile.reference)
		end
	end

	return followers
end
