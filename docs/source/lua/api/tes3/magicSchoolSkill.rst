tes3.magicSchoolSkill
====================================================================================================

`tes3.skill.* constants`_. These constants are used to convert magic schools to their respective skill. These constants map to their respective tes3.skill constants.

Examples
----------------------------------------------------------------------------------------------------

Convert magic school from event data to a respective skill
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The example below gives the player 100 experience for failing to cast a spell. Here we can see how tes3.magicSchoolSkill is used.

.. code-block:: lua

    local function OnSpellFailed(e)

        local skill = tes3.magicSchoolSkill[e.expGainSchool] -- Note: e.expGainSchool is one of tes3.magicSchool(s)

        tes3.mobilePlayer:exerciseSkill(skill, 100)
    end

    event.register("spellCastedFailure", OnSpellFailed)


