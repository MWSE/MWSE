#include "NIFlipController.h"

#include "ExceptionUtil.h"

namespace NI {
#if defined(SE_NI_FLIPCONTROLLER_FNADDR_GETTEXTUREATINDEX) && SE_NI_FLIPCONTROLLER_FNADDR_GETTEXTUREATINDEX > 0
	Texture* FlipController::getTextureAtIndex(size_t index) const {
		return _getTextureAtIndex(this, index);
	}
#endif

#if defined(SE_NI_FLIPCONTROLLER_FNADDR_SETTEXTURE) && SE_NI_FLIPCONTROLLER_FNADDR_SETTEXTURE > 0
	void FlipController::setTexture(Texture* texture, size_t index) {
		_setTexture(this, texture, index);
	}
#endif

#if defined(SE_NI_FLIPCONTROLLER_FNADDR_UPDATETIMINGS) && SE_NI_FLIPCONTROLLER_FNADDR_UPDATETIMINGS > 0
	void FlipController::updateTimings() {
		_updateTimings(this);
	}
#endif

#if defined(SE_NI_FLIPCONTROLLER_FNADDR_COPY) && SE_NI_FLIPCONTROLLER_FNADDR_COPY > 0
	void FlipController::copy(FlipController* to) const {
		TimeController::_copy(this, to);

		for (size_t i = 0; i < textures.endIndex; ++i) {
			to->setTexture(getTextureAtIndex(i), i);
		}

		to->currentIndex = currentIndex;
		to->affectedMap = affectedMap;
		to->flipStartTime = flipStartTime;
		to->duration = duration;
		to->secondsPerFrame = secondsPerFrame;

		to->updateTimings();
	}
#endif
}
