lpeg.match
====================================================================================================

Attempt to match the given pattern against the subject string.
	
If the match succeeds, return the index in the subject of the first character after the match, or
the captured values (if the pattern captured any value).

An optional numeric argument init makes the match start at that position in the subject string. As 
usual in Lua libraries, a negative value counts from the end.

Unlike typical pattern-matching functions, match works only in anchored mode; that is, it tries to 
match the pattern with a prefix of the given subject string (at position init), not with an 
arbitrary substring of the subject. So, if we want to find a pattern anywhere in a string, we must
either write a loop in Lua or write a pattern that matches anywhere. This second approach is easy 
and quite efficient; see examples in the full documentation for details.

Returns
----------------------------------------------------------------------------------------------------

`values`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

p (`pattern`_)
    The pattern to match.

subject (`string`_)
    The string to match against.

init (`number`_)
    Optional. Start the match at this position in subject.

Examples
----------------------------------------------------------------------------------------------------

Simple Match
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Create a pattern that matches a word followed by the end of the string, and use it to match strings.

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


.. _`number`: ../../../lua/type/number.html
.. _`string`: ../../../lua/type/string.html
