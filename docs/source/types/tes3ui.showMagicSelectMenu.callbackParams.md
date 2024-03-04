# tes3ui.showMagicSelectMenu.callbackParams
<div class="search_terms" style="display: none">tes3ui.showmagicselectmenu.callbackparams, .showmagicselectmenu.callbackparams</div>

<!---
	This file is autogenerated. Do not edit this file manually. Your changes will be ignored.
	More information: https://github.com/MWSE/MWSE/tree/master/docs
-->

The table passed to `tes3ui.showMagicSelectMenu` function's callback.

## Properties

### `item`
<div class="search_terms" style="display: none">item</div>

The enchanted item that has been selected. The actual magic will be `item.enchantment`.

**Returns**:

* `result` ([tes3item](../types/tes3item.md), nil)

***

### `itemData`
<div class="search_terms" style="display: none">itemdata</div>

The item data of the enchanted item that has been selected. Fully recharged items may not have itemData.

**Returns**:

* `result` ([tes3itemData](../types/tes3itemData.md), nil)

***

### `spell`
<div class="search_terms" style="display: none">spell</div>

The spell or power that has been selected.

**Returns**:

* `result` ([tes3spell](../types/tes3spell.md), nil)
