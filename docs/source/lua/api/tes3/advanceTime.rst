tes3.advanceTime
====================================================================================================

Advances the game time. Can be used to simulate player resting.

Returns
----------------------------------------------------------------------------------------------------

`number`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

hours (`number`_)
    How many hours to progress.

resting (`boolean`_)
    Default: ``false``. Should advancing time count as resting? If set to true invokes usual sleeping mechanics: health, fatigue and magicka restoration, and possible rest interruption. The length of the rest will be equal to hours parameter, rounded down to nearest natural number.

updateEnvironment (`boolean`_)
    Default: ``true``. Controls if the weather system is updated for each hour passed.

.. _`boolean`: ../../../lua/type/boolean.html
.. _`number`: ../../../lua/type/number.html
