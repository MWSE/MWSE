local tes3 = {}

-- Game constants.
tes3.actionFlag = require("tes3.actionFlag")
tes3.activeBodyPart = require("tes3.activeBodyPart")
tes3.activeBodyPartLayer = require("tes3.activeBodyPartLayer")
tes3.actorType = require("tes3.actorType")
tes3.aiBehaviorState = require("tes3.aiBehaviorState")
tes3.aiPackage = require("tes3.aiPackage")
tes3.animationBodySection = require("tes3.animationBodySection")
tes3.animationGroup = require("tes3.animationGroup")
tes3.animationStartFlag = require("tes3.animationStartFlag")
tes3.animationState = require("tes3.animationState")
tes3.apparatusType = require("tes3.apparatusType")
tes3.armorSlot = require("tes3.armorSlot")
tes3.armorWeightClass = require("tes3.armorWeightClass")
tes3.attachmentType = require("tes3.attachmentType")
tes3.attribute = require("tes3.attribute")
tes3.attributeName = require("tes3.attributeName")
tes3.bodyPartAttachment = require("tes3.bodyPartAttachment")
tes3.bookType = require("tes3.bookType")
tes3.clothingSlot = require("tes3.clothingSlot")
tes3.codePatchFeature = require("tes3.codePatchFeature")
tes3.compilerSource = require("tes3.compilerSource")
tes3.contentType = require("tes3.contentType")
tes3.creatureType = require("tes3.creatureType")
tes3.crimeType = require("tes3.crimeType")
tes3.damageSource = require("tes3.damageSource")
tes3.dialogueFilterContext = require("tes3.dialogueFilterContext")
tes3.dialoguePage = require("tes3.dialoguePage")
tes3.dialogueType = require("tes3.dialogueType")
tes3.effect = require("tes3.effect")
tes3.effectAttribute = require("tes3.effectAttribute")
tes3.effectEventType = require("tes3.effectEventType")
tes3.effectRange = require("tes3.effectRange")
tes3.enchantmentType = require("tes3.enchantmentType")
tes3.event = require("tes3.event")
tes3.flowDirection = require("tes3.flowDirection")
tes3.gmst = require("tes3.gmst")
tes3.inventorySelectFilter = require("tes3.inventorySelectFilter")
tes3.inventoryTileType = require("tes3.inventoryTileType")
tes3.itemSoundState = require("tes3.itemSoundState")
tes3.justifyText = require("tes3.justifyText")
tes3.keybind = require("tes3.keybind")
tes3.keyboardCode = require("tes3.keyboardCode")
tes3.keyTransition = require("tes3.keyTransition")
tes3.language = require("tes3.language")
tes3.languageCode = require("tes3.languageCode")
tes3.magicSchool = require("tes3.magicSchool")
tes3.magicSourceType = require("tes3.magicSourceType")
tes3.merchantService = require("tes3.merchantService")
tes3.musicSituation = require("tes3.musicSituation")
tes3.niType = require("tes3.niType")
tes3.objectType = require("tes3.objectType")
tes3.palette = require("tes3.palette")
tes3.partIndex = require("tes3.partIndex")
tes3.physicalAttackType = require("tes3.physicalAttackType")
tes3.quickKeyType = require("tes3.quickKeyType")
tes3.scanCode = require("tes3.scanCode")
tes3.scanCodeToNumber = require("tes3.scanCodeToNumber")
tes3.skill = require("tes3.skill")
tes3.skillName = require("tes3.skillName")
tes3.skillRaiseSource = require("tes3.skillRaiseSource")
tes3.skillType = require("tes3.skillType")
tes3.soundGenType = require("tes3.soundGenType")
tes3.soundMix = require("tes3.soundMix")
tes3.specialization = require("tes3.specialization")
tes3.specializationName = require("tes3.specializationName")
tes3.spellSource = require("tes3.spellSource")
tes3.spellState = require("tes3.spellState")
tes3.spellType = require("tes3.spellType")
tes3.uiElementType = require("tes3.uiElementType")
tes3.uiEvent = require("tes3.uiEvent")
tes3.uiProperty = require("tes3.uiProperty")
tes3.uiState = require("tes3.uiState")
tes3.vfxContext = require("tes3.vfxContext")
tes3.voiceover = require("tes3.voiceover")
tes3.weaponType = require("tes3.weaponType")
tes3.weather = require("tes3.weather")

-- Second pass game constants, declared based on the above constants.
tes3.magicSchoolSkill = {
	[tes3.magicSchool.alteration] = tes3.skill.alteration,
	[tes3.magicSchool.conjuration] = tes3.skill.conjuration,
	[tes3.magicSchool.destruction] = tes3.skill.destruction,
	[tes3.magicSchool.illusion] = tes3.skill.illusion,
	[tes3.magicSchool.mysticism] = tes3.skill.mysticism,
	[tes3.magicSchool.restoration] = tes3.skill.restoration,
}

-------------------------------------------------
-- Functions
-------------------------------------------------

local attributeGMSTs = {
	[tes3.attribute.strength] = tes3.gmst.sAttributeStrength,
	[tes3.attribute.intelligence] = tes3.gmst.sAttributeIntelligence,
	[tes3.attribute.willpower] = tes3.gmst.sAttributeWillpower,
	[tes3.attribute.agility] = tes3.gmst.sAttributeAgility,
	[tes3.attribute.speed] = tes3.gmst.sAttributeSpeed,
	[tes3.attribute.endurance] = tes3.gmst.sAttributeEndurance,
	[tes3.attribute.personality] = tes3.gmst.sAttributePersonality,
	[tes3.attribute.luck] = tes3.gmst.sAttributeLuck,
}

-- Translate an attribute constant to a readable name.
function tes3.getAttributeName(attributeId)
	local GMSTId = attributeGMSTs[attributeId]
	if (GMSTId) then
		local GMST = tes3.findGMST(GMSTId)
		if (GMST) then
			return GMST.value
		end
	end

	-- Fallback to legacy code for early loaders.
	return tes3.attributeName[attributeId] or "invalid"
end

-- Translate an skill constant to a readable name.
function tes3.getSkillName(skillId)
	local skill = tes3.getSkill(skillId)
	if (skill) then
		return skill.name
	end

	-- Fallback to legacy code for early loaders.
	return tes3.skillName[skillId] or "invalid"
end

local specializationGMSTs = {
	[tes3.specialization.combat] = tes3.gmst.sSpecializationCombat,
	[tes3.specialization.magic] = tes3.gmst.sSpecializationMagic,
	[tes3.specialization.stealth] = tes3.gmst.sSpecializationStealth,
}

-- Translate an specialization constant to a readable name.
function tes3.getSpecializationName(specializationId)
	local GMSTId = specializationGMSTs[specializationId]
	if (GMSTId) then
		local GMST = tes3.findGMST(GMSTId)
		if (GMST) then
			return GMST.value
		end
	end

	-- Fallback to legacy code for early loaders.
	return tes3.specializationName[specializationId] or "invalid"
end

-- Return an attachment from a reference.
function tes3.getAttachment(reference, attachment)
	return reference and reference.attachments and reference.attachments[attachment]
end

-- Function to compare two keybind objects for equality. Useful in key events.
function tes3.isKeyEqual(params)
	local actual = params.actual
	local expected = params.expected

	-- Handle mouseWheelEventData
	local actualMouseWheel = actual.mouseWheel or actual.delta and math.clamp(actual.delta, -1, 1)
	local expectedMouseWheel = expected.mouseWheel or expected.delta and math.clamp(expected.delta, -1, 1)

	-- Handle mouseDownEventData
	local actualMouseButton = actual.mouseButton or actual.button
	local expectedMouseButton = expected.mouseButton or expected.button

	if ((actual.keyCode or false) ~= (expected.keyCode or false)
		or (actual.isShiftDown or false) ~= (expected.isShiftDown or false)
		or (actual.isControlDown or false) ~= (expected.isControlDown or false)
		or (actual.isAltDown or false) ~= (expected.isAltDown or false)
		or (actual.isSuperDown or false) ~= (expected.isSuperDown or false)
		or (actualMouseButton or false) ~= (expectedMouseButton or false)
		or (actualMouseWheel or false) ~= (expectedMouseWheel or false)) then
		return false
	end

	return true
end

-- Iterator to use TES3::Iterator in a for loop.
-- Only returns values, rather than a key-value pair.
function tes3.iterate(iterator)
	local next, t, k, v = pairs(iterator)
	return function()
		k, v = next(t, k)
		return v
	end
end

-- Iterator to use TES3::TArray in a for loop.
function tes3.loopTArray(tarray)
	local index = 0
	local length = #tarray
	return function()
		index = index + 1
		if (index <= length) then
			return tarray[index]
		end
	end
end

-- Shortcut for the current MenuMode state.
function tes3.menuMode()
	return tes3.getWorldController().flagMenuMode
end

-- Shortcut to check if we're in the main menu.
function tes3.onMainMenu()
	return tes3.worldController.charGenState.value == 0
end

local function getLuaModRuntime(key)
	return mwse.activeLuaMods[key:gsub("[/\\]", "."):lower()] --- @diagnostic disable-line
end

function tes3.getLuaModMetadata(key)
	local runtime = getLuaModRuntime(key)
	if (not runtime) then
		return
	end

	return runtime.metadata
end

-- Checks to see if a lua mod is active.
function tes3.isLuaModActive(key)
	local runtime = getLuaModRuntime(key)
	if (not runtime) then
		return false
	end

	return runtime.initialized == true
end

return tes3
