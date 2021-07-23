setNoCollisionFlag
====================================================================================================

Sets if this reference has active collision. This is preferable to the hasNoCollision property, if are manipulating multiple objects at once and do not want to constantly recalculate collision groups.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

hasNoCollision (`boolean`_)
    If true, the reference no longer has collision.

updateCollisions (`boolean`_)
    If true, collision groups for the active cells are recalculated.

.. _`boolean`: ../../../lua/type/boolean.html
