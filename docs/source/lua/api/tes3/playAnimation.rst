tes3.playAnimation
====================================================================================================

Plays a given animation group. Optional flags can be used to define how the group starts.

Animation Start Flags
----------------------------------------------------------------------------------------------------

The ``startFlag`` parameter determines under what condition the new animation will play.

- ``tes3.animationStartFlag.normal``: The current animation will finish its full cycle, and the new animation will start from its beginning.
- ``tes3.animationStartFlag.immediate``: The current animation will stop regardless of the frame it is on, and the new animation will start from its beginning.
- ``tes3.animationStartFlag.immediateLoop``: Immediate Loop. The current animation will stop regardless of the frame it is on, and the new animation will start at the beginning of its loop cycle.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

reference (`tes3mobileActor`_, `tes3reference`_, `string`_)
    The reference that will play the animation.

group (`number`_)
    Optional. The animation group id to start playing -- a value from 0 to 149. Applies the animation to the whole body. Maps to tes3.animationGroup.* constants.

lower (`number`_)
    Optional. Sets the animation group id for the lower body. This is used to combine different animations for each body section. Maps to tes3.animationGroup.* constants.

upper (`number`_)
    Optional. Sets the animation group id for the upper body. This is used to combine different animations for each body section. Maps to tes3.animationGroup.* constants.

shield (`number`_)
    Optional. Sets the animation group id for the shield arm. This is used to combine different animations for each body section. Maps to tes3.animationGroup.* constants.

startFlag (`number`_)
    Default: ``tes3.animationStartFlag.immediate``. A flag for starting the group with, using tes3.animationStartFlag.* constants.

loopCount (`number`_)
    Default: ``-1``. If provided, the animation will repeat its loop section a given number of times. To make an animation play through once, set loopCount = 0. Defaults to infinite looping.

mesh (`string`_)
    Optional. Deprecated. Please use tes3.loadAnimation (check its documentation) before calling playAnimation. You can also use loadAnimation to reset loaded animations to default.

.. _`number`: ../../../lua/type/number.html
.. _`string`: ../../../lua/type/string.html
.. _`tes3mobileActor`: ../../../lua/type/tes3mobileActor.html
.. _`tes3reference`: ../../../lua/type/tes3reference.html
