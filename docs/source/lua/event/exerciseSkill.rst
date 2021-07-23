exerciseSkill
====================================================================================================

This event is invoked whenever the player gains experience in a skill. The event can be blocked to prevent progress. Additionally, both the skill gaining experience and the progress gained can be changed.

Event Data
----------------------------------------------------------------------------------------------------

skill
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`number`_. Read-only. The skill that is gaining experience. This can be changed to provide experience to a different skill.

progress
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`number`_. The amount of experience that skill is gaining. Note that experience is not on a scale of 1 to 100. This value is modifiable.

.. _`number`: ../../lua/type/number.html
