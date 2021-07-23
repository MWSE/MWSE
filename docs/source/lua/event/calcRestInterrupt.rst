calcRestInterrupt
====================================================================================================

This event is called when the player is about to start resting. The hour and count of creatures can be modified, or disabled. Blocking this event blocks any interrupting spawn.

Event Data
----------------------------------------------------------------------------------------------------

count
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`number`_. The number of creatures interrupting rest. Set to 0 to prevent a spawn.

resting
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`boolean`_. Read-only. If true, the player is attempting to rest, as opposed to wait.

hour
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`number`_. The hour of rest remaining that the rest will be interrupted on.

waiting
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`boolean`_. Read-only. If true, the player is attempting to wait, as opposed to rest.

.. _`boolean`: ../../lua/type/boolean.html
.. _`number`: ../../lua/type/number.html
