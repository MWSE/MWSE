return {
	type = "value",
	description = [[Sets alignment of child elements inside its parent, as a fraction of interior space. Either childAlignX or childAlignY is used depending on the flow direction. childAlignX (horizontal alignment) is used for top-to-bottom flow direction, and childAlignY (vertical alignment) is used for left-to-right flow direction. Previous limitations on using this layout have been resolved.

0.0 = Left/top edge touches left/top edge of parent.
0.5 = Centered.
1.0 = Right/bottom edge touches right/bottom edge of parent.
For negative values, there is a special case behaviour: all children but the last will be left-aligned/top-aligned, the last child will be right-aligned/bottom-aligned.]],
	valuetype = "number",
}