# mwseMCMExclusionsPageFilter
<div class="search_terms" style="display: none">mwsemcmexclusionspagefilter</div>

<!---
	This file is autogenerated. Do not edit this file manually. Your changes will be ignored.
	More information: https://github.com/MWSE/MWSE/tree/master/docs
-->

The filters used in the Exclusions Page.

## Properties

### `callback`
<div class="search_terms" style="display: none">callback</div>

A custom filter function. The callback function needs to return a string array of items that should appear in the list. To use callback, don't pass the `type` field, just `label` and `callback`.

**Returns**:

* `result` (nil, fun(): string[])

***

### `label`
<div class="search_terms" style="display: none">label</div>

The text shown on a button used to activate this filter on the Exclusions Page.

**Returns**:

* `result` (string)

***

### `noScripted`
<div class="search_terms" style="display: none">noscripted</div>

If set to true, no objects with a script will be added to the list.

**Returns**:

* `result` (boolean, nil)

***

### `objectFilters`
<div class="search_terms" style="display: none">objectfilters</div>

If using "Object" filter, you can pass a dictionary-style table of fields and values that the objects need to satisfy to appear in the list.

**Returns**:

* `result` (table&lt;string, unknown&gt;, nil)

***

### `objectType`
<div class="search_terms" style="display: none">objecttype</div>

If using "Object" filter, pass the object types from [`tes3.objectType`](https://mwse.github.io/MWSE/references/object-types/) enumeration here.

**Returns**:

* `result` (integer, integer[], nil)

***

### `type`
<div class="search_terms" style="display: none">type</div>

The filter type. Available options are:

- "Plugin" - The list will contain the currently loaded plugin list.
- "Object" - This filter will list objects.

Another option is to pass no `type`. Then, you can define custom `callback` function.

**Returns**:

* `result` (string, nil)
