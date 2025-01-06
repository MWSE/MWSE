# tes3uiColorPreview
<div class="search_terms" style="display: none">tes3uicolorpreview, colorpreview</div>

<!---
	This file is autogenerated. Do not edit this file manually. Your changes will be ignored.
	More information: https://github.com/MWSE/MWSE/tree/master/docs
-->

A widget containing properties specific to color previews.

This type inherits the following: [tes3uiWidget](../types/tes3uiWidget.md)
## Properties

### `alpha`
<div class="search_terms" style="display: none">alpha</div>

The current alpha value of the preview.

**Returns**:

* `result` (number)

***

### `checkerboard`
<div class="search_terms" style="display: none">checkerboard</div>

*Read-only*. The checkerboard image used by the preview element.

**Returns**:

* `result` (Image)

***

### `checkerSize`
<div class="search_terms" style="display: none">checkersize</div>

*Read-only*. The size of individual square in the color preview image in pixels.

**Returns**:

* `result` (integer, nil)

***

### `color`
<div class="search_terms" style="display: none">color</div>

The current color of the preview.

**Returns**:

* `result` (ffiImagePixel)

***

### `darkGray`
<div class="search_terms" style="display: none">darkgray</div>

*Read-only*. The color of darker squares in the color preview image.

**Returns**:

* `result` ([mwseColorTable](../types/mwseColorTable.md), nil)

***

### `element`
<div class="search_terms" style="display: none">element</div>

Access back to the element this widget interface is for.

**Returns**:

* `result` ([tes3uiElement](../types/tes3uiElement.md))

***

### `height`
<div class="search_terms" style="display: none">height</div>

*Read-only*. The height of the individual preview element.

**Returns**:

* `result` (integer)

***

### `image`
<div class="search_terms" style="display: none">image</div>

*Read-only*. The image used by the preview element.

**Returns**:

* `result` (Image)

***

### `lightGray`
<div class="search_terms" style="display: none">lightgray</div>

*Read-only*. The color of lighter squares in the color preview image.

**Returns**:

* `result` ([mwseColorTable](../types/mwseColorTable.md), nil)

***

### `texture`
<div class="search_terms" style="display: none">texture</div>

*Read-only*. The texture used by the preview element.

**Returns**:

* `result` ([niSourceTexture](../types/niSourceTexture.md))

***

### `width`
<div class="search_terms" style="display: none">width</div>

*Read-only*. The width of the individual preview element.

**Returns**:

* `result` (integer)

***

## Methods

### `getAlpha`
<div class="search_terms" style="display: none">getalpha, alpha</div>

Gets the current alpha value.

```lua
local alpha = myObject:getAlpha()
```

**Returns**:

* `alpha` (number)

***

### `getColor`
<div class="search_terms" style="display: none">getcolor, color</div>

Gets the current color of the preview.

```lua
local color = myObject:getColor()
```

**Returns**:

* `color` ([mwseColorTable](../types/mwseColorTable.md))

***

### `getColorAlpha`
<div class="search_terms" style="display: none">getcoloralpha, coloralpha</div>

Gets the current color and alpha.

```lua
local color, alpha = myObject:getColorAlpha()
```

**Returns**:

* `color` ([mwseColorTable](../types/mwseColorTable.md))
* `alpha` (number)

***

### `getRGBA`
<div class="search_terms" style="display: none">getrgba, rgba</div>

Gets the current color and alpha in a single table.

```lua
local color = myObject:getRGBA()
```

**Returns**:

* `color` ([mwseColorATable](../types/mwseColorATable.md))

***

### `setColor`
<div class="search_terms" style="display: none">setcolor, color</div>

Changes the color of this widget to given color.

```lua
myObject:setColor(newColor, alpha)
```

**Parameters**:

* `newColor` ([mwseColorTable](../types/mwseColorTable.md), ffiImagePixel): The new color to set.
* `alpha` (number): *Default*: `1`. Alpha value to set.
