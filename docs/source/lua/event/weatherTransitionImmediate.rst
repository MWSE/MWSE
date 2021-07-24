weatherTransitionImmediate
====================================================================================================

The **weatherChangedImmediate** event occurs when the currently simulated weather is changed without transition. This can occur when going from an interior to an exterior in a new region, or while resting.

This can interrupt weather transitions, which means the weatherTransitionFinished event will not be triggered.

Related events: `weatherCycled`_, `weatherTransitionFinished`_, `weatherChangedImmediate`_, `weatherTransitionStarted`_, `weatherChangedImmediate`_

Event Data
----------------------------------------------------------------------------------------------------

to
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3weather`_. The weather object that is the new current weather.

.. _`weatherChangedImmediate`: ../../lua/event/weatherChangedImmediate.html
.. _`weatherCycled`: ../../lua/event/weatherCycled.html
.. _`weatherTransitionFinished`: ../../lua/event/weatherTransitionFinished.html
.. _`weatherTransitionStarted`: ../../lua/event/weatherTransitionStarted.html
.. _`tes3weather`: ../../lua/type/tes3weather.html
