<!---
	This file is autogenerated. Do not edit this file manually. Your changes will be ignored.
	More information: https://github.com/MWSE/MWSE/tree/master/docs
-->

# tes3animationData

A game object which contains information on actor's animations.

Animations are divided into three layers. The layer 0 is the base layer containing all the base animations for every humanoid in the game, including the player (when in third person). These animations come from `base_anim.nif`. Layer 1 is for female animations. Any animations present in `base_anim_female.nif` override their male counterparts for humanoid races. For beast races, layer 1 animations come from `base_anim_kna.nif`. Layer 2 are the custom animations assigned to the actor.

## Properties

### `actorNode`



**Returns**:

* `result` ([niNode](../../types/niNode))

***

### `animationGroups`

*Read-only*. The animation groups, indexed by the [`tes3.animationGroup`](https://mwse.github.io/MWSE/references/animation-groups/) namespace.

**Returns**:

* `result` (tes3animationGroup[])

***

### `animationGroupSoundgenCounts`

*Read-only*. The number of sound generators for each of the animation groups, indexed by the [`tes3.animationGroup`](https://mwse.github.io/MWSE/references/animation-groups/) namespace.

**Returns**:

* `result` (number[])

***

### `animGroupLayerIndicies`

*Read-only*. The layer from which each of the actor's animation groups come, indexed by the [`tes3.animationGroup`](https://mwse.github.io/MWSE/references/animation-groups/) namespace.

**Returns**:

* `result` (number[])

***

### `approxRootTravelSpeeds`

*Read-only*. The approximate root node travel speed for each of the animation groups, indexed by the [`tes3.animationGroup`](https://mwse.github.io/MWSE/references/animation-groups/) namespace.

**Returns**:

* `result` (number[])

***

### `currentAnimGroupLayers`

*Read-only*. 

**Returns**:

* `result` (number[])

***

### `currentAnimGroups`

*Read-only*. The currently playing [animation group](https://mwse.github.io/MWSE/references/animation-groups/), on each of the [body sections](https://mwse.github.io/MWSE/references/animation-body-sections/).

**Returns**:

* `result` (number[])

***

### `currentNodeIndices`

*Read-only*. 

**Returns**:

* `result` (number[])

***

### `currentSoundgenIndices`

*Read-only*. The index of the currently used sound generator for each of the [body sections](https://mwse.github.io/MWSE/references/animation-body-sections/).

**Returns**:

* `result` (number[])

***

### `deltaTime`



**Returns**:

* `result` (number)

***

### `flags`



**Returns**:

* `result` (number)

***

### `hasOverrideAnimations`

*Read-only*. 

**Returns**:

* `result` (boolean)

***

### `headGeometry`



**Returns**:

* `result` ([niGeometry](../../types/niGeometry))

***

### `headNode`



**Returns**:

* `result` ([niNode](../../types/niNode))

***

### `keyframeLayers`

*Read-only*. 

**Returns**:

* `result` ([tes3animationDataSequenceGroup](../../types/tes3animationDataSequenceGroup)[])

***

### `lipsyncLevel`



**Returns**:

* `result` (number)

***

### `loopCounts`

*Read-only*. 

**Returns**:

* `result` (number[])

***

### `manager`



**Returns**:

* `result` (niKeyframeManager)

***

### `modelRootNode`



**Returns**:

* `result` ([niNode](../../types/niNode))

***

### `movementSpeed`

The animation speed multiplier of movement animations. This includes walking, running, crouching, swimming, turning, jumping and other movement related animations.

**Returns**:

* `result` (number)

***

### `nextLoopCounts`



**Returns**:

* `result` (number)

***

### `positionDeltaModelRoot`



**Returns**:

* `result` ([tes3vector3](../../types/tes3vector3))

***

### `spine1Node`



**Returns**:

* `result` ([niNode](../../types/niNode))

***

### `spine2Node`



**Returns**:

* `result` ([niNode](../../types/niNode))

***

### `spineAngle`



**Returns**:

* `result` (number)

***

### `timings`



**Returns**:

* `result` (number[])

***

### `weaponSpeed`

The animation speed multiplier of weapon animations. This includes all weapon related idle, attack, and ready/unready animations.

**Returns**:

* `result` (number)

***

## Methods

### `getReference`

This method fetches the reference of the actor to whom this animation data belongs.

```lua
local result = myObject:getReference()
```

**Returns**:

* `result` ([tes3reference](../../types/tes3reference))

***

### `playAnimationGroup`

This method plays an animation group on the related actor, invoking `playGroup` event.

```lua
myObject:playAnimationGroup(animationGroup, startFlag, loopCount)
```

**Parameters**:

* `animationGroup` (number): The animation group to play. A value from [`tes3.animationGroup`](https://mwse.github.io/MWSE/references/animation-groups/) namespace.
* `startFlag` (number): A flag for starting the group with, using [`tes3.animationStartFlag`](https://mwse.github.io/MWSE/references/animation-start-flags/) constants.
* `loopCount` (number): If provided, the animation will repeat its loop section a given number of times. To make an animation play through once, set loopCount = 0, while -1 is used for infinite looping.

***

### `playAnimationGroupForIndex`

This method plays an animation group on the provided body section of related actor, invoking `playGroup` event.

```lua
myObject:playAnimationGroupForIndex(animationGroup, triIndex, startFlag, loopCount)
```

**Parameters**:

* `animationGroup` (number): The animation group to play. A value from [`tes3.animationGroup`](https://mwse.github.io/MWSE/references/animation-groups/) namespace.
* `triIndex` (number): The body section on which to play the animation. A value from [`tes3.animationBodySection`](https://mwse.github.io/MWSE/references/animation-body-sections/) namespace.
* `startFlag` (number): A flag for starting the group with, using [`tes3.animationStartFlag`](https://mwse.github.io/MWSE/references/animation-start-flags/) constants.
* `loopCount` (number): If provided, the animation will repeat its loop section a given number of times. To make an animation play through once, set loopCount = 0, while -1 is used for infinite looping.

***

### `setHeadNode`



```lua
myObject:setHeadNode(head)
```

**Parameters**:

* `head` ([niNode](../../types/niNode)): 

***

### `setOverrideLayerKeyframes`



```lua
local success = myObject:setOverrideLayerKeyframes(kfData)
```

**Parameters**:

* `kfData` (tes3keyframeDefinition): 

**Returns**:

* `success` (boolean)

***
