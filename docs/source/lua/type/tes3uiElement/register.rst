register
====================================================================================================

Sets an ``event`` handler, which can add or override an existing event handler. The use of ``registerBefore`` or ``registerAfter`` is recommended if you do not want to replace the existing event handler. The eventID can be a standard ``event`` name, or an event specific to an element class. The callback receives an argument with the event data. See below for details.
    
The original Morrowind callback is captured and can be invoked with the ``forwardEvent`` method on the event argument. If there is an existing Lua callback, it is replaced.



Lua UI event specification:

Events can be bound to elements via the `Element`_ ``register`` method, which takes an event name. Event names can be one of the standard events listed here, or a widget-specific event.

Standard events:
    **mouseLeave**
        Mouse cursor moving outside an element. Triggers once.
    **mouseOver**
        Mouse cursor moving over an element. Triggers once.
    **mouseDown**
        Left mouse button down over an element.
    **mouseClick**
        Left mouse button up over an element, after a mouseDown over the element.
    **mouseScrollUp**
        ..
    **mouseScrollDown**
        Mouse wheel scrolling.
    **mouseDoubleClick**
        Standard double-click.
    **mouseStillIdle**
        Mouse cursor positioned outside an element. Triggers every frame.
    **mouseStillOver**
        Mouse cursor positioned over an element. Triggers every frame.
    **mouseStillPressed**
        Mouse cursor positioned over an element, after a mouseDown over the element. Triggers every frame.
    **mouseStillPressedOutside**
        Apparently not working in the engine. Mouse cursor positioned outside an element, after a mouseDown over the element. Triggers every frame.
    **mouseRelease**
        Left mouse button up over an element.
    **keyPress**
        A raw key press.
    **keyEnter**
        The Return key is pressed.
    **help**
        On mouseover, but also marking the element as having a tooltip. Create a tooltip within the callback using the `tes3ui.createTooltipMenu`_ function.
    **focus**
        When a menu is clicked on, and moved on top of other menus.
    **unfocus**
        Just before another menu is clicked on, or a widget in that menu receives an event, or when the menu mode is toggled off. You may return false from this event to prevent the menu from being deselected, and to prevent leaving menu mode.
    **preUpdate**
        Before the menu layout is updated.
    **update**
        After the menu layout is updated.
    **destroy**
        When the UI element is destroyed, before any data or children are destroyed.


**Widget-specific events**
    To be documented.


Event forwarding
-------------------------------------------------------------------------------

The original Morrowind event handler is saved when you first register an event. It may be optionally invoked with the ``forwardEvent`` method.  Note that handler may or may not destroy the event widget or the menu, so you should know how it behaves before accessing any elements after a callback. 

**Example**

.. code-block:: lua

    local function onClick(e)
        -- pre-event code
        e.source:forwardEvent(e)
        -- post-event code
    end
    
    local button = menu:findChild("MenuExample_Ok")
    button:register("mouseClick", onClick)


Event handler
-------------------------------------------------------------------------------

The standard type signature for events.

`boolean`_ eventHandler(**EventData** e)
    Returns: ``optional``
        Returning ``false`` may cancel an interaction for certain events. e.g. unfocus
   
    EventData:
        **source** (`Element`_)
            The source element of the event.
        
        **id** (`number`_)
            The numeric id of the event type.
    
        **widget** (`Element`_)
            The widget element that the source belongs to, if the element is a sub-part of a widget. May not be accurate if the element is not a sub-part.
        
        **data0** (`number`_)
            ..
            
        **data1** (`number`_)
            Event-specific raw data values. For mouse events, these are the screen X and Y coordinates of the pointer. For keyboard events, data0 is the `scan code`_.
        
        **relativeX** (`number`_)
            ..
            
        **relativeY** (`number`_)
            For mouse events only. X and Y coordinates of the pointer relative to the top-left of the element.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

eventID (`string`_)
    The event id.

callback (`function`_)
    The callback function.

.. _`function`: ../../../lua/type/function.html
.. _`number`: ../../../lua/type/number.html
.. _`string`: ../../../lua/type/string.html
