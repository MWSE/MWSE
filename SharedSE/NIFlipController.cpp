#include "NIFlipController.h"

namespace NI {
	Texture* FlipController::getTextureAtIndex(size_t index) const {
		return _getTextureAtIndex(this, index);
	}

	void FlipController::setTexture(Texture* texture, size_t index) {
		_setTexture(this, texture, index);
	}

	void FlipController::updateTimings() {
		_updateTimings(this);
	}

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
}
