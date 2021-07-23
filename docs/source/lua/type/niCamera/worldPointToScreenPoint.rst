worldPointToScreenPoint
====================================================================================================

Given a world coordinate position, calculates where that point relates to the screen space.

Returns
----------------------------------------------------------------------------------------------------

`tes3vector2`_. The screen space position. Screen space is measured as ([-screenWidth/2, screenWidth/2], [-screenHeight/2, screenHeight/2]) with up-right being positive and an origin at the center of the screen.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

point (`tes3vector3`_)
    The world position to calculate screen position for.

.. _`tes3vector2`: ../../../lua/type/tes3vector2.html
.. _`tes3vector3`: ../../../lua/type/tes3vector3.html
