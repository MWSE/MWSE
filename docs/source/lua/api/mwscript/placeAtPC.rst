mwscript.placeAtPC
====================================================================================================

Wrapper for the PlaceAtPC mwscript function.

Returns
----------------------------------------------------------------------------------------------------

`tes3reference`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

reference (`tes3reference`_, `tes3mobileActor`_, `string`_)
    Optional. The target reference for this command to be executed on. Defaults to the normal script execution reference.

object (`tes3object`_, `string`_)
    The object to place.

count (`number`_)
    Default: ``1``. No description available.

distance (`number`_)
    Default: ``256``. No description available.

direction (`number`_)
    Default: ``1``. No description available.

.. _`number`: ../../../lua/type/number.html
.. _`string`: ../../../lua/type/string.html
.. _`tes3mobileActor`: ../../../lua/type/tes3mobileActor.html
.. _`tes3object`: ../../../lua/type/tes3object.html
.. _`tes3reference`: ../../../lua/type/tes3reference.html
