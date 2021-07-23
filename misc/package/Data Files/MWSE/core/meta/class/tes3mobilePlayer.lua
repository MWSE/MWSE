--- @meta

--- A mobile object for a the player.
--- @class tes3mobilePlayer : tes3mobileNPC, tes3mobileActor, tes3mobileObject
--- @field alwaysRun boolean Toggle flag for if the player should always run.
--- @field attackDisabled boolean Toggle flag for if the player can attack.
--- @field autoRun boolean Toggle flag for if the player should constantly run forward.
--- @field birthsign tes3birthsign The player's birthsign.
--- @field bounty number The player's current bounty.
--- @field cameraHeight number Access to the player camera height, used for both the first person and third person camera (specifically the orbit centre). It is normally placed at head height by the game, on loading, or when a race is selected. The camera height can be reset to its regular value by setting this property to nil.
--- @field castReady boolean Toggle flag for if the player has casting ready.
--- @field clawMultiplier tes3globalVariable Quick access to the ClawMultiplier global variable.
--- @field controlsDisabled boolean Toggle flag for if the player's controls are disabled.
--- @field dialogueList tes3iterator Access to the collection that holds what dialogue topics the player has access to.
--- @field firstPerson tes3npc Quick access to the first person NPC.
--- @field firstPersonReference tes3reference Quick access to the first person NPC's reference.
--- @field inactivityTime number The time that the player has spent inactive.
--- @field inJail boolean Toggle flag for if the player is currently in jail.
--- @field is3rdPerson boolean Shows if the player's camera is currently in 3rd person view.
--- @field jumpingDisabled boolean Toggle flag for if the player can jump.
--- @field knownWerewolf tes3globalVariable Quick access to the KnownWerewolf global variable.
--- @field lastUsedAlembic tes3apparatus The last used alchemy alembic.
--- @field lastUsedAmmoCount number The amount of ammo for the ranged weapon that that was last equipped.
--- @field lastUsedCalcinator tes3apparatus The last used alchemy calcinator.
--- @field lastUsedMortar tes3apparatus The last used alchemy mortar.
--- @field lastUsedRetort tes3apparatus The last used alchemy retort.
--- @field levelupPerSpecialization table Array-style table access to how many skill levels there have been for each of the 3 specializations.
--- @field levelUpProgress number The progress the player has made towards leveling up.
--- @field levelupsPerAttribute table Array-style table access to how many skill levels there have been for each of the 8 attributes.
--- @field magicDisabled boolean Toggle flag for if the player can use magic.
--- @field markLocation tes3markData Access to the structure that holds the player's current mark/recall location.
--- @field mouseLookDisabled boolean Toggle flag for if the player's mouse look controls are disabled.
--- @field restHoursRemaining number How many hours are left while resting.
--- @field skillProgress table Array-style table access to skill progress for each of the 27 skills.
--- @field sleeping boolean Toggle flag for if the player is currently sleeping.
--- @field telekinesis number Direct access to the player's telekinesis effect attribute.
--- @field travelling boolean Toggle flag for if the player is currently travelling.
--- @field vanityDisabled boolean Toggle flag for if the player's vanity camera is disabled.
--- @field viewSwitchDisabled boolean Toggle flag for if the player can switch between first person and vanity cameras.
--- @field visionBonus number Direct access to the player's vision bonus effect attribute.
--- @field waiting boolean Toggle flag for if the player is currently waiting.
--- @field weaponReady boolean Toggle flag for if the player has a weapon ready. Setting it to true will cause the player to take out their weapon. Setting it to false will cause them to put it away.
tes3mobilePlayer = {}

--- Exercises a skill, providing experience in it.
--- @param skill number The skill index to give experience to.
--- @param progress number The amount of experience to grant.
function tes3mobilePlayer:exerciseSkill(skill, progress) end

--- Checks to see if a skill is ready to be leveled up, and performs any levelup logic.
--- @param skill number The skill index to check for leveling.
function tes3mobilePlayer:levelSkill(skill) end

