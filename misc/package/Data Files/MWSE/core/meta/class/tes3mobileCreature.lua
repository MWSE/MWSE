--- @meta

--- A mobile object for a creature.
--- @class tes3mobileCreature : tes3mobileActor, tes3mobileObject
--- @field combat tes3statistic Direct access to the creature's combat statistic. This is a creature-specific generalization of skills, where this statistic is used in place of all combat skills. To get an individual skill value by skill ID, see the getSkillValue or getSkillStatistic methods.
--- @field flySpeed number The calculated fly movement speed.
--- @field magic tes3statistic Direct access to the creature's magic statistic. This is a creature-specific generalization of skills, where this statistic is used in place of all magic skills. To get an individual skill value by skill ID, see the getSkillValue or getSkillStatistic methods.
--- @field moveSpeed number The calculated base movement speed.
--- @field object tes3creatureInstance The actor object that maps to this mobile.
--- @field runSpeed number The calculated run movement speed.
--- @field skills table An array-style table with access to the three creature skill statistics.
--- @field stealth tes3statistic Direct access to the creature's stealth statistic. This is a creature-specific generalization of skills, where this statistic is used in place of all stealth skills. To get an individual skill value by skill ID, see the getSkillValue or getSkillStatistic methods.
--- @field swimRunSpeed number The calculated swim movement speed while running.
--- @field swimSpeed number The calculated swim movement speed.
--- @field walkSpeed number The calculated walk movement speed.
tes3mobileCreature = {}

