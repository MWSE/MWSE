childAlignX
====================================================================================================

`number`_. Sets alignment of child elements inside its parent, though it only works in specific conditions. 0.0 = left/top edge touches left/top edge of parent, 0.5 = centred, 1.0 = right/bottom edge touches right/bottom edge of parent. For negative values, there is a special case behaviour: all children but the last will be left-aligned/top-aligned, the last child will be right-aligned/bottom-aligned.
    
    Child alignment only works if the element has proportional sizing (using widthProportional/heightProportional) and all children use non-proportional sizing (widthProportional and heightProportional are nil).

.. _`number`: ../../../lua/type/number.html
