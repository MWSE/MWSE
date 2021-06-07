timer.delayOneFrame
====================================================================================================

Creates a timer that will finish the next frame. It defaults to the next simulation frame.

Returns
----------------------------------------------------------------------------------------------------

`mwseTimer`_.

Parameters
----------------------------------------------------------------------------------------------------

callback (`function`_)
    The callback function that will execute when the timer expires.

type (`number`_)
    Default: ``timer.simulate``. Type of the timer. This value can be timer.simulate, timer.game and timer.real.

.. _`number`: ../../../lua/type/number.html
.. _`function`: ../../../lua/type/function.html
.. _`mwseTimer`: ../../../lua/type/mwseTimer.html
