tes3.magicSchoolSkill
====================================================================================================

`tes3.skill.* constants`_. These constants are used to convert magic schools to their respective skill. These constants map to their respective `tes3.skill`_ constants.

=== =====
key value
=== =====
2   10
0   11
3   12
1   13
4   14
5   15
=== =====

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



.. _`tes3.magicSchool`: magicSchool.html
.. _`tes3.skill.* constants`: skill.html
