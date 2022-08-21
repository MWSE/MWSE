return {
	type = "value",
	description = [[Morrowind incorrectly handles the affected map when cloning NiFlipController objects, preventing mods from using the controller for anything but the base map. MWSE fixes this issue. However, some mods contain bugged assets that have the incorrect affected map assigned, relying on the bug to reassign the controller back to the base map. Disabling this fix will provide support for these mods, but will also prevent newer mods from taking advantage of the fixed controller. It is recommended that you leave this enabled unless you know you need the compatibility.]],
	valuetype = "boolean",
	default = true,
}
