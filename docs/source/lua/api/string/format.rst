string.format
====================================================================================================

This function creates a string, given various values. The first parameter follows the `printf <http://www.cplusplus.com/reference/cstdio/printf/>`_ format, with the additional option of ``%q`` to automatically quote a string.

========= ======================================================= =============
Specifier Output                                                  Example
========= ======================================================= =============
``%d``    Signed decimal integer.                                 -392
``%i``    Same as ``%d``.                                         -392
``%u``    Unsigned decimal integer.                               7235
``%o``    Unsigned octal.                                         610
``%x``    Unsigned hexadecimal integer.                           7fa
``%X``    As ``%x``, but uppercase.                               7FA
``%f``    Decimal floating point, lowercase.                      392.65
``%F``    Decimal floating point, uppercase.                      392.65
``%e``    Scientific notation (mantissa/exponent), lowercase.     3.9265e+2
``%E``    Scientific notation (mantissa/exponent), uppercase.     3.9265E+2
``%g``    Use the shortest representation: %e or %f.              392.65
``%G``    Use the shortest representation: %E or %F.              392.65
``%a``    Hexadecimal floating point, lowercase.                  -0xc.90fep-2
``%A``    Hexadecimal floating point, uppercase.                  -0XC.90FEP-2
``%c``    Character.                                              a
``%s``    String of characters.                                   sample
``%q``    Quoted string of characters.                            "sample"
``%p``    Pointer address.                                        b8000000
``%%``    The literal ``%`` character.                            %
========= ======================================================= =============

Returns
----------------------------------------------------------------------------------------------------

`string`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

format (`string`_)
    The format string to use for the output.

... (`values`_)
    Optional. Values to format into the given string.

.. _`string`: ../../../lua/type/string.html
