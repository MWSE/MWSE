return {
	type = "value",
	description = [[A generic lua table that data can be written to, and synced to/from the save. All information stored must be valid for serialization to json. For item references, this is the same table as on the tes3itemData structure. To store data that doesn't get serialized to/from the save, use tempData.]],
	valuetype = "table",
}