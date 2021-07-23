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
		
		// Copy textures.
		for (size_t i = 0; i < textures.endIndex; i++) {
			to->setTexture(getTextureAtIndex(i), i);
		}

		// Copy fields.
		to->currentIndex = currentIndex;
		to->affectedMap = affectedMap;
		to->flipStartTime = flipStartTime;
		to->duration = duration;
		to->secondsPerFrame = secondsPerFrame;

		// Recalculate duration and high key frame.
		// This is needed because setTexture invalidates these fields, expecting to be manually recalculated later.
		to->updateTimings();
	}
}
