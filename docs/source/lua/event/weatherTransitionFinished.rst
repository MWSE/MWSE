weatherTransitionFinished
====================================================================================================

The **weatherTransitionFinished** event occurs when the currently simulated weather finished transitioning to a new weather.

Weather will continue transitioning in interiors, and may finish while in an interior. The transition may be interrupted at any time by the weatherChangedImmediate event, and this finish event will not be triggered.

Related events: `weatherCycled`_, `weatherChangedImmediate`_, `weatherTransitionStarted`_, `weatherChangedImmediate`_

Event Data
----------------------------------------------------------------------------------------------------

to
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3weather`_. The weather object that will be transitioned to.

.. _`weatherChangedImmediate`: ../../lua/event/weatherChangedImmediate.html
.. _`weatherCycled`: ../../lua/event/weatherCycled.html
.. _`weatherTransitionStarted`: ../../lua/event/weatherTransitionStarted.html
.. _`tes3weather`: ../../lua/type/tes3weather.html
