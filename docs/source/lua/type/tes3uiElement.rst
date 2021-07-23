tes3uiElement
====================================================================================================

A UI element, the main building block of the UI system. All elements are created with methods on a parent Element.  Elements are very configurable, and have many HTML-like layout features. All layout properties can be set to ``nil`` to reset them to the default value, which will deactivate any related layout mode.

Elements can have custom data attached using their `Property`_ key-value store, and specific Elements have specific ``element.widget`` accessors to control behaviour.

Properties
----------------------------------------------------------------------------------------------------

`absolutePosAlignX`_ (`number`_)
    Sets element position to a point relative to the parent element. 0.0 = left/top content edge, 1.0 = right/bottom content edge. The positioning is absolute, which frees the element from the standard flow layout and allows overlapping elements.
    
    Bug note: Elements may not respond to widthProportional/heightProportional sizing after either of these properties are set. If you need to use both you should consider testing if it works first.

`absolutePosAlignY`_ (`number`_)
    See absolutePosAlignY.

`alpha`_ (`number`_)
    Element alpha colour, using range [0.0, 1.0]. Used to composite elements. If you wish to hide an element completely, use ``disable`` instead.

`autoHeight`_ (`boolean`_)
    When ``true``, automatically expands element dimensions to fit child elements. Dimensions are restricted by minWidth, minHeight, maxWidth and maxHeight properties.

`autoWidth`_ (`boolean`_)
    When ``true``, automatically expands element dimensions to fit child elements. Dimensions are restricted by minWidth, minHeight, maxWidth and maxHeight properties.

`borderAllSides`_ (`number`_)
    Integer number. Border size in pixels. Border is the extra empty space around an element. Individual border sizes default to using the borderAllSides setting.

`borderBottom`_ (`number`_)
    Integer number. Bottom border size in pixels. When this is set to -1, the borderAllSides setting is used for this side instead.

`borderLeft`_ (`number`_)
    Integer number. Left border size in pixels. When this is set to -1, the borderAllSides setting is used for this side instead.

`borderRight`_ (`number`_)
    Integer number. Left border size in pixels. When this is set to -1, the borderAllSides setting is used for this side instead.

`borderTop`_ (`number`_)
    Integer number. Top border size in pixels. When this is set to -1, the borderAllSides setting is used for this side instead.

`childAlignX`_ (`number`_)
    Sets alignment of child elements inside its parent, though it only works in specific conditions. 0.0 = left/top edge touches left/top edge of parent, 0.5 = centred, 1.0 = right/bottom edge touches right/bottom edge of parent. For negative values, there is a special case behaviour: all children but the last will be left-aligned/top-aligned, the last child will be right-aligned/bottom-aligned.
    
    Child alignment only works if the element has proportional sizing (using widthProportional/heightProportional) and all children use non-proportional sizing (widthProportional and heightProportional are nil).

`childAlignY`_ (`number`_)
    See childAlignX.

`childOffsetX`_ (`number`_)
    Integer number. View offset in pixels, applied to the position of child nodes. Used in scroll panes.

`childOffsetY`_ (`number`_)
    See childOffsetX.

`children`_ (`table`_)
    Read-only. A table containing references to child elements. This is a copy that does not track changes.

`color`_ (`table`_)
    Element RGB colour, an array of 3 floats with value range [0.0, 1.0]. For menus and rects, it sets the background colour. For text, it sets the text colour. For images, it multiplies the image by the colour.

`consumeMouseEvents`_ (`boolean`_)
    When ``true``, mouse events over this element are sent to event handlers, or discarded if there is no handler. When ``false``, mouse events go upwards to the first ancestor that can consume mouse events. Useful to set on widget sub-elements. ``true`` by default.

`disabled`_ (`boolean`_)
    Disables user actions on this element. Widgets may stop accepting mouse and keyboard input while disabled.

`flowDirection`_ (`string`_)
    Can have values ``"left_to_right"`` or ``"top_to_bottom"``. Indicates which direction child elements are laid out.

`font`_ (`number`_)
    Index of font to use for text.
        0 - Magic Cards (default)
        1 - Century Sans
        2 - Daedric

`height`_ (`number`_)
    Element dimensions in pixels. Integer number.

`heightProportional`_ (`boolean`_)
    Sets element dimensions using a proportional sizer. The sizer starts with the parent dimension in the flow direction, subtracts any fixed dimension children leaving the proportional sizer space. Each proportionally sized element then gets an equal division of the space, multiplied by this member. Values above 1.0 are permissible.
    
    Bug note: If widthProportional is used without heightProportional, an element may not respond to changes in parent size. It is recommended to set heightProportional, or have a fixed size sibling element if dynamic reflow is required.
    
    Overrides fixed, minimum and maximum sizes unless this value is ``nil`` (default).

`id`_ (`number`_)
    Read-only. The element's ID.  The element can be later accessed by ``ancestor:findChild(id)``. Note that multiple elements may have the same ID, such as subparts of a widget, or list items. Therefore, you may think of ids as an element class identifier.

`imageScaleX`_ (`number`_)
    Image scaling multipliers. Only applies to image elements.

`imageScaleY`_ (`number`_)
    Image scaling multipliers. Only applies to image elements.

`justifyText`_ (`string`_)
    Can have values ``"left"``, ``"center"``, or ``"right"``. Controls text justification. To work correctly for center/right justification,  ``wrapText`` must be ``true``.

`maxHeight`_ (`number`_)
    Maximum dimensions for auto-size layout and resizable frames. Integer number.

`maxWidth`_ (`number`_)
    Maximum dimensions for auto-size layout and resizable frames. Integer number.

`minHeight`_ (`number`_)
    Minimum dimensions for auto-size layout and resizable frames. Integer number.

`minWidth`_ (`number`_)
    Minimum dimensions for auto-size layout and resizable frames. Integer number.

`name`_ (`string`_)
    Read-only. The element's name, taken from the name registered for the ID.

`paddingAllSides`_ (`number`_)
    Integer number. Padding size in pixels. Padding is the blank space between the edge of an element and its contents. Individual padding sizes default to -1, making it use the paddingAllSides setting.

`paddingBottom`_ (`number`_)
    Integer number. Bottom padding size in pixels. When this is set to -1, the paddingAllSides setting is used for this side instead.

`paddingLeft`_ (`number`_)
    Integer number. Left padding size in pixels. When this is set to -1, the paddingAllSides setting is used for this side instead.

`paddingRight`_ (`number`_)
    Integer number. Right padding size in pixels. When this is set to -1, the paddingAllSides setting is used for this side instead.

`paddingTop`_ (`number`_)
    Integer number. Top padding size in pixels. When this is set to -1, the paddingAllSides setting is used for this side instead.

`parent`_ (`tes3uiElement`_)
    Read-only. A reference to the parent element.

`positionX`_ (`number`_)
    Integer number. Element X position relative to its parent's top-left content area. For top-level menus there is a difference: (0, 0) is the centre of the screen.

`positionY`_ (`number`_)
    Integer number. Element Y position relative to its parent's top-left content area. For top-level menus there is a difference: (0, 0) is the centre of the screen.

`rawText`_ (`string`_)
    The raw value of the element's text. This, unlike the normal text property, will not directly read widget information or handle the removal of the positional cursor.

`repeatKeys`_ (`boolean`_)
    Controls if there is repeating text input when keys are held down. ``true`` by default.

`scaleMode`_ (`boolean`_)
    When set to ``true`` on image and NIF elements, they are scaled to fit ``width`` and ``height``.

`text`_ (`string`_)
    The element's text. Text input can be read by accessing this property.

`visible`_ (`boolean`_)
    Controls if the element is visible.

`widget`_ (`table`_)
    Access to custom properties specific to a widget. These properties are documented in the widget create* function descriptions.

`width`_ (`number`_)
    Element dimensions in pixels. Integer number.

`widthProportional`_ (`boolean`_)
    Sets element dimensions using a proportional sizer. The sizer starts with the parent dimension in the flow direction, subtracts any fixed dimension children leaving the proportional sizer space. Each proportionally sized element then gets an equal division of the space, multiplied by this member. Values above 1.0 are permissible.
    
    Bug note: If widthProportional is used without heightProportional, an element may not respond to changes in parent size. It is recommended to set heightProportional, or have a fixed size sibling element if dynamic reflow is required.
    
    Overrides fixed, minimum and maximum sizes unless this value is ``nil`` (default).

`wrapText`_ (`boolean`_)
    Controls text wrapping. Setting this to ``true`` will also set ``layoutHeightFraction`` to ``1.0``, which is required for wrapping text to adjust to its container size.

.. toctree::
    :hidden:

    tes3uiElement/absolutePosAlignX
    tes3uiElement/absolutePosAlignY
    tes3uiElement/alpha
    tes3uiElement/autoHeight
    tes3uiElement/autoWidth
    tes3uiElement/borderAllSides
    tes3uiElement/borderBottom
    tes3uiElement/borderLeft
    tes3uiElement/borderRight
    tes3uiElement/borderTop
    tes3uiElement/childAlignX
    tes3uiElement/childAlignY
    tes3uiElement/childOffsetX
    tes3uiElement/childOffsetY
    tes3uiElement/children
    tes3uiElement/color
    tes3uiElement/consumeMouseEvents
    tes3uiElement/disabled
    tes3uiElement/flowDirection
    tes3uiElement/font
    tes3uiElement/height
    tes3uiElement/heightProportional
    tes3uiElement/id
    tes3uiElement/imageScaleX
    tes3uiElement/imageScaleY
    tes3uiElement/justifyText
    tes3uiElement/maxHeight
    tes3uiElement/maxWidth
    tes3uiElement/minHeight
    tes3uiElement/minWidth
    tes3uiElement/name
    tes3uiElement/paddingAllSides
    tes3uiElement/paddingBottom
    tes3uiElement/paddingLeft
    tes3uiElement/paddingRight
    tes3uiElement/paddingTop
    tes3uiElement/parent
    tes3uiElement/positionX
    tes3uiElement/positionY
    tes3uiElement/rawText
    tes3uiElement/repeatKeys
    tes3uiElement/scaleMode
    tes3uiElement/text
    tes3uiElement/visible
    tes3uiElement/widget
    tes3uiElement/width
    tes3uiElement/widthProportional
    tes3uiElement/wrapText

.. _`absolutePosAlignX`: tes3uiElement/absolutePosAlignX.html
.. _`absolutePosAlignY`: tes3uiElement/absolutePosAlignY.html
.. _`alpha`: tes3uiElement/alpha.html
.. _`autoHeight`: tes3uiElement/autoHeight.html
.. _`autoWidth`: tes3uiElement/autoWidth.html
.. _`borderAllSides`: tes3uiElement/borderAllSides.html
.. _`borderBottom`: tes3uiElement/borderBottom.html
.. _`borderLeft`: tes3uiElement/borderLeft.html
.. _`borderRight`: tes3uiElement/borderRight.html
.. _`borderTop`: tes3uiElement/borderTop.html
.. _`childAlignX`: tes3uiElement/childAlignX.html
.. _`childAlignY`: tes3uiElement/childAlignY.html
.. _`childOffsetX`: tes3uiElement/childOffsetX.html
.. _`childOffsetY`: tes3uiElement/childOffsetY.html
.. _`children`: tes3uiElement/children.html
.. _`color`: tes3uiElement/color.html
.. _`consumeMouseEvents`: tes3uiElement/consumeMouseEvents.html
.. _`disabled`: tes3uiElement/disabled.html
.. _`flowDirection`: tes3uiElement/flowDirection.html
.. _`font`: tes3uiElement/font.html
.. _`height`: tes3uiElement/height.html
.. _`heightProportional`: tes3uiElement/heightProportional.html
.. _`id`: tes3uiElement/id.html
.. _`imageScaleX`: tes3uiElement/imageScaleX.html
.. _`imageScaleY`: tes3uiElement/imageScaleY.html
.. _`justifyText`: tes3uiElement/justifyText.html
.. _`maxHeight`: tes3uiElement/maxHeight.html
.. _`maxWidth`: tes3uiElement/maxWidth.html
.. _`minHeight`: tes3uiElement/minHeight.html
.. _`minWidth`: tes3uiElement/minWidth.html
.. _`name`: tes3uiElement/name.html
.. _`paddingAllSides`: tes3uiElement/paddingAllSides.html
.. _`paddingBottom`: tes3uiElement/paddingBottom.html
.. _`paddingLeft`: tes3uiElement/paddingLeft.html
.. _`paddingRight`: tes3uiElement/paddingRight.html
.. _`paddingTop`: tes3uiElement/paddingTop.html
.. _`parent`: tes3uiElement/parent.html
.. _`positionX`: tes3uiElement/positionX.html
.. _`positionY`: tes3uiElement/positionY.html
.. _`rawText`: tes3uiElement/rawText.html
.. _`repeatKeys`: tes3uiElement/repeatKeys.html
.. _`scaleMode`: tes3uiElement/scaleMode.html
.. _`text`: tes3uiElement/text.html
.. _`visible`: tes3uiElement/visible.html
.. _`widget`: tes3uiElement/widget.html
.. _`width`: tes3uiElement/width.html
.. _`widthProportional`: tes3uiElement/widthProportional.html
.. _`wrapText`: tes3uiElement/wrapText.html

Methods
----------------------------------------------------------------------------------------------------

`createBlock`_ (`tes3uiElement`_)
    Creates an empty block container inside the element. Used to group and layout elements.

`createButton`_ (`tes3uiElement`_)
    Creates a clickable button. Register the "mouseClick" event to capture a button press.
    
    Custom widget properties:
        | `number`_ ``element.widget.state``: Interaction state. 1 = normal, 2 = disabled, 4 = active. Controls which colour set to use for text.
        | `table`_ (float[3]) ``element.widget.idle``: Text colour for normal state, no mouse interaction.
        | `table`_ (float[3]) ``element.widget.over``: Text colour for normal state, on mouseOver.
        | `table`_ (float[3]) ``element.widget.pressed``: Text colour for normal state, on mouseDown.
        | `table`_ (float[3]) ``element.widget.idleDisabled``: Text colour for disabled state, no mouse interaction.
        | `table`_ (float[3]) ``element.widget.overDisabled``: Text colour for disabled state, on mouseOver.
        | `table`_ (float[3]) ``element.widget.pressedDisabled``: Text colour for disabled state, on mouseDown.
        | `table`_ (float[3]) ``element.widget.idleActive``: Text colour for active state, no mouse interaction.
        | `table`_ (float[3]) ``element.widget.overActive``: Text colour for active state, on mouseOver.
        | `table`_ (float[3]) ``element.widget.pressedActive``: Text colour for active state, on mouseDown.

`createDivider`_ (`tes3uiElement`_)
    Creates a textured horizontal divider line, as used in the magic and stat menus. Scales automatically to the container width.

`createFillBar`_ (`tes3uiElement`_)
    Creates a horizontal quantity indicator bar.

    Custom widget properties:
        | `number`_ (integer) ``element.widget.current``: Current (filled) value.
        | `number`_ (integer) ``element.widget.max``: Maximum value.
        | `boolean`_ ``element.widget.showText``: If text of the format "current/max" is shown. Default is ``true``.
        | `table`_ (float[3]) ``element.widget.fillColor``: Colour of filled area.
        | `number`_ (float) ``element.widget.fillAlpha``: Alpha transparency of filled area.

`createHorizontalScrollPane`_ (`tes3uiElement`_)
    Creates a horizontally scrolling pane.

    Custom widget properties:
        | `number`_ ``element.widget.positionX``: Horizontal scroll offset in pixels.
        | `boolean`_ ``element.widget.scrollbarVisible``: Set if the scrollbar is displayed.

    Custom widget methods:
        | ``element.widget:contentsChanged()``: Call to update scroll bar slider and limits after adding or removing elements to the content container. Because content size depends on layout, this must be run after a menu:updateLayout(). Only required if the content size changes.

`createHypertext`_ (`tes3uiElement`_)
    Creates a text area with clickable words as links. Usage is still under research.

`createImage`_ (`tes3uiElement`_)
    Creates an image element from a texture file. The texture must have power-of-2 dimensions (i.e. 16, 32, 64, 128, 256, 512, 1024); the final display size can be trimmed by setting width and height.

`createImageButton`_ (`tes3uiElement`_)
    Creates an interactive button composed of images for the **idle**, **over**, and **pressed** states. The texture must have power-of-2 dimensions (i.e. 16, 32, 64, 128, 256, 512, 1024); the final display size can be trimmed by setting width and height.

`createLabel`_ (`tes3uiElement`_)
    Creates a text label. It defaults to displaying all text on a single line. To get a multi-line label, set ``wrap_text`` to ``true``. The element is created with ``autoWidth`` and ``autoHeight`` turned on.

`createNif`_ (`tes3uiElement`_)
    Creates a NIF model from a file. Still under research.

`createParagraphInput`_ (`tes3uiElement`_)
    Creates a multi-line text input element, with line wrapping on. To receive input the keyboard must be captured with ``tes3ui.acquireTextInput(element)``. Read the input with the ``text`` property. Write an initial value to edit by setting the ``text`` property.

    Custom widget properties:
        | `number`_ (integer) ``element.widget.lengthLimit``: Maximum input length. Default is ``1023``.

`createRect`_ (`tes3uiElement`_)
    Creates a filled rectangle. The rectangle is displayed as filled with the element's colour. It supports alpha compositing.

`createSlider`_ (`tes3uiElement`_)
    Creates a horizontal slider.

    Custom widget properties:
        | `number`_ (integer) ``element.widget.current``: Current value.
        | `number`_ (integer) ``element.widget.max``: Maximum value.
        | `number`_ (integer) ``element.widget.step``: Amount changed by left and right arrow buttons.
        | `number`_ (integer) ``element.widget.jump``: Amount changed by clicking inside the slider area.

    Custom events used with register:
        | ``"PartScrollBar_changed"``: Triggers on value change; moving the slider is not enough if the value is the same.

`createSliderVertical`_ (`tes3uiElement`_)
    Creates a vertical slider.

    Custom widget properties:
        | `number`_ (integer) ``element.widget.current``: Current value.
        | `number`_ (integer) ``element.widget.max``: Maximum value.
        | `number`_ (integer) ``element.widget.step``: Amount changed by up and down arrow buttons.
        | `number`_ (integer) ``element.widget.jump``: Amount changed by clicking inside the slider area.

    Custom events used with register:
        | ``"PartScrollBar_changed"``: Triggers on value change; moving the slider is not enough if the value is the same.

`createTextInput`_ (`tes3uiElement`_)
    Creates a single line text input element. To receive input the keyboard must be captured with ``tes3ui.acquireTextInput(element)``. Read the input with the ``text`` property. Write an initial value to display by setting the ``text`` property; that value will be cleared on the first keypress.

    Custom widget properties:
        | `boolean`_ ``element.widget.eraseOnFirstKey``: Clears the initial value if the first keypress is not an edit action. Default is ``true``.
        | `number`_ (integer) ``element.widget.lengthLimit"``: Maximum input length, or ``nil`` for no limit. If you are setting names, the engine limits most identifiers to 31 characters. Default is ``nil``.

`createTextSelect`_ (`tes3uiElement`_)
    Creates a selectable line of text, with configurable hover, click, and disabled colours. Can be used to create a list box by placing them in a ScrollPane. ``state`` sets the initial interaction state, documented below.

    Custom widget properties:
        | `number`_ ``element.widget.state``: Interaction state. 1 = normal, 2 = disabled, 4 = active. Controls which colour set to use.
        | `table`_ (float[3]) ``element.widget.idle``: Colour for normal state, no mouse interaction.
        | `table`_ (float[3]) ``element.widget.over``: Colour for normal state, on mouseOver.
        | `table`_ (float[3]) ``element.widget.pressed``: Colour for normal state, on mouseDown.
        | `table`_ (float[3]) ``element.widget.idleDisabled``: Colour for disabled state, no mouse interaction.
        | `table`_ (float[3]) ``element.widget.overDisabled``: Colour for disabled state, on mouseOver.
        | `table`_ (float[3]) ``element.widget.pressedDisabled``: Colour for disabled state, on mouseDown.
        | `table`_ (float[3]) ``element.widget.idleActive``: Colour for active state, no mouse interaction.
        | `table`_ (float[3]) ``element.widget.overActive``: Colour for active state, on mouseOver.
        | `table`_ (float[3]) ``element.widget.pressedActive``: Colour for active state, on mouseDown.

`createThinBorder`_ (`tes3uiElement`_)
    Creates a styled thin border element. Any content should be created as children of this border.

`createVerticalScrollPane`_ (`tes3uiElement`_)
    Creates a vertically scrolling pane. Useful as a list box.

    Custom widget properties:
        | `number`_ ``element.widget.positionY``: Vertical scroll offset in pixels.
        | `boolean`_ ``element.widget.scrollbarVisible``: Set if the scrollbar is displayed.

    Custom widget methods:
        | ``element.widget:contentsChanged()``: Call to update scroll bar slider and limits after adding or removing elements to the content container. Because content size depends on layout, this must be run after a menu:updateLayout(). Only required if the content size changes.

`destroy`_
    Deletes an element and all its child elements. If any element is bound to text input by `tes3ui.acquireTextInput`_, the input is automatically released.

`destroyChildren`_
    Deletes all the child elements of this element. If any element is bound to text input by `tes3ui.acquireTextInput`_, the input is automatically released.

`findChild`_ (`tes3uiElement`_)
    Finds a child element matching the ``id`` argument. Searches children recursively. Returns the first child element with a matching id, or ``nil`` if no match found.

`forwardEvent`_
    Forwards an event to the original Morrowind event handler, using this element as a source. This may be optionally called at any point in a callback. Note that handler may or may not destroy the event widget or the menu, so you should know how it behaves before accessing any elements after a callback. 

`getContentElement`_ (`tes3uiElement`_)
    Returns the descendant element that creation functions used on this element will place children into, or the calling element if there is no specific descendant for children.

    Some widgets like ScrollPanes are built of multiple layers of elements. When an element is created in a complex widget, it is automatically placed as a child of a content element, but other functions do not access this content element directly. This function finds this content container for any element, so that changing layout and accessing children is possible. For simple elements, the calling element will be returned so that there is always a valid container element.

`getPropertyBool`_ (`boolean`_)
    Properties are named variables attached to an element. Gets a property value with ``propName`` as the property key. Morrowind uses these to bind variables to the UI. Useful for element class-specific properties. Returns ``false`` if the property key did not have data.

`getPropertyFloat`_ (`number`_)
    Properties are named variables attached to an element. Gets a property value with ``propName`` as the property key. Morrowind uses these to bind variables to the UI. Useful for element class-specific properties. Returns 0 if the property key did not have data.

`getPropertyInt`_ (`number`_)
    Properties are named variables attached to an element. Gets a property value with ``propName`` as the property key. Morrowind uses these to bind variables to the UI. Useful for element class-specific properties. Returns 0 if the property key did not have data.

`getPropertyObject`_ (`object`_)
    Properties are named variables attached to an element. Gets a property value with ``propName`` as the property key. Morrowind uses these to bind variables to the UI. Useful for element class-specific properties. Returns ``nil`` if the property key did not have data, or if it could not guess the type of the property value.
    
    For ``getPropertyObject``, an expected return value not derived from ``tes3baseObject`` requires the usertype to be provided as well. Currently accepted usertypes are:
	``tes3itemData``
	``tes3itemStack``
	``tes3gameFile``
	``tes3inventoryTile``
	``tes3uiElement``
	
	This is required because there is no identifying type information stored with the pointer. If the usertype is incorrect, there are no safety checks keeping the game from crashing.

`getTopLevelMenu`_ (`tes3uiElement`_)
    Finds the parent menu containing the element. Useful for finding the correct menu to run layout updates on.

`register`_
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

`registerAfter`_
    Sets an ``event`` handler to run after any existing event handler on the element. Can be any event usable with ``register``. The callback receives an argument with the event data. See ``register`` for details.

`registerBefore`_
    Sets an ``event`` handler to run before any existing event handler on the element. Can be any event usable with ``register``. The callback receives an argument with the event data. See ``register`` for details.

`reorderChildren`_ (`boolean`_)
        Moves the layout order of the children of this element. ``count`` elements are taken from starting child `Element`_ or index (0-based) ``moveFrom``, and moved before the child `Element`_ or index (0-based) ``insertBefore``. If ``count`` is -1, all children after ``moveFrom`` are moved. If any index is a negative number, then the index represents a distance from the end of the child list.
    
    Returns ``true`` if the operation succeeded, or ``false`` if at least one argument was invalid.

`setPropertyBool`_
    Sets a property value with ``prop`` as the property key. Properties are named variables attached to an element. Gets a property value with ``propName`` as the property key. Morrowind uses these to bind variables to the UI. Useful for element class-specific properties.

`setPropertyFloat`_
    Sets a property value with ``prop`` as the property key. Properties are named variables attached to an element. Gets a property value with ``propName`` as the property key. Morrowind uses these to bind variables to the UI. Useful for element class-specific properties.

`setPropertyInt`_
    Sets a property value with ``prop`` as the property key. Properties are named variables attached to an element. Gets a property value with ``propName`` as the property key. Morrowind uses these to bind variables to the UI. Useful for element class-specific properties.

`setPropertyObject`_
    Sets a property value with ``prop`` as the property key. Properties are named variables attached to an element. Gets a property value with ``propName`` as the property key. Morrowind uses these to bind variables to the UI. Useful for element class-specific properties.

`triggerEvent`_
    Triggers a UI event on an element, either using supplied event data, or by constructing new event data using ``eventName``. ``eventName`` is the same as used in ``register``.

`unregister`_
    Unregisters an `event`_ handler.

`updateLayout`_
    Updates a menu's element layout and all child elements. Needs to be called on a top level menu when any elements contained in it are added, moved or resized.

.. toctree::
    :hidden:

    tes3uiElement/createBlock
    tes3uiElement/createButton
    tes3uiElement/createDivider
    tes3uiElement/createFillBar
    tes3uiElement/createHorizontalScrollPane
    tes3uiElement/createHypertext
    tes3uiElement/createImage
    tes3uiElement/createImageButton
    tes3uiElement/createLabel
    tes3uiElement/createNif
    tes3uiElement/createParagraphInput
    tes3uiElement/createRect
    tes3uiElement/createSlider
    tes3uiElement/createSliderVertical
    tes3uiElement/createTextInput
    tes3uiElement/createTextSelect
    tes3uiElement/createThinBorder
    tes3uiElement/createVerticalScrollPane
    tes3uiElement/destroy
    tes3uiElement/destroyChildren
    tes3uiElement/findChild
    tes3uiElement/forwardEvent
    tes3uiElement/getContentElement
    tes3uiElement/getPropertyBool
    tes3uiElement/getPropertyFloat
    tes3uiElement/getPropertyInt
    tes3uiElement/getPropertyObject
    tes3uiElement/getTopLevelMenu
    tes3uiElement/register
    tes3uiElement/registerAfter
    tes3uiElement/registerBefore
    tes3uiElement/reorderChildren
    tes3uiElement/setPropertyBool
    tes3uiElement/setPropertyFloat
    tes3uiElement/setPropertyInt
    tes3uiElement/setPropertyObject
    tes3uiElement/triggerEvent
    tes3uiElement/unregister
    tes3uiElement/updateLayout

.. _`createBlock`: tes3uiElement/createBlock.html
.. _`createButton`: tes3uiElement/createButton.html
.. _`createDivider`: tes3uiElement/createDivider.html
.. _`createFillBar`: tes3uiElement/createFillBar.html
.. _`createHorizontalScrollPane`: tes3uiElement/createHorizontalScrollPane.html
.. _`createHypertext`: tes3uiElement/createHypertext.html
.. _`createImage`: tes3uiElement/createImage.html
.. _`createImageButton`: tes3uiElement/createImageButton.html
.. _`createLabel`: tes3uiElement/createLabel.html
.. _`createNif`: tes3uiElement/createNif.html
.. _`createParagraphInput`: tes3uiElement/createParagraphInput.html
.. _`createRect`: tes3uiElement/createRect.html
.. _`createSlider`: tes3uiElement/createSlider.html
.. _`createSliderVertical`: tes3uiElement/createSliderVertical.html
.. _`createTextInput`: tes3uiElement/createTextInput.html
.. _`createTextSelect`: tes3uiElement/createTextSelect.html
.. _`createThinBorder`: tes3uiElement/createThinBorder.html
.. _`createVerticalScrollPane`: tes3uiElement/createVerticalScrollPane.html
.. _`destroy`: tes3uiElement/destroy.html
.. _`destroyChildren`: tes3uiElement/destroyChildren.html
.. _`findChild`: tes3uiElement/findChild.html
.. _`forwardEvent`: tes3uiElement/forwardEvent.html
.. _`getContentElement`: tes3uiElement/getContentElement.html
.. _`getPropertyBool`: tes3uiElement/getPropertyBool.html
.. _`getPropertyFloat`: tes3uiElement/getPropertyFloat.html
.. _`getPropertyInt`: tes3uiElement/getPropertyInt.html
.. _`getPropertyObject`: tes3uiElement/getPropertyObject.html
.. _`getTopLevelMenu`: tes3uiElement/getTopLevelMenu.html
.. _`register`: tes3uiElement/register.html
.. _`registerAfter`: tes3uiElement/registerAfter.html
.. _`registerBefore`: tes3uiElement/registerBefore.html
.. _`reorderChildren`: tes3uiElement/reorderChildren.html
.. _`setPropertyBool`: tes3uiElement/setPropertyBool.html
.. _`setPropertyFloat`: tes3uiElement/setPropertyFloat.html
.. _`setPropertyInt`: tes3uiElement/setPropertyInt.html
.. _`setPropertyObject`: tes3uiElement/setPropertyObject.html
.. _`triggerEvent`: tes3uiElement/triggerEvent.html
.. _`unregister`: tes3uiElement/unregister.html
.. _`updateLayout`: tes3uiElement/updateLayout.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`table`: ../../lua/type/table.html
.. _`tes3uiElement`: ../../lua/type/tes3uiElement.html
