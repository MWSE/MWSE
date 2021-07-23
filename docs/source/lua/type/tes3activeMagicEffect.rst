tes3activeMagicEffect
====================================================================================================

An active magic effect.

Properties
----------------------------------------------------------------------------------------------------

`attributeId`_ (`number`_)
    Read-only. The attribute ID (note that this may be the skill ID if the effect affects skills).

`duration`_ (`number`_)
    Read-only. The total duration of this specific effect.

`effectId`_ (`number`_)
    Read-only. The magic effect ID.

`effectIndex`_ (`number`_)
    Read-only. The index of the effect inside the magic source instance.

`effectInstance`_ (`tes3magicEffectInstance`_)
    Read-only. The magic effect instance for this effect.

`harmful`_ (`boolean`_)
    Read-only. No description available.

`instance`_ (`tes3magicSourceInstance`_)
    Read-only. The magic source instance for this effect.

`magnitude`_ (`number`_)
    Read-only. The unresisted magnitude of the magic effect, that is the magnitude before resistance attributes are applied. This is always an integer number. To find the actual applied magnitude, you must check the magic effect instance.

`magnitudeMin`_ (`number`_)
    Read-only. Deprecated.

`mobile`_ (`tes3magicSourceInstance`_)
    Read-only. The mobile affected by the magic effect.

`next`_ (`tes3activeMagicEffect`_)
    Read-only. Deprecated.

`previous`_ (`tes3activeMagicEffect`_)
    Read-only. Deprecated.

`serial`_ (`number`_)
    Read-only. The magic source instance serial number. See the ``instance`` and ``effectInstance`` properties to easily access the magic and effect instances.

`skillId`_ (`number`_)
    Read-only. The skill ID (note that this may be the attribute ID if the effect affects attributes).

.. toctree::
    :hidden:

    tes3activeMagicEffect/attributeId
    tes3activeMagicEffect/duration
    tes3activeMagicEffect/effectId
    tes3activeMagicEffect/effectIndex
    tes3activeMagicEffect/effectInstance
    tes3activeMagicEffect/harmful
    tes3activeMagicEffect/instance
    tes3activeMagicEffect/magnitude
    tes3activeMagicEffect/magnitudeMin
    tes3activeMagicEffect/mobile
    tes3activeMagicEffect/next
    tes3activeMagicEffect/previous
    tes3activeMagicEffect/serial
    tes3activeMagicEffect/skillId

.. _`attributeId`: tes3activeMagicEffect/attributeId.html
.. _`duration`: tes3activeMagicEffect/duration.html
.. _`effectId`: tes3activeMagicEffect/effectId.html
.. _`effectIndex`: tes3activeMagicEffect/effectIndex.html
.. _`effectInstance`: tes3activeMagicEffect/effectInstance.html
.. _`harmful`: tes3activeMagicEffect/harmful.html
.. _`instance`: tes3activeMagicEffect/instance.html
.. _`magnitude`: tes3activeMagicEffect/magnitude.html
.. _`magnitudeMin`: tes3activeMagicEffect/magnitudeMin.html
.. _`mobile`: tes3activeMagicEffect/mobile.html
.. _`next`: tes3activeMagicEffect/next.html
.. _`previous`: tes3activeMagicEffect/previous.html
.. _`serial`: tes3activeMagicEffect/serial.html
.. _`skillId`: tes3activeMagicEffect/skillId.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`number`: ../../lua/type/number.html
.. _`tes3activeMagicEffect`: ../../lua/type/tes3activeMagicEffect.html
.. _`tes3magicEffectInstance`: ../../lua/type/tes3magicEffectInstance.html
.. _`tes3magicSourceInstance`: ../../lua/type/tes3magicSourceInstance.html
