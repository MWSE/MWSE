return {
	type = "value",
	description = [[Reads the contents of the rendered texture into a system memory niPixelData. This requires that the rendered texture is not the current render target -- make sure niRenderer:setRenderTarget(nil) is called before using this function. The texture dimensions of the niRenderedTexture and niPixelData must be the same. It returns true if the readback succeeded, false otherwise. This function will also change the pixel data's pixel format to match the d3d surface.
	
This function requires the GPU to finish rendering and send data to the CPU. This can impact framerates significantly, so measure and consider performance carefully when using this.]],
	arguments = {
		{ name = "pixelData", type = "niPixelData", description = "The pixel data to copy the texture into." },
	},
	valuetype = "boolean",
}