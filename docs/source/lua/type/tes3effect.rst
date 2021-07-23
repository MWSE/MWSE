tes3effect
====================================================================================================

A structure that defines information for an effect and its associated variables, typically found on spells, alchemy, and enchantments.

Properties
----------------------------------------------------------------------------------------------------

`attribute`_ (`number`_)
    The attribute associated with this effect, or -1 if no attribute is used.

`cost`_ (`number`_)
    The base magicka cost of this effect.

`duration`_ (`number`_)
    How long the effect should last.

`id`_ (`number`_)
    The base tes3magicEffect ID that the effect uses.

`max`_ (`number`_)
    The maximum magnitude of the effect.

`min`_ (`number`_)
    The minimum magnitude of the effect.

`object`_ (`tes3magicEffect`_)
    Read-only. Fetches the tes3magicEffect for the given id used.

`radius`_ (`number`_)
    The radius of the effect.

`rangeType`_ (`number`_)
    Determines if the effect is self, touch, or target ranged.

`skill`_ (`number`_)
    The skill associated with this effect, or -1 if no skill is used.

.. toctree::
    :hidden:

    tes3effect/attribute
    tes3effect/cost
    tes3effect/duration
    tes3effect/id
    tes3effect/max
    tes3effect/min
    tes3effect/object
    tes3effect/radius
    tes3effect/rangeType
    tes3effect/skill

.. _`attribute`: tes3effect/attribute.html
.. _`cost`: tes3effect/cost.html
.. _`duration`: tes3effect/duration.html
.. _`id`: tes3effect/id.html
.. _`max`: tes3effect/max.html
.. _`min`: tes3effect/min.html
.. _`object`: tes3effect/object.html
.. _`radius`: tes3effect/radius.html
.. _`rangeType`: tes3effect/rangeType.html
.. _`skill`: tes3effect/skill.html

.. _`number`: ../../lua/type/number.html
.. _`tes3magicEffect`: ../../lua/type/tes3magicEffect.html
