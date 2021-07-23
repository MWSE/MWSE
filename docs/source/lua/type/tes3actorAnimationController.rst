tes3actorAnimationController
====================================================================================================

Works in conjuction with mobile AI to perform idle, movement, and attack animations. Holds data on the status of the current and next desired animation states for the different body sections that can be animated.

Properties
----------------------------------------------------------------------------------------------------

`mobile`_ (`tes3mobileActor`_)
    Read-only. The mobile actor that this controller is responsible for.

`weaponSpeed`_ (`number`_)
    Read-only. The animation speed scaling of the weapon animations. This includes all weapon related idle, attack, and ready/unready animations. This value is reset at the starting frame of each weapon strike, normally equal to the the weapon speed. Therefore if you use this it should be changed after every strike, and reset after. Ready/unready actions do not change speed, but are affected by speed.

Use by getting the animation controller from the mobile. e.g. mobile.animationController.weaponSpeed = 0.5

.. toctree::
    :hidden:

    tes3actorAnimationController/mobile
    tes3actorAnimationController/weaponSpeed

.. _`mobile`: tes3actorAnimationController/mobile.html
.. _`weaponSpeed`: tes3actorAnimationController/weaponSpeed.html

.. _`number`: ../../lua/type/number.html
.. _`tes3mobileActor`: ../../lua/type/tes3mobileActor.html
