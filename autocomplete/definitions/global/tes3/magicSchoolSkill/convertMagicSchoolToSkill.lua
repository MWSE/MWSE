local function OnSpellFailed(e)

	local skill = tes3.magicSchoolSkill[e.expGainSchool] -- Note: e.expGainSchool is one of tes3.magicSchool(s)

	tes3.mobilePlayer:exerciseSkill(skill, 100)
end

event.register("spellCastedFailure", OnSpellFailed)
