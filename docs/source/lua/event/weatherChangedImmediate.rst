weatherChangedImmediate
====================================================================================================

The weatherChangedImmediate event occurs when the currently simulated weather is changed without transition. This can occur when going from an interior to an exterior in a new region, or while resting. This can interrupt weather transitions, which means the weatherTransitionFinished event will not be triggered.

Event Data
----------------------------------------------------------------------------------------------------

to
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3weather`_. The weather object that is the new current weather. Can be modified to change behavior.

.. _`tes3weather`: ../../lua/type/tes3weather.html
