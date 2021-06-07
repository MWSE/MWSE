tes3statistic
====================================================================================================

A structure that holds statistical information relating to the current and base values of a statistic such as health, fatigue, magicka, attributes, or skills.

Properties
----------------------------------------------------------------------------------------------------

`base`_ (`number`_)
    The base value of the statistic. For example, for a mobile with a normal health amount of 100, fortified by 50, and having taken damage of 25, the base value would be 100, the normal health amount.

`current`_ (`number`_)
    The current value of the statistic. For example, for a mobile with a normal health amount of 100, fortified by 50, and having taken damage of 25, the current value would be 125, the health amount after all buffs and debuffs have been accounted for.

`normalized`_ (`number`_)
    The normalized value of the statistic. For example, for an account with current health of 50 and base health of 100, the normalized value would be equal to current divided by base. 50 (current) / 100 (base) = .5 (normalized). This is useful for quickly calculating percentages for statistics.

.. toctree::
    :hidden:

    tes3statistic/base
    tes3statistic/current
    tes3statistic/normalized

.. _`base`: tes3statistic/base.html
.. _`current`: tes3statistic/current.html
.. _`normalized`: tes3statistic/normalized.html

.. _`number`: ../../lua/type/number.html
