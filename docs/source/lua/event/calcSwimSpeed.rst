calcSwimSpeed
====================================================================================================

One of the movement events, **calcSwimSpeed** is used when calculating the movement speed while in water when running.

The movement event flow is described below:

- Swimming: `calcWalkSpeed`_ -> **calcSwimSpeed** -> `calcMoveSpeed`_
- Swimming (while running): `calcWalkSpeed`_ -> **calcSwimSpeed** -> `calcSwimRunSpeed`_ -> `calcMoveSpeed`_

Event Data
----------------------------------------------------------------------------------------------------

speed
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`number`_. The previous speed calculated, starting with the base engine values.

mobile
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3mobileActor`_. Read-only. The mobile actor whose speed is being calculated.

reference
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3reference`_. Read-only. mobile’s related reference.

type
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`number`_. Read-only. The type of movement that was calculated. This is always 3 for this event.

.. _`calcMoveSpeed`: ../../lua/event/calcMoveSpeed.html
.. _`calcSwimRunSpeed`: ../../lua/event/calcSwimRunSpeed.html
.. _`calcWalkSpeed`: ../../lua/event/calcWalkSpeed.html
.. _`number`: ../../lua/type/number.html
.. _`tes3mobileActor`: ../../lua/type/tes3mobileActor.html
.. _`tes3reference`: ../../lua/type/tes3reference.html
