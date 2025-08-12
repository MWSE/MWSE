#include "TES3Attachment.h"

#include "TES3Misc.h"

namespace TES3 {

	//
	// LightAttachment
	//

	NI::Light* LightAttachmentNode::getLight() const {
		return light;
	}

	void LightAttachmentNode::setLight(NI::Light* newLight) {
		if (newLight == nullptr) {
			throw std::invalid_argument("A light node cannot be set to nullptr. Remove the whole dynamic light attachment to remove the dynamic light.");
		}

		light = newLight;
	}

	//
	// LockAttachment
	//

	Misc* LockAttachmentNode::getKey() const {
		return key;
	}

	void LockAttachmentNode::setKey(Misc* k) {
		if (k && !k->getIsKey()) {
			throw std::invalid_argument("Invalid key specified. Object is not a key.");
		}

		key = k;
	}
}
