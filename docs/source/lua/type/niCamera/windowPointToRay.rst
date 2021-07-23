windowPointToRay
====================================================================================================

Given a screen space position, calculates the world position and outlook direction. This can be useful when trying to find a reference under a UI element, such as the cusor.

Returns
----------------------------------------------------------------------------------------------------

`tes3vector3`_. The world point that the given screen position looks out from.

`tes3vector3`_. The look direction of the camera from the given origin point.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

point (`tes3vector2`_)
    The screen position to calculate a world ray for. Screen space is measured as ([-screenWidth/2, screenWidth/2], [-screenHeight/2, screenHeight/2]) with up-right being positive and an origin at the center of the screen.

.. _`tes3vector2`: ../../../lua/type/tes3vector2.html
.. _`tes3vector3`: ../../../lua/type/tes3vector3.html
