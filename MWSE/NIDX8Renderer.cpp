#include "NIDX8Renderer.h"

namespace NI {
	DX8AdapterDesc* DX8Renderer::getCurrentAdapter() const {
		if (systemDesc == nullptr) {
			return nullptr;
		}
		return systemDesc->adapters[currentAdapterIndex];
	}
}
