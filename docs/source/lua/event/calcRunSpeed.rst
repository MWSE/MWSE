calcRunSpeed
====================================================================================================

One of the movement events, **calcRunSpeed** is used when calculating when the player is running, but not swimming or flying.

The movement event flow is described below:

- Running: `calcWalkSpeed`_ -> **calcRunSpeed** -> `calcMoveSpeed`_

Related events: `calcMoveSpeed`_, `calcWalkSpeed`_, `calcFlySpeed`_, `calcSwimSpeed`_, `calcSwimRunSpeed`_

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

`number`_. Read-only. The type of movement that was calculated. This is always 2 for this event.

.. _`calcFlySpeed`: ../../lua/event/calcFlySpeed.html
.. _`calcMoveSpeed`: ../../lua/event/calcMoveSpeed.html
.. _`calcSwimRunSpeed`: ../../lua/event/calcSwimRunSpeed.html
.. _`calcSwimSpeed`: ../../lua/event/calcSwimSpeed.html
.. _`calcWalkSpeed`: ../../lua/event/calcWalkSpeed.html
.. _`number`: ../../lua/type/number.html
.. _`tes3mobileActor`: ../../lua/type/tes3mobileActor.html
.. _`tes3reference`: ../../lua/type/tes3reference.html
