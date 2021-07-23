widthProportional
====================================================================================================

`boolean`_. Sets element dimensions using a proportional sizer. The sizer starts with the parent dimension in the flow direction, subtracts any fixed dimension children leaving the proportional sizer space. Each proportionally sized element then gets an equal division of the space, multiplied by this member. Values above 1.0 are permissible.
    
    Bug note: If widthProportional is used without heightProportional, an element may not respond to changes in parent size. It is recommended to set heightProportional, or have a fixed size sibling element if dynamic reflow is required.
    
    Overrides fixed, minimum and maximum sizes unless this value is ``nil`` (default).

.. _`boolean`: ../../../lua/type/boolean.html
