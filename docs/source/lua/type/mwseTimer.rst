mwseTimer
====================================================================================================

A Timer is a class used to keep track of callback that should be invoked at a later time.

Properties
----------------------------------------------------------------------------------------------------

`callback`_ (`function`_)
    The callback that will be invoked when the timer elapses.

`duration`_ (`number`_)
    The amount of time left on the timer.

`iterations`_ (`number`_)
    The amount of iterations left for the timer.

`state`_ (`number`_)
    The state of the timer, matching timer.active, timer.paused, or timer.expired.

`timeLeft`_ (`number`_)
    The amount of time left before this timer will complete.

`timing`_ (`number`_)
    When this timer ends, or the time remaining if the timer is paused.

.. toctree::
    :hidden:

    mwseTimer/callback
    mwseTimer/duration
    mwseTimer/iterations
    mwseTimer/state
    mwseTimer/timeLeft
    mwseTimer/timing

.. _`callback`: mwseTimer/callback.html
.. _`duration`: mwseTimer/duration.html
.. _`iterations`: mwseTimer/iterations.html
.. _`state`: mwseTimer/state.html
.. _`timeLeft`: mwseTimer/timeLeft.html
.. _`timing`: mwseTimer/timing.html

Methods
----------------------------------------------------------------------------------------------------

`cancel`_
    Cancels the timer.

`pause`_
    Pauses the timer.

`reset`_
    Resets the timer completion time, as if it elapsed. Only works if the timer is active.

`resume`_
    Resumes a paused timer.

.. toctree::
    :hidden:

    mwseTimer/cancel
    mwseTimer/pause
    mwseTimer/reset
    mwseTimer/resume

.. _`cancel`: mwseTimer/cancel.html
.. _`pause`: mwseTimer/pause.html
.. _`reset`: mwseTimer/reset.html
.. _`resume`: mwseTimer/resume.html

.. _`function`: ../../lua/type/function.html
.. _`number`: ../../lua/type/number.html
