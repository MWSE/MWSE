uiObjectTooltip
====================================================================================================

The uiObjectTooltip event triggers when a new tooltip is displayed for all in-world objects and items, and inventory tiles in all dialogues. The tooltip will be already be built. Tooltips for inventory tiles are built on mouseover, while tooltips for in-world objects are rebuilt every frame.

Event Data
----------------------------------------------------------------------------------------------------

object
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3physicalObject`_. Read-only. The object being examined.

itemData
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3itemData`_. Read-only. The item data of object, if any.

tooltip
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3uiElement`_. Read-only. The newly created tooltip element. Due to timeouts and target changes, it may be destroyed at any time.

reference
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3reference`_. Read-only. The reference being examined. Only valid for in-world objects. For inventory tiles it will always be nil.

count
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`number`_. Read-only. The number of items in the stack.

.. _`number`: ../../lua/type/number.html
.. _`tes3itemData`: ../../lua/type/tes3itemData.html
.. _`tes3physicalObject`: ../../lua/type/tes3physicalObject.html
.. _`tes3reference`: ../../lua/type/tes3reference.html
.. _`tes3uiElement`: ../../lua/type/tes3uiElement.html
