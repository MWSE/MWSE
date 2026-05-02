return {
	type = "value",
	description = [[This field is used to detect cyclic references. It is created temporarily when absent. Every table that is currently processed is used as key, the value is true.

!!! info
	This field is used internally by the implementation and is usually not of interest to the end user.]],
	valuetype = "table<table, true>|nil",
}
