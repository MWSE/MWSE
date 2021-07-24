weatherCycled
====================================================================================================

The **weatherCycled** event occurs when region weathers are randomized. This occurs at an interval set by the ini option “Hours Between Weather Changes”. Regions may select the same weather as before, and there will be no transition in this case.

Related events: `weatherTransitionFinished`_, `weatherChangedImmediate`_, `weatherTransitionStarted`_, `weatherChangedImmediate`_

.. _`weatherChangedImmediate`: ../../lua/event/weatherChangedImmediate.html
.. _`weatherTransitionFinished`: ../../lua/event/weatherTransitionFinished.html
.. _`weatherTransitionStarted`: ../../lua/event/weatherTransitionStarted.html
