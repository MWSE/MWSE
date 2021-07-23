weaponSpeed
====================================================================================================

`number`_. The animation speed scaling of the weapon animations. This includes all weapon related idle, attack, and ready/unready animations. This value is reset at the starting frame of each weapon strike, normally equal to the the weapon speed. Therefore if you use this it should be changed after every strike, and reset after. Ready/unready actions do not change speed, but are affected by speed.

Use by getting the animation controller from the mobile. e.g. mobile.animationController.weaponSpeed = 0.5

.. _`number`: ../../../lua/type/number.html
