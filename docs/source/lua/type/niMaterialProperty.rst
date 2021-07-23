niMaterialProperty
====================================================================================================

A rendering property that controls the surface properties of objects affected by the property. These surface properties include translucency, ambient reflective color, diffuse reflective color, emissive color intensity, and specular color.

Properties
----------------------------------------------------------------------------------------------------

`alpha`_ (`number`_)
    The alpha value of the material. Value should be between 0 - 1.

`ambient`_ (`niColor`_)
    The ambient material color.

`diffuse`_ (`niColor`_)
    The diffuse material color.

`emissive`_ (`niColor`_)
    The emissive material color.

`name`_ (`string`_)
    The human-facing name of the given object.

`references`_ (`string`_)
    Read-only. The number of references that exist for the given object. When this value hits zero, the object's memory is freed.

`runTimeTypeInformation`_ (`niRTTI`_)
    The runtime type information for this object.

`shininess`_ (`number`_)
    The shine of the material. Value should be >= 0.

`specular`_ (`niColor`_)
    The specular material color.

`type`_ (`niPropertyType`_)
    The unique class identifier number of the given rendering property.

.. toctree::
    :hidden:

    niMaterialProperty/alpha
    niMaterialProperty/ambient
    niMaterialProperty/diffuse
    niMaterialProperty/emissive
    niMaterialProperty/name
    niMaterialProperty/references
    niMaterialProperty/runTimeTypeInformation
    niMaterialProperty/shininess
    niMaterialProperty/specular
    niMaterialProperty/type

.. _`alpha`: niMaterialProperty/alpha.html
.. _`ambient`: niMaterialProperty/ambient.html
.. _`diffuse`: niMaterialProperty/diffuse.html
.. _`emissive`: niMaterialProperty/emissive.html
.. _`name`: niMaterialProperty/name.html
.. _`references`: niMaterialProperty/references.html
.. _`runTimeTypeInformation`: niMaterialProperty/runTimeTypeInformation.html
.. _`shininess`: niMaterialProperty/shininess.html
.. _`specular`: niMaterialProperty/specular.html
.. _`type`: niMaterialProperty/type.html

Methods
----------------------------------------------------------------------------------------------------

`clone`_ (`niObject`_)
    Creates a copy of this object.

`isInstanceOfType`_ (`boolean`_)
    Determines if the object is of a given type, or of a type derived from the given type. Types can be found in the tes3.niType table.

`isOfType`_ (`boolean`_)
    Determines if the object is of a given type. Types can be found in the tes3.niType table.

`prependController`_
    Add a controller to the object as the first controller.

`removeAllControllers`_
    Removes all controllers.

`removeController`_
    Removes a controller from the object.

.. toctree::
    :hidden:

    niMaterialProperty/clone
    niMaterialProperty/isInstanceOfType
    niMaterialProperty/isOfType
    niMaterialProperty/prependController
    niMaterialProperty/removeAllControllers
    niMaterialProperty/removeController

.. _`clone`: niMaterialProperty/clone.html
.. _`isInstanceOfType`: niMaterialProperty/isInstanceOfType.html
.. _`isOfType`: niMaterialProperty/isOfType.html
.. _`prependController`: niMaterialProperty/prependController.html
.. _`removeAllControllers`: niMaterialProperty/removeAllControllers.html
.. _`removeController`: niMaterialProperty/removeController.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niColor`: ../../lua/type/niColor.html
.. _`niObject`: ../../lua/type/niObject.html
.. _`niRTTI`: ../../lua/type/niRTTI.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
