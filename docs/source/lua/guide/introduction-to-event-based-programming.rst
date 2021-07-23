
Introduction to Event Based Programming
========================================================
Event Based Programming, also known as Event Driven Programming or Event Driven Architecture, is an architectural design where code is executed in response to the triggering of an event, also known as a notification, in a software system. 

A practical example of this is this website. If you click a button on this website, something happens. In other words, your action, clicking on a button, triggers an event. More generally, an action starts a chain of events that results in some other action occurring.

This architectural design is the means through which mods can be made for Morrowind using the MWSE-Lua API. 

Event Based Programming In Context
--------------------------------------------------------
MWSE provides a variety of events that can be used to execute code. The details of which events are available and what event data is exposed to them can be found on the `events`_ page.

For example, consider the *loaded* event. This event occurs after a save game has been loaded. When you register a function for this event, your function will be executed when the event occurs. MWSE will automatically handle triggering the event. All you have to do is register your code to be executed. 

Here is an example of how to register a function for the *loaded* event:

.. code-block:: lua
   :linenos:

    -- The function to call on the loaded event.
    local function loadedHandler()
        -- Execute our code.
    end
    
    -- Register our loadedHandler function to the loaded event.
    event.register("loaded", loadedHandler) 

By registering functions to be executed on certain events, powerful functions that extend, override, or alter game logic can be created. 

Event Filtering
--------------------------------------------------------
Whenever you register your function for an event, it is executed every time the event occurs. This is not always what you want. For example, you may want to register a function for the *keyDown* event, but only have your code trigger when a certain key is pressed. You can use event filters for this.

.. code-block:: lua
   :linenos:

    -- Filter by the scan code to get Z key presses only.
    event.register("key", myOnKeyCallback, { filter = tes3.scanCode.z } )

Different filters can be applied to different events. Please refer to the documentation for the event that you want to filter for more information.

Conclusion
--------------------------------------------------------
This guide introduced Event Based Programming and explained some aspects of it in relation to MWSE. This guide also explained how to use event filtering when registering events in MWSE.

For more in-depth documentation on Event Based Programming, please refer to this `Event Based Programming`_ article, or other external sources.

.. _`events`: ../event.html
.. _`Event Based Programming`: https://www.technologyuk.net/computing/software-development/software-design/event-driven-programming.shtml