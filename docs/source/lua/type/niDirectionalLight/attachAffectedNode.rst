attachAffectedNode
====================================================================================================

Adds a node to the dynamic effect's affected nodes list. The node's ``:updateEffects()`` function should be called afterwards to recognize the change.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

node (`niNode`_)
    The node to add to the affected nodes list.

.. _`niNode`: ../../../lua/type/niNode.html
