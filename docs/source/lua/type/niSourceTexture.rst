niSourceTexture
====================================================================================================

Represents all texture objects that are created from a file or a block of in-memory pixel data. NiSourceTexture objects represent both static and dynamic content, as NiSourceTexture data objects can have their pixel data modified on the fly to implement dynamic texture behavior.

Properties
----------------------------------------------------------------------------------------------------

`fileName`_ (`string`_)
    Read-only. The platform-independent version of the filename from which the image was created, or NULL if the image was created from pixel data.

`formatPrefs`_ (`niFormatPrefs`_)
    The `NiFormatPrefs` of the texture.

`height`_ (`number`_)
    Read-only. The height of the texture.

`isStatic`_ (`boolean`_)
    The static flag.

`name`_ (`string`_)
    The human-facing name of the given object.

`pixelData`_ (`niPixelData`_)
    The app-level pixel data.

`platformFilename`_ (`string`_)
    Read-only. The platform-specific version of the filename.

`references`_ (`string`_)
    Read-only. The number of references that exist for the given object. When this value hits zero, the object's memory is freed.

`runTimeTypeInformation`_ (`niRTTI`_)
    The runtime type information for this object.

`width`_ (`number`_)
    Read-only. The width of the texture.

.. toctree::
    :hidden:

    niSourceTexture/fileName
    niSourceTexture/formatPrefs
    niSourceTexture/height
    niSourceTexture/isStatic
    niSourceTexture/name
    niSourceTexture/pixelData
    niSourceTexture/platformFilename
    niSourceTexture/references
    niSourceTexture/runTimeTypeInformation
    niSourceTexture/width

.. _`fileName`: niSourceTexture/fileName.html
.. _`formatPrefs`: niSourceTexture/formatPrefs.html
.. _`height`: niSourceTexture/height.html
.. _`isStatic`: niSourceTexture/isStatic.html
.. _`name`: niSourceTexture/name.html
.. _`pixelData`: niSourceTexture/pixelData.html
.. _`platformFilename`: niSourceTexture/platformFilename.html
.. _`references`: niSourceTexture/references.html
.. _`runTimeTypeInformation`: niSourceTexture/runTimeTypeInformation.html
.. _`width`: niSourceTexture/width.html

Methods
----------------------------------------------------------------------------------------------------

`clearPixelData`_
    Detaches any pixel data associated with this texture. Any render-specific data will be maintained, and remain in the GPU's memory.

`clone`_ (`niObject`_)
    Creates a copy of this object.

`isInstanceOfType`_ (`boolean`_)
    Determines if the object is of a given type, or of a type derived from the given type. Types can be found in the tes3.niType table.

`isOfType`_ (`boolean`_)
    Determines if the object is of a given type. Types can be found in the tes3.niType table.

`loadPixelDataFromFile`_
    Loads the file associated with the texture into memory, and makes it accessible from the pixelData property.

`prependController`_
    Add a controller to the object as the first controller.

`removeAllControllers`_
    Removes all controllers.

`removeController`_
    Removes a controller from the object.

.. toctree::
    :hidden:

    niSourceTexture/clearPixelData
    niSourceTexture/clone
    niSourceTexture/isInstanceOfType
    niSourceTexture/isOfType
    niSourceTexture/loadPixelDataFromFile
    niSourceTexture/prependController
    niSourceTexture/removeAllControllers
    niSourceTexture/removeController

.. _`clearPixelData`: niSourceTexture/clearPixelData.html
.. _`clone`: niSourceTexture/clone.html
.. _`isInstanceOfType`: niSourceTexture/isInstanceOfType.html
.. _`isOfType`: niSourceTexture/isOfType.html
.. _`loadPixelDataFromFile`: niSourceTexture/loadPixelDataFromFile.html
.. _`prependController`: niSourceTexture/prependController.html
.. _`removeAllControllers`: niSourceTexture/removeAllControllers.html
.. _`removeController`: niSourceTexture/removeController.html

Functions
----------------------------------------------------------------------------------------------------

`createFromPath`_
    Creates an NiSourceTexture from the given filepath.

.. toctree::
    :hidden:

    niSourceTexture/createFromPath

.. _`createFromPath`: niSourceTexture/createFromPath.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niFormatPrefs`: ../../lua/type/niFormatPrefs.html
.. _`niObject`: ../../lua/type/niObject.html
.. _`niPixelData`: ../../lua/type/niPixelData.html
.. _`niRTTI`: ../../lua/type/niRTTI.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
