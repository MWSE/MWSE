local ffi = require("ffi")

ffi.cdef[[
	typedef struct {
		float r;
		float g;
		float b;
	} RGB;

	typedef struct {
		float h;
		float s;
		float v;
	} HSV;
	HSV sRGBtoHSV(RGB rgb);
	RGB HSVtosRGB(HSV c);
	void generateMainPicker(const double hue, RGB *data, const uint32_t width, const uint32_t height);
	void generateSaturationPicker(const double hue, const double value, RGB *data, const uint32_t width, const uint32_t height);
	void generatePreviewImage(const RGB color, const double alpha, RGB *destination, double *alphas, RGB *checkers, const int32_t width, const uint32_t height);
]]
local ColorUtils = ffi.load(".\\Data Files\\MWSE\\core\\mwse\\ui\\tes3uiElement\\createColorPicker\\colorUtils\\ColorUtils")

--- @class ffiImagePixel : ffi.cdata*
--- @field r number Red in range [0, 1].
--- @field g number Green in range [0, 1].
--- @field b number Blue in range [0, 1].

--- @class ffiHSV : ffi.cdata*
--- @field h number Hue in range [0, 360)
--- @field s number Saturation in range [0, 1]
--- @field v number Value/brightness in range [0, 1]


local this = {}

--- Converts from sRGB to HSV using Oklab color space.
--- @type fun(color: ffiImagePixel): ffiHSV
this.sRGBtoHSV = ColorUtils.sRGBtoHSV

--- Converts from HSV to sRGB using Oklab color space.
--- @type fun(hsv: ffiHSV): ffiImagePixel
this.HSVtosRGB = ColorUtils.HSVtosRGB

--- Generates an image for use in the main picker for given hue.
--- The image is a plot of saturation along X and value along Y.
--- @param hue number
--- @param image Image
function this.generateMainPicker(hue, image)
	ColorUtils.generateMainPicker(hue, image.data, image.width, image.height)
end

--- Generates an image for use in saturation picker for given hue and value.
--- @param hue number Hue in range [0, 360)
--- @param value number Value in range [0, 1]
--- @param image Image
function this.generateSaturationPicker(hue, value, image)
	ColorUtils.generateSaturationPicker(hue, value, image.data, image.width, image.height);
end

--- Generates an image for ColorPreview widget. Essentially a "over" alpha compositing operation
--- where checkers is assumed to be fully opaque and all the pixels of the foreground have given
--- color and alpha values.
--- @param color ffiImagePixel
--- @param alpha number
--- @param destination Image
--- @param checkers Image
function this.generatePreviewImage(color, alpha, destination, checkers)
	ColorUtils.generatePreviewImage(
		color, alpha,
		destination.data, destination.alphas, checkers.data,
		destination.width, destination.height
	)
end

return this
