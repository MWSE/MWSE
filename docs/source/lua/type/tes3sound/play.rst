play
====================================================================================================

This metod plays the sound.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

playbackFlags (`number`_)
    Optional. These match low-level DirectSound flags. Most mods don't need to modify this, though a value of 1 to loop may be useful. The rest of the flags can be read in the IDirectSoundBuffer8::Play documentation: https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ee418074(v=vs.85).

volume (`number`_)
    Optional. No description available.

pitch (`number`_)
    Optional. No description available.

isNot3D (`boolean`_)
    Optional. No description available.

.. _`boolean`: ../../../lua/type/boolean.html
.. _`number`: ../../../lua/type/number.html
