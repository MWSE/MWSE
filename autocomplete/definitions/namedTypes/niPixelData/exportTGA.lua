return {
	type = "method",
	description = [[Exports this pixel data as a Truevision TGA file. Only supports export of pixel data in RGBA32 format, which all `niPixelData` created using [`niPixelData.new`](https://mwse.github.io/MWSE/types/niPixelData/#new) are.]],
	arguments = {
		{ name = "path", type = "string", description = "The path to write the file at, relative to the Morrowind installation folder. The `.tga` extension needs to be specified manually." },
	},
}