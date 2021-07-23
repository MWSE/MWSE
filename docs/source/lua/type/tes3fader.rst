tes3fader
====================================================================================================

An object that applies a graphical effect on the screen, such as screen glare or damage coloring.

Properties
----------------------------------------------------------------------------------------------------

`active`_ (`boolean`_)
    The activation state for the fader. Setting this effectively calls activate/deactivate.

.. toctree::
    :hidden:

    tes3fader/active

.. _`active`: tes3fader/active.html

Methods
----------------------------------------------------------------------------------------------------

`activate`_
    Activates a deactivated fader.

`deactivate`_
    Deactivates an activated fader.

`fadeIn`_
    Transitions the fader to a value of 1 over a given duration.

`fadeOut`_
    Transitions the fader to a value of 0 over a given duration.

`fadeTo`_
    Transitions the fader to a value over a given duration.

`setColor`_ (`boolean`_)
    Applies a coloring effect to the fader.

`setTexture`_
    Updates the fader for the current frame.

`update`_
    Updates the fader for the current frame.

`updateMaterialProperty`_
    Updates the fader for the current frame.

.. toctree::
    :hidden:

    tes3fader/activate
    tes3fader/deactivate
    tes3fader/fadeIn
    tes3fader/fadeOut
    tes3fader/fadeTo
    tes3fader/setColor
    tes3fader/setTexture
    tes3fader/update
    tes3fader/updateMaterialProperty

.. _`activate`: tes3fader/activate.html
.. _`deactivate`: tes3fader/deactivate.html
.. _`fadeIn`: tes3fader/fadeIn.html
.. _`fadeOut`: tes3fader/fadeOut.html
.. _`fadeTo`: tes3fader/fadeTo.html
.. _`setColor`: tes3fader/setColor.html
.. _`setTexture`: tes3fader/setTexture.html
.. _`update`: tes3fader/update.html
.. _`updateMaterialProperty`: tes3fader/updateMaterialProperty.html

Functions
----------------------------------------------------------------------------------------------------

`new`_ (`tes3fader`_)
    Creates a new fader, and adds it to the fader system.

.. toctree::
    :hidden:

    tes3fader/new

.. _`new`: tes3fader/new.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`tes3fader`: ../../lua/type/tes3fader.html
