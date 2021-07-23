weatherTransitionStarted
====================================================================================================

The **weatherTransitionStarted** event occurs when the currently simulated weather has started to gradually transition to a new weather. This can occur when moving between regions, or when the weather cycles.

Weather will not start transitioning in interiors, but will continue transitioning, and may finish while in an interior. The transition may be interrupted at any time by the weatherChangedImmediate event, and the weatherTransitionFinished event will not occur.

Event Data
----------------------------------------------------------------------------------------------------

to
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3weather`_. The weather object that will be transitioned to.

from
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3weather`_. Read-only. The weather object that is the current weather.

.. _`tes3weather`: ../../lua/type/tes3weather.html
