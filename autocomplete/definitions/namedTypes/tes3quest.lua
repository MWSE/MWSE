return {
	type = "class",
	description = [[A representation of a journal quest log, with associated dialogue(s), infos and quest flags. Quest tracking was added by Tribunal as a separate system on top of the dialogue system. It had to add tracking to existing quests as well as new ones, but its design is not seamlessly integrated.

Quests have a name, separate from the dialogue id, which is derived from a specially flagged info (the Quest Name flag) in the dialogue infos. Multiple dialogue topics with the same name are combined into one quest. The overall quest state of active or completed is tracked by the quest.]],
	inherits = "tes3baseObject",
}