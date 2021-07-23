tes3actionData
====================================================================================================

A substructure of mobile actors that provides information about the current or previous action.

Properties
----------------------------------------------------------------------------------------------------

`aiBehaviorState`_ (`number`_)
    The behavior state of the tes3actionData.

`animationAttackState`_ (`number`_)
    No description available.

`attackDirection`_ (`number`_)
    **Deprecated. Use tes3actionData.physicalAttackType instead.** When attacking, the direction swung with the weapon. This shows if the actor was thrusting, swinging, or chopping.

`attackSwing`_ (`number`_)
    When attacking, this value represents how much the weapon has been pulled back. The value ranges from [0.0 - 1.0].

`currentAnimationGroup`_ (`number`_)
    No description available.

`hitTarget`_ (`tes3mobileActor`_)
    No description available.

`lastBarterHoursPassed`_ (`number`_)
    No description available.

`nockedProjectile`_ (`tes3weapon`_)
    The currently knocked projectile the associated actor is using.

`physicalAttackType`_ (`number`_)
    A number from the tes3.physicalAttackType enumeration identifying the physical attack type. Can be tes3.physicalAttackType.slash, .chop, .thrust, .projectile, .creature1, .creature2, or .creature3.

`physicalDamage`_ (`number`_)
    When attacking, this is the value of the weapon damage that was rolled. This value doesn't take into account the actor's strength, or other additional damage.

`stolenFrom`_ (`tes3object`_)
    No description available.

`target`_ (`tes3mobileActor`_)
    No description available.

`walkDestination`_ (`tes3vector3`_)
    If moving to a location, this is the position to be walked to.

.. toctree::
    :hidden:

    tes3actionData/aiBehaviorState
    tes3actionData/animationAttackState
    tes3actionData/attackDirection
    tes3actionData/attackSwing
    tes3actionData/currentAnimationGroup
    tes3actionData/hitTarget
    tes3actionData/lastBarterHoursPassed
    tes3actionData/nockedProjectile
    tes3actionData/physicalAttackType
    tes3actionData/physicalDamage
    tes3actionData/stolenFrom
    tes3actionData/target
    tes3actionData/walkDestination

.. _`aiBehaviorState`: tes3actionData/aiBehaviorState.html
.. _`animationAttackState`: tes3actionData/animationAttackState.html
.. _`attackDirection`: tes3actionData/attackDirection.html
.. _`attackSwing`: tes3actionData/attackSwing.html
.. _`currentAnimationGroup`: tes3actionData/currentAnimationGroup.html
.. _`hitTarget`: tes3actionData/hitTarget.html
.. _`lastBarterHoursPassed`: tes3actionData/lastBarterHoursPassed.html
.. _`nockedProjectile`: tes3actionData/nockedProjectile.html
.. _`physicalAttackType`: tes3actionData/physicalAttackType.html
.. _`physicalDamage`: tes3actionData/physicalDamage.html
.. _`stolenFrom`: tes3actionData/stolenFrom.html
.. _`target`: tes3actionData/target.html
.. _`walkDestination`: tes3actionData/walkDestination.html

.. _`number`: ../../lua/type/number.html
.. _`tes3mobileActor`: ../../lua/type/tes3mobileActor.html
.. _`tes3object`: ../../lua/type/tes3object.html
.. _`tes3vector3`: ../../lua/type/tes3vector3.html
.. _`tes3weapon`: ../../lua/type/tes3weapon.html
