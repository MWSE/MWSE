timer.start
====================================================================================================

Creates a timer.

Returns
----------------------------------------------------------------------------------------------------

`mwseTimer`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

type (`number`_)
    Default: ``timer.simulate``. Type of the timer. This value can be timer.simulate, timer.game or timer.real.

duration (`number`_)
    Duration of the timer. The method of time passing depends on the timer type.

callback (`function`_)
    The callback function that will execute when the timer expires.

iterations (`number`_)
    Default: ``1``. The number of iterations to run. Use ``-1`` for infinite looping.

Examples
----------------------------------------------------------------------------------------------------

Show a Countdown Message
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In this example, whenever the player activates an item they will be shown a 10 second countdown. Because it is a simulation timer, it will not tick down while the menu is open, nor is it sensitive to the game timescale/clock.

.. code-block:: lua

    local timeLeft = 0;

    local function onTimerExpired() 
        timeLeft = timeLeft - 1;
        tes3.messageBox("%d seconds left", timeLeft);
    end

    local function onActivate(eventData)
        if (eventData.activator == tes3.player) then
            timeLeft = 10;
            timer.start{ duration = 1, iterations = 10, type = timer.simulate, callback = onTimerExpired }
        end
    end
    event.register("activate", onActivate)


.. _`function`: ../../../lua/type/function.html
.. _`mwseTimer`: ../../../lua/type/mwseTimer.html
.. _`number`: ../../../lua/type/number.html
