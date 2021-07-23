calcWalkSpeed
====================================================================================================

One of the movement events, **calcWalkSpeed** is used when calculating the base walk speed. Nearly every other movement speed event starts with this, with the exception of `calcFlySpeed`_.

The movement event flow is described below:

- Walking: **calcWalkSpeed** -> `calcMoveSpeed`_
- Running: **calcWalkSpeed** -> `calcRunSpeed`_ -> `calcMoveSpeed`_
- Swimming: **calcWalkSpeed** -> `calcSwimSpeed`_ -> `calcMoveSpeed`_
- Swimming (while running): **calcWalkSpeed** -> `calcSwimSpeed`_ -> `calcSwimRunSpeed`_ -> `calcMoveSpeed`_

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

`number`_. Read-only. The type of movement that was calculated. This is always 1 for this event.

.. _`calcMoveSpeed`: ../../lua/event/calcMoveSpeed.html
.. _`calcRunSpeed`: ../../lua/event/calcRunSpeed.html
.. _`calcSwimRunSpeed`: ../../lua/event/calcSwimRunSpeed.html
.. _`calcSwimSpeed`: ../../lua/event/calcSwimSpeed.html
.. _`number`: ../../lua/type/number.html
.. _`tes3mobileActor`: ../../lua/type/tes3mobileActor.html
.. _`tes3reference`: ../../lua/type/tes3reference.html
