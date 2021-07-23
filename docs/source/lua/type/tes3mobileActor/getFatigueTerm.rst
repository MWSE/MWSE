getFatigueTerm
====================================================================================================

Gets the fatigue-based skill scaling term used by many game mechanics, based on the actor's current and maximum fatigue. It is equal to ``max(0, fFatigueBase - fFatigueMult * max(0, 1 - fatigue.current/fatigue.base))``

Returns
----------------------------------------------------------------------------------------------------

`number`_.

.. _`number`: ../../../lua/type/number.html
