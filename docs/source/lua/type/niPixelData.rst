niPixelData
====================================================================================================

Represents 2D arrays of pixel values, as well as pyramids of multiple mipmap levels, each of 2D arrays of pixel values. Also contains information representing the format of the pixels and dimensions of the arrays.

Properties
----------------------------------------------------------------------------------------------------

`bytesPerPixel`_ (`number`_)
    Read-only. The number of bytes per pixel.

`mipMapLevels`_ (`number`_)
    Read-only. The number of MipMap levels.

`references`_ (`string`_)
    Read-only. The number of references that exist for the given object. When this value hits zero, the object's memory is freed.

`runTimeTypeInformation`_ (`niRTTI`_)
    The runtime type information for this object.

.. toctree::
    :hidden:

    niPixelData/bytesPerPixel
    niPixelData/mipMapLevels
    niPixelData/references
    niPixelData/runTimeTypeInformation

.. _`bytesPerPixel`: niPixelData/bytesPerPixel.html
.. _`mipMapLevels`: niPixelData/mipMapLevels.html
.. _`references`: niPixelData/references.html
.. _`runTimeTypeInformation`: niPixelData/runTimeTypeInformation.html

Methods
----------------------------------------------------------------------------------------------------

`clone`_ (`niObject`_)
    Creates a copy of this object.

`createSourceTexture`_ (`niSourceTexture`_)
    Creates a source texture from the current NiPixelData object.

`getHeight`_ (`number`_)
    Returns the height of the mipmap level at the given index, where level 1 is the largest.

`getWidth`_ (`number`_)
    Returns the width of the mipmap level at the given index, where level 1 is the largest.

`isInstanceOfType`_ (`boolean`_)
    Determines if the object is of a given type, or of a type derived from the given type. Types can be found in the tes3.niType table.

`isOfType`_ (`boolean`_)
    Determines if the object is of a given type. Types can be found in the tes3.niType table.

`setPixelsByte`_
    Sets the pixel data from byte data.

`setPixelsFloat`_
    Sets the pixel data from float data.

.. toctree::
    :hidden:

    niPixelData/clone
    niPixelData/createSourceTexture
    niPixelData/getHeight
    niPixelData/getWidth
    niPixelData/isInstanceOfType
    niPixelData/isOfType
    niPixelData/setPixelsByte
    niPixelData/setPixelsFloat

.. _`clone`: niPixelData/clone.html
.. _`createSourceTexture`: niPixelData/createSourceTexture.html
.. _`getHeight`: niPixelData/getHeight.html
.. _`getWidth`: niPixelData/getWidth.html
.. _`isInstanceOfType`: niPixelData/isInstanceOfType.html
.. _`isOfType`: niPixelData/isOfType.html
.. _`setPixelsByte`: niPixelData/setPixelsByte.html
.. _`setPixelsFloat`: niPixelData/setPixelsFloat.html

Functions
----------------------------------------------------------------------------------------------------

`new`_
    Creates a new NiPixelData object.

.. toctree::
    :hidden:

    niPixelData/new

.. _`new`: niPixelData/new.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niObject`: ../../lua/type/niObject.html
.. _`niRTTI`: ../../lua/type/niRTTI.html
.. _`niSourceTexture`: ../../lua/type/niSourceTexture.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
