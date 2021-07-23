lpeg.R
====================================================================================================

Returns a pattern that matches any single character belonging to one of the given ranges.
	
Each range is a string xy of length 2, representing all characters with code between the codes of x and y (both inclusive).

As an example, the pattern lpeg.R("09") matches any digit, and lpeg.R("az", "AZ") matches any ASCII letter.

Returns
----------------------------------------------------------------------------------------------------

`pattern`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

p (`pattern`_)
    The input pattern.

Examples
----------------------------------------------------------------------------------------------------

Simple Match
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Create a pattern using lpeg.R that matches a word followed by the end of the string.

.. code-block:: lua

    local lpeg = require("lpeg")

    local function onLoaded(e)

        -- matches a word followed by end-of-string
        p = lpeg.R("az")^1 * -1

        mwse.log(p:match("hello"))        --> 6
        mwse.log(lpeg.match(p, "hello"))  --> 6
        mwse.log(p:match("1 hello"))      --> nil

    end

    event.register("loaded", onLoaded)


