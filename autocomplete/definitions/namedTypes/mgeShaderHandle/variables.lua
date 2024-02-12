return {
	type = "value",
	description = [[Lists the shader's editable variables. The result table has the variable names as keys, and the variable types as values. To get and set the actual variable, use the expression `shaderHandle.<variableName>`.

Variable types are:

- 'b' boolean
- 'i' integer
- 'f' float
- 's' string
- 'B' boolean array
- 'I' integer array
- 'F' float array (was 'a' for MGE v0.16.x and earlier)
- '2' vec2
- '3' vec3
- '4' vec4
- 'V' vec4 array
- 'm' matrix]],
	valuetype = "table<string, string>",
}