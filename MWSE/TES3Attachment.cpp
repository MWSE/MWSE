#include "TES3Attachment.h"

#include "TES3Misc.h"

namespace TES3 {
	Misc* LockAttachmentNode::getKey() {
		return key;
	}

	void LockAttachmentNode::setKey(Misc* k) {
		if (k && !(k->flags & 1)) {
			throw std::invalid_argument("Invalid key specified. Object is not a key.");
		}

		key = k;
	}
}
