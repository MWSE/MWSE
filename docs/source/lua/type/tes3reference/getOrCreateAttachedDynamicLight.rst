getOrCreateAttachedDynamicLight
====================================================================================================

Fetches the dynamic light attachment. If there isn't one, a new one will be created with the given light and value.

    If no light is supplied as an argument, a point light of radius 512 will be automatically created.
    
    If the light is not attached to any part of the scene graph yet, the point light will be placed as a child of the "attachLight" subnode of the model, or a child of the model if "attachLight" is not found.

Returns
----------------------------------------------------------------------------------------------------

`tes3lightNode`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

light (`niPointLight`_)
    Optional. No description available.

phase (`number`_)
    Optional. No description available.

.. _`niPointLight`: ../../../lua/type/niPointLight.html
.. _`number`: ../../../lua/type/number.html
