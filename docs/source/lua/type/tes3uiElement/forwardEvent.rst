forwardEvent
====================================================================================================

Forwards an event to the original Morrowind event handler, using this element as a source. This may be optionally called at any point in a callback. Note that handler may or may not destroy the event widget or the menu, so you should know how it behaves before accessing any elements after a callback. 

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

id (`tes3uiEvent`_)
    The original UI event.

