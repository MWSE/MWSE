updateDerivedStatistics
====================================================================================================

Updates statistics derived from attributes, which are magicka, fatigue, and encumbrance. Will also update the UI if used on the player. Normally handled automatically when you use tes3.modStatistic.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

attribute (`tes3statistic`_)
    Optional. Limits the update to statistics derived from this attribute.  e.g. ``mobile:updateDerivedStatistics(mobile.strength)``. If not present, all derived statistics will be updated.

.. _`tes3statistic`: ../../../lua/type/tes3statistic.html
