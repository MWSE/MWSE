re.match
====================================================================================================

Matches the given pattern against the given subject, returning all captures.

Returns
----------------------------------------------------------------------------------------------------

`string`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

subject (`string`_)
    The string to search.

pattern (`string`_)
    The pattern to search with.

Examples
----------------------------------------------------------------------------------------------------

Simple Match
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Use re.match to find all the words in a string, then find the first numeral.

.. code-block:: lua

    local re = require("re")

    local function onLoaded(e)

        -- returns all words in a string
        mwse.log("%s %s %s %s", re.match("the number 423 is odd", "({%a+} / .)*"))
        --> the    number    is    odd

        -- returns the first numeral in a string
        mwse.log(re.match("the number 423 is odd", "s <- {%d+} / . s"))
        --> 423

    end

    event.register("loaded", onLoaded)


.. _`string`: ../../../lua/type/string.html
