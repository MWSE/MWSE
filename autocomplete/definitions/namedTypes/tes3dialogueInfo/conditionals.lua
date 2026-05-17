return {
	type = "value",
	description = [[The dialogue conditional filters attached to this info. Each entry is a table with a 1-based `index`, `type` (`tes3.dialogueConditionalType`), `operator` (`tes3.dialogueConditionalComparator`), numeric `value`, optional `constantType` (`tes3.dialogueConditionalConstantType`), and one of `functionId` (`tes3.dialogueConditionalFunction`), `object`, or `variable` depending on the conditional kind.]],
	readOnly = true,
	valuetype = "table<integer, table>",
}
