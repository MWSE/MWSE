getPropertyObject
====================================================================================================

Properties are named variables attached to an element. Gets a property value with ``propName`` as the property key. Morrowind uses these to bind variables to the UI. Useful for element class-specific properties. Returns ``nil`` if the property key did not have data, or if it could not guess the type of the property value.
    
    For ``getPropertyObject``, an expected return value not derived from ``tes3baseObject`` requires the usertype to be provided as well. Currently accepted usertypes are:
	``tes3itemData``
	``tes3itemStack``
	``tes3gameFile``
	``tes3inventoryTile``
	``tes3uiElement``
	
	This is required because there is no identifying type information stored with the pointer. If the usertype is incorrect, there are no safety checks keeping the game from crashing.

Returns
----------------------------------------------------------------------------------------------------

`object`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

propName (`string`_)
    The property name.

expectedUsertype (`string`_)
    Optional. A Lua usertype name, if expecting a non-standard object type.

.. _`string`: ../../../lua/type/string.html
