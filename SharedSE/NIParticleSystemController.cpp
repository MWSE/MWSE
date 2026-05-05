#include "NIParticleSystemController.h"

namespace NI {
	nonstd::span<PerParticleData> ParticleSystemController::getPerParticleData() {
		if (particleData == nullptr) { return {}; }
		return { particleData, particleDataCount };
	}
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::ParticleSystemController)
#endif
