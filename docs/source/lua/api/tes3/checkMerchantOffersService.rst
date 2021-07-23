tes3.checkMerchantOffersService
====================================================================================================

Checks if a merchant will offer a service to you, including dialogue checks like disposition and faction membership. A specific service can be checked, or if no service is given, a generic dialogue check is made. If the service if refused, the dialogue reply for the refusal may also returned (it may be nil, as there may not always be a reply available).

Returns
----------------------------------------------------------------------------------------------------

`boolean, tes3DialogueInfo`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

reference (`tes3reference`_, `tes3mobileActor`_, `string`_)
    No description available.

service (`number`_)
    Optional. The specific service to check for availability. Uses the tes3.merchantService.* constants.

.. _`number`: ../../../lua/type/number.html
.. _`string`: ../../../lua/type/string.html
.. _`tes3mobileActor`: ../../../lua/type/tes3mobileActor.html
.. _`tes3reference`: ../../../lua/type/tes3reference.html
