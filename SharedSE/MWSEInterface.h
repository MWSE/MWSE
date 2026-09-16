#pragma once

namespace mwse {
	static const int supported_api_version = 1;

	struct MWSEAPI {
		virtual int getAPIVersion() const = 0;
	};

	struct MWSEAPIv1 : public MWSEAPI {
		virtual int getAPIVersion() const override;

		virtual int getWeatherCurrent() const;
		virtual int getWeatherNext() const;
		virtual float getWeatherLerp() const;
		virtual bool getWeatherExists(int weatherID) const;
		virtual bool getWeatherHasFog(int weatherID) const;
		virtual float getWeatherRippleFactor(int weatherID) const;
	};

	MWSEAPI* getInterface(int version);
}
