# tes3npcInstance

An NPC object that has been cloned. Typically represents an NPC that has been instanced in the world.

This type inherits the following: [tes3actor](../../types/tes3actor), [tes3physicalObject](../../types/tes3physicalObject), [tes3object](../../types/tes3object), [tes3baseObject](../../types/tes3baseObject)
## Properties

### `actorFlags`

*Read-only*. A number representing the actor flags. Truly a bit field.

**Returns**:

* `result` (number)

***

### `aiConfig`

*Read-only*. A substructure off of actors that contains information on the current AI configuration.

**Returns**:

* `result` ([tes3aiConfig](../../types/tes3aiConfig))

***

### `attributes`

Quick access to the base NPC's attributes.

**Returns**:

* `result` (table)

***

### `autoCalc`

Direct access to the actor autocalc flag.

**Returns**:

* `result` (boolean)

***

### `barterGold`

Friendly access to actor's barter gold amount.

**Returns**:

* `result` (number)

***

### `baseDisposition`

The actor's base disposition.

**Returns**:

* `result` (number)

***

### `baseObject`

*Read-only*. Access to the base NPC object.

**Returns**:

* `result` ([tes3npc](../../types/tes3npc))

***

### `blocked`

The blocked state of the object.

**Returns**:

* `result` (boolean)

***

### `blood`

Friendly access to actor's blood type.

**Returns**:

* `result` (number)

***

### `boundingBox`

The bounding box for the object.

**Returns**:

* `result` ([tes3boundingBox](../../types/tes3boundingBox))

***

### `class`

*Read-only*. Quick access to the base NPC's class.

**Returns**:

* `result` ([tes3class](../../types/tes3class))

***

### `cloneCount`

*Read-only*. The number of clones that exist of this actor.

**Returns**:

* `result` (number)

***

### `deleted`

*Read-only*. The deleted state of the object.

**Returns**:

* `result` (boolean)

***

### `disabled`

*Read-only*. The disabled state of the object.

**Returns**:

* `result` (boolean)

***

### `disposition`

*Read-only*. The actor's effective disposition, with all modifiers applied.

**Returns**:

* `result` (number)

***

### `equipment`

*Read-only*. The items currently equipped to the actor.

**Returns**:

* `result` ([tes3iterator](../../types/tes3iterator))

***

### `faction`

*Read-only*. Quick access to the base NPC's faction.

**Returns**:

* `result` ([tes3faction](../../types/tes3faction))

***

### `factionIndex`

Deprecated. Use `reputation` instead.

**Returns**:

* `result` (number)

***

### `fatigue`

*Read-only*. Quick access to the base NPC's fatigue.

**Returns**:

* `result` (number)

***

### `female`

Direct access to the actor female flag.

**Returns**:

* `result` (boolean)

***

### `health`

*Read-only*. Quick access to the base NPC's health.

**Returns**:

* `result` (number)

***

### `height`

*Read-only*. Convenience access to the NPCs (race) height.

**Returns**:

* `result` (number)

***

### `id`

*Read-only*. The unique identifier for the object.

**Returns**:

* `result` (string)

***

### `inventory`

*Read-only*. The items currently carried by the actor.

**Returns**:

* `result` ([tes3inventory](../../types/tes3inventory))

***

### `isAttacked`

*Read-only*. If true, the actor's attacked flag is set.

**Returns**:

* `result` (boolean)

***

### `isEssential`

*Read-only*. If true, the actor's essential flag is set.

**Returns**:

* `result` (boolean)

***

### `isGuard`

*Read-only*. If true, the actor is a guard.

**Returns**:

* `result` (boolean)

***

### `isInstance`

Always returns true.

**Returns**:

* `result` (boolean)

***

### `isLocationMarker`

True if this object is an editor marker for a gameplay location. These include travel, intervention, prison, door, and interior north markers. Markers are invisible in-game.

**Returns**:

* `result` (boolean)

***

### `isRespawn`

*Read-only*. If true, the actor's respawn flag is set.

**Returns**:

* `result` (boolean)

***

### `level`

*Read-only*. Quick access to the base NPC's level.

**Returns**:

* `result` (number)

***

### `magicka`

*Read-only*. Quick access to the base NPC's magicka.

**Returns**:

* `result` (number)

***

### `mesh`

The path to the object's mesh.

**Returns**:

* `result` (string)

***

### `mobile`

*Read-only*. The mobile that the object is represented by.

**Returns**:

* `result` ([tes3mobileActor](../../types/tes3mobileActor))

***

### `modified`

The modification state of the object since the last save.

**Returns**:

* `result` (boolean)

***

### `name`

Quick access to the base NPC's name.

**Returns**:

* `result` (number)

***

### `nextInCollection`

The next object in parent collection's list.

**Returns**:

* `result` ([tes3object](../../types/tes3object))

***

### `objectFlags`

*Read-only*. The raw flags of the object.

**Returns**:

* `result` (number)

***

### `objectType`

*Read-only*. The type of object. Maps to values in [`tes3.objectType`](https://mwse.github.io/MWSE/references/object-types/).

**Returns**:

* `result` (number)

***

### `owningCollection`

The collection responsible for holding this object.

**Returns**:

* `result` ([tes3referenceList](../../types/tes3referenceList))

***

### `persistent`

The persistent flag of the object.

**Returns**:

* `result` (boolean)

***

### `previousInCollection`

The previous object in parent collection's list.

**Returns**:

* `result` ([tes3object](../../types/tes3object))

***

### `race`

*Read-only*. Quick access to the base NPC's race.

**Returns**:

* `result` ([tes3race](../../types/tes3race))

***

### `reference`

*Read-only*. The reference that the object is represented by.

**Returns**:

* `result` ([tes3reference](../../types/tes3reference))

***

### `reputation`

The actor's reputation.

**Returns**:

* `result` (number)

***

### `scale`

The object's scale.

**Returns**:

* `result` (number)

***

### `sceneCollisionRoot`

The scene graph node for this object's physics collision, if its mesh has a root collision node.

**Returns**:

* `result` ([niNode](../../types/niNode))

***

### `sceneNode`

The scene graph node for this object.

**Returns**:

* `result` ([niNode](../../types/niNode))

***

### `sceneReference`

The scene graph reference node for this object.

**Returns**:

* `result` ([niNode](../../types/niNode))

***

### `script`

*Read-only*. Quick access to the base NPC's script.

**Returns**:

* `result` ([tes3script](../../types/tes3script))

***

### `skills`

Quick access to the base NPC's skills.

**Returns**:

* `result` (table)

***

### `soul`

The soul value for this NPC's base object. This is typically `nil`, unless the `calcSouLValue` event provides one for the base actor.

**Returns**:

* `result` (number, nil)

***

### `sourceless`

The soruceless flag of the object.

**Returns**:

* `result` (boolean)

***

### `sourceMod`

*Read-only*. The filename of the mod that owns this object.

**Returns**:

* `result` (string)

***

### `spells`

*Read-only*. Quick access to the base NPC's spell list. It is a `tes3spellList`, which is a list wrapper with helper functions. The actual list is accessed with `.iterator`. e.g. `for _, spell in pairs(npc.spells.iterator) do print(spell.name) end`

**Returns**:

* `result` ([tes3spellList](../../types/tes3spellList))

***

### `stolenList`

A list of actors that the object has been stolen from.

**Returns**:

* `result` ([tes3iterator](../../types/tes3iterator))

***

### `supportsLuaData`

If true, references of this object can store temporary or persistent lua data.

**Returns**:

* `result` (boolean)

***

### `weight`

*Read-only*. Convenience access to the NPCs (race) weight.

**Returns**:

* `result` (number)

***

## Methods

### `__tojson`

Serializes the object to json.

```lua
local string = tes3baseObject:__tojson()
```

**Returns**:

* `string` (string)

***

### `hasItemEquipped`

Checks if the actor has provided item equipped.

```lua
local result = tes3actor:hasItemEquipped(item, itemData)
```

**Parameters**:

* `item` ([tes3item](../../types/tes3item), string): The item to perform a check for.
* `itemData` ([tes3itemData](../../types/tes3itemData)): *Optional*.

**Returns**:

* `result` (boolean)

***

### `offersService`

Checks if the actor will offer a service in dialogue. This an offer and may still be refused by dialogue checks. To also get the result of dialogue checks, use [`tes3.checkMerchantOffersService()`](https://mwse.github.io/MWSE/apis/tes3/#tes3checkmerchantoffersservice).

```lua
local result = tes3actor:offersService(service)
```

**Parameters**:

* `service` (number): Use one of the values in the [`tes3.merchantService.*`](https://mwse.github.io/MWSE/references/merchant-services/) table.

**Returns**:

* `result` (boolean)

***

### `onInventoryClose`

A callback function invoked when an inventory is closed. Typically not used outside of specific purposes. You may find `tes3.reference`'s `onCloseInventory()` to be more convenient to use.

```lua
tes3actor:onInventoryClose(reference)
```

**Parameters**:

* `reference` ([tes3reference](../../types/tes3reference))

***

### `reevaluateEquipment`

Causes the NPC to reevaluate his/hers equipment choices and equip the best available.

```lua
tes3npcInstance:reevaluateEquipment()
```

***

### `tradesItemType`

Checks if the actor will buy and sell items of a given object type. e.g. `actor:tradesItemType(tes3.objectType.repairItem)`

```lua
local result = tes3actor:tradesItemType(objectType)
```

**Parameters**:

* `objectType` (number): Accepts values from [`tes3.objectType`](https://mwse.github.io/MWSE/references/object-types/) namespace.

**Returns**:

* `result` (boolean)

***

