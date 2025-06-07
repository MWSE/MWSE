return {
	type = "function",
	description = "Saves a screenshot.",
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "path", type = "string", description = "The location of the folder to save the screenshot to. Relative to the Morrowind directory. *Needs* to include the file format extension. The supported file formats are `bmp`, `jpeg`, `dds`, `png`, and `tga`." },
			{ name = "captureWithUI", type = "boolean", optional = true, default = false, description = "If set to `true`, the screenshot will include the user interface." },
		},
	}},
}
