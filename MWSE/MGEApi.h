#pragma once

namespace mge {
	static const int supported_api_version = 3;

	struct MGEAPI {
		virtual int getAPIVersion() const = 0;
		virtual int getMGEVersion() const = 0;
	};

	struct DistantLandRenderConfig {
		// API v1
		float DrawDist;
		float NearStaticEnd;
		float FarStaticEnd;
		float VeryFarStaticEnd;
		float FarStaticMinSize;
		float VeryFarStaticMinSize;
		float AboveWaterFogStart;
		float AboveWaterFogEnd;
		float BelowWaterFogStart;
		float BelowWaterFogEnd;
		float InteriorFogStart;
		float InteriorFogEnd;
		unsigned char WaterWaveHeight;
		unsigned char WaterCaustics;
		unsigned int ShadowResolution;
		float Wind[10];
		float FogDist[10];
		float FogOffset[10];

		// Custom wrapper functions.
		std::reference_wrapper<float[10]> getWind() { return std::ref(Wind); }
		std::reference_wrapper<float[10]> getFogDist() { return std::ref(FogDist); }
		std::reference_wrapper<float[10]> getFogOffset() { return std::ref(FogOffset); }
	};

	enum struct LightingMode {
		// API v1
		Vertex = 0,
		PerPixelAll = 1,
		PerPixelInteriorOnly = 2,
	};

	typedef void* ShaderHandle;

	struct ShaderVariableInfo {
		char valueType;
		char name[128];
	};

	struct MacroFunctions {
		typedef void (*Macro)();

		// API v1
		Macro TakeScreenshot;
		Macro ToggleStatusText;
		Macro ToggleFpsCounter;
		Macro ShowLastMessage;
		Macro ToggleBlending;
		Macro ToggleShaders;
		Macro ToggleDistantLand;
		Macro ToggleShadows;
		Macro ToggleGrass;
		Macro ToggleLightingMode;
		Macro ToggleTransparencyAA;
		Macro IncreaseViewRange;
		Macro DecreaseViewRange;
		Macro ToggleZoom;
		Macro ResetEnableZoom;
		Macro IncreaseZoom;
		Macro DecreaseZoom;
		Macro ToggleCrosshair;
		Macro NextTrack;
		Macro DisableMusic;
		Macro IncreaseFOV;
		Macro DecreaseFOV;
		Macro HaggleMore1;
		Macro HaggleMore10;
		Macro HaggleMore100;
		Macro HaggleMore1000;
		Macro HaggleMore10000;
		Macro HaggleLess1;
		Macro HaggleLess10;
		Macro HaggleLess100;
		Macro HaggleLess1000;
		Macro HaggleLess10000;
		Macro MoveForward3PCam;
		Macro MoveBack3PCam;
		Macro MoveLeft3PCam;
		Macro MoveRight3PCam;
		Macro MoveDown3PCam;
		Macro MoveUp3PCam;
	};

	enum struct RenderFeature {
		// API v1
		FPSCounter = 0,
		DisplayMessages = 1,
		PauseRenderingInMenus = 2,
		NoMWMGEBlending = 3,
		NoMWSunglare = 4,
		Shaders = 5,
		TransparencyAA = 6,
		UpdateHDR = 7,
		ExponentialFog = 8,
		AtmosphericScattering = 9,
		Grass = 10,
		Shadows = 11,
		DistantWater = 12,
		DistantLand = 13,
		DistantStatics = 14,
		ReflectiveWater = 15,
		ReflectNearStatics = 16,
		ReflectInterior = 17,
		ReflectSky = 18,
		BlurReflections = 19,
		DynamicRipples = 20,
		CrosshairAutoHide = 21,
	};

	struct MGEAPIv1 : public MGEAPI {
		virtual int getAPIVersion() const override;
		virtual int getMGEVersion() const override;

		virtual bool loadConfig();
		virtual bool saveConfig();

		virtual bool featureGetEnabled(RenderFeature feature);
		virtual void featureSetEnabled(RenderFeature feature, bool enable);

		virtual float guiGetScale();
		virtual void guiSetScale(float scale);

		virtual const MacroFunctions* macroFunctions();
		virtual bool reloadDistantLand();
		virtual DistantLandRenderConfig* getDistantLandRenderConfig();
		virtual LightingMode lightingModeGet();
		virtual void lightingModeSet(LightingMode mode);

		virtual float cameraGetFoV();
		virtual void cameraSetFoV(float fov);
		virtual void cameraThirdPersonGetOffset(float* out_values);
		virtual void cameraThirdPersonSetOffset(float* values);

		virtual bool zoomGetEnabled();
		virtual void zoomSetEnabled(bool enable);
		virtual void zoomIn(float amount);
		virtual void zoomOut(float amount);
		virtual float zoomGetZoom() const;
		virtual void zoomSetZoom(float amount);
		virtual void zoomSetZoomContinuous(float rate, float rateTarget);
		virtual void zoomStop() const;

		virtual bool cameraShakeGetEnabled();
		virtual void cameraShakeSetEnabled(bool enable);
		virtual float cameraShakeGetAcceleration();
		virtual void cameraShakeSetAcceleration(float value);
		virtual float cameraShakeGetMagnitude();
		virtual void cameraShakeSetMagnitude(float value);

		virtual void hudReset();
		virtual bool hudLoad(const char* name, const char* texture);
		virtual void hudFree(const char* name);
		virtual bool hudGetEnabled(const char* name);
		virtual void hudSetEnabled(const char* name, bool enable);
		virtual void hudPosition(const char* name, float x, float y);
		virtual void hudScale(const char* name, float scaleX, float scaleY);
		virtual void hudFullscreen(const char* name);
		virtual void hudSetEffect(const char* name, const char* effectPath);
		virtual void hudSetFloat(const char* name, const char* variableName, float value);
		virtual void hudSetInt(const char* name, const char* variableName, int value);
		virtual void hudSetVector4(const char* name, const char* variableName, float value[4]);
		virtual void hudSetTexture(const char* name, const char* texturePath);

		virtual float shaderGetHDRReactionSpeed();
		virtual void shaderSetHDRReactionSpeed(float speed);
		virtual ShaderHandle shaderLoad(const char* id);
		virtual bool shaderReload(ShaderHandle handle);
		virtual ShaderHandle shaderGetShader(const char* id);
		virtual ShaderHandle shaderListShaders(size_t index);
		virtual const char* shaderGetName(ShaderHandle handle);
		virtual bool shaderGetEnabled(ShaderHandle handle);
		virtual void shaderSetEnabled(ShaderHandle handle, bool enable);
		virtual int shaderGetPriority(ShaderHandle handle);
		virtual void shaderSetPriority(ShaderHandle handle, int value);
		virtual bool shaderGetVariableInfo(ShaderHandle handle, size_t index, ShaderVariableInfo* out_info);
		virtual bool shaderGetBool(ShaderHandle handle, const char* variableName, bool* out_value);
		virtual bool shaderGetFloat(ShaderHandle handle, const char* variableName, float* out_value);
		virtual bool shaderGetInt(ShaderHandle handle, const char* variableName, int* out_value);
		virtual bool shaderGetString(ShaderHandle handle, const char* variableName, const char** out_value);
		virtual bool shaderGetFloatArray(ShaderHandle handle, const char* variableName, float* out_values, size_t* count);
		virtual bool shaderGetVector(ShaderHandle handle, const char* variableName, float* out_values, size_t count);
		virtual bool shaderGetMatrix(ShaderHandle handle, const char* variableName, float* out_values);
		virtual bool shaderSetBool(ShaderHandle handle, const char* variableName, bool value);
		virtual bool shaderSetFloat(ShaderHandle handle, const char* variableName, float value);
		virtual bool shaderSetInt(ShaderHandle handle, const char* variableName, int value);
		virtual bool shaderSetString(ShaderHandle handle, const char* variableName, const char* value);
		virtual bool shaderSetFloatArray(ShaderHandle handle, const char* variableName, const float* values, size_t* count);
		virtual bool shaderSetVector(ShaderHandle handle, const char* variableName, const float* values, size_t count);
		virtual bool shaderSetMatrix(ShaderHandle handle, const char* variableName, const float* values);

		virtual void weatherScatteringGet(float* inscatter, float* outscatter);
		virtual void weatherScatteringSet(float inscatter[3], float outscatter[3]);
		virtual void weatherDistantFogGet(int weatherID, float* fogDistMult, float* fogOffset);
		virtual void weatherDistantFogSet(int weatherID, float fogDistMult, float fogOffset);
		virtual void weatherPerPixelLightGet(int weatherID, float* sunMult, float* ambMult);
		virtual void weatherPerPixelLightSet(int weatherID, float sunMult, float ambMult);
	};

	struct MGEAPIv2 : public MGEAPIv1 {
		virtual void saveScreenshot(const char* path, bool captureWithUI);

		virtual void weatherScatteringSkylightGet(float* skylight);
		virtual void weatherScatteringSkylightSet(float skylight[3]);
	};

	struct MGEAPIv3 : public MGEAPIv2 {
		virtual float nearRenderDistanceGet();
		virtual void nearRenderDistanceSet(float distance);

		virtual bool shaderGetBoolArray(ShaderHandle handle, const char* variableName, int* values, size_t* count);
		virtual bool shaderGetIntArray(ShaderHandle handle, const char* variableName, int* values, size_t* count);
		virtual bool shaderGetVectorArray(ShaderHandle handle, const char* variableName, float* values, size_t* count);

		virtual bool shaderSetBoolArray(ShaderHandle handle, const char* variableName, const int* values, size_t* count);
		virtual bool shaderSetIntArray(ShaderHandle handle, const char* variableName, const int* values, size_t* count);
		virtual bool shaderSetVectorArray(ShaderHandle handle, const char* variableName, const float* values, size_t* count);
	};

	inline MGEAPIv1* api = nullptr;
	inline int apiVersion = 0;
	inline const MacroFunctions* macros = nullptr;

	struct VersionStruct {
		BYTE major;
		BYTE minor;
		BYTE patch;
		BYTE build;

		inline bool valid() const {
			return major > 0 || minor > 0;
		}
	};
	extern VersionStruct guiVersion;
}
