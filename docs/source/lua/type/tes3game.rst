tes3game
====================================================================================================

A core game object used for storing game settings.

Properties
----------------------------------------------------------------------------------------------------

`parentWindowHandle`_ (`number`_)
    Read-only. The Windows HWND for the parent window.

`playerTarget`_ (`tes3reference`_)
    Read-only. The current activation target.

`renderDistance`_ (`number`_)
    The game's rendering distance.

`screenShotsEnabled`_ (`boolean`_)
    State on if screenshots are enabled.

`screenX`_ (`number`_)
    Read-only. The screen's horizontal position.

`screenY`_ (`number`_)
    Read-only. The screen's vertical position.

`soundQuality`_ (`number`_)
    Sound quality level.

`voice`_ (`number`_)
    Mix volumes for voices.

`volumeEffect`_ (`number`_)
    Mix volumes for effects.

`volumeFootsteps`_ (`number`_)
    Mix volumes for footsteps.

`volumeMaster`_ (`number`_)
    Mix volumes for all sounds.

`volumeMedia`_ (`number`_)
    Mix volumes for music.

`windowHandle`_ (`number`_)
    Read-only. The Windows HWND for the window.

`wireframeProperty`_ (`niProperty`_)
    Read-only. The reused wireframe property, appled when toggling wireframe rendering.

`worldSceneGraphRoot`_ (`niNode`_)
    Read-only. Access to the root of the scene graph.

.. toctree::
    :hidden:

    tes3game/parentWindowHandle
    tes3game/playerTarget
    tes3game/renderDistance
    tes3game/screenShotsEnabled
    tes3game/screenX
    tes3game/screenY
    tes3game/soundQuality
    tes3game/voice
    tes3game/volumeEffect
    tes3game/volumeFootsteps
    tes3game/volumeMaster
    tes3game/volumeMedia
    tes3game/windowHandle
    tes3game/wireframeProperty
    tes3game/worldSceneGraphRoot

.. _`parentWindowHandle`: tes3game/parentWindowHandle.html
.. _`playerTarget`: tes3game/playerTarget.html
.. _`renderDistance`: tes3game/renderDistance.html
.. _`screenShotsEnabled`: tes3game/screenShotsEnabled.html
.. _`screenX`: tes3game/screenX.html
.. _`screenY`: tes3game/screenY.html
.. _`soundQuality`: tes3game/soundQuality.html
.. _`voice`: tes3game/voice.html
.. _`volumeEffect`: tes3game/volumeEffect.html
.. _`volumeFootsteps`: tes3game/volumeFootsteps.html
.. _`volumeMaster`: tes3game/volumeMaster.html
.. _`volumeMedia`: tes3game/volumeMedia.html
.. _`windowHandle`: tes3game/windowHandle.html
.. _`wireframeProperty`: tes3game/wireframeProperty.html
.. _`worldSceneGraphRoot`: tes3game/worldSceneGraphRoot.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`niProperty`: ../../lua/type/niProperty.html
.. _`number`: ../../lua/type/number.html
.. _`tes3reference`: ../../lua/type/tes3reference.html
