return {
	type = "value",
	description = [[The 32 raw button bytes, indexed from 1 to 32. Bit 0x80 indicates a held button. Button ID 0 in events and helpers corresponds to buttons[1]. Entries are writable.]],
	readOnly = true,
	valuetype = "number[]",
}
