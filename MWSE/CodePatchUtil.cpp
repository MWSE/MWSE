#include "CodePatchUtil.h"

#include "Log.h"

#define DEBUG_MCP_FEATURESET false

namespace mwse {
	namespace mcp {
		FeatureStoreMap_t featureStore;

		bool loadFeatureList() {
			// Open file.
			HANDLE fileHandle = CreateFileA(
				"mcpatch\\installed",
				GENERIC_READ,
				FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
				NULL
			);

			// Values start at 0x3C.
			if (fileHandle != INVALID_HANDLE_VALUE && SetFilePointer(fileHandle, 0x8, 0, FILE_BEGIN) != 0x8) {
				CloseHandle(fileHandle);
				return false;
			}

			// Make sure we opened the file.
			if (fileHandle == INVALID_HANDLE_VALUE) {
				return false;
			}

			// Values are stored as 4-byte integers.
			long value = 0;
			DWORD bytesRead = 0;
			while (true) {
				// Read four bytes.
				ReadFile(fileHandle, &value, sizeof(long), &bytesRead, 0);

				// If we couldn't read, we're at the EOF and are done.
				if (bytesRead != sizeof(long)) {
					break;
				}

				// If the value we got back was negative, the feature is disabled.
				long id = std::abs(value);
				if (value > 0) {
#if DEBUG_MCP_FEATURESET
					mwse::log::getLog() << __FUNCTION__ << ": Feature #" << id << " is enabled." << std::endl;
#endif
					featureStore[id] = true;
				}
				else {
#if DEBUG_MCP_FEATURESET
					mwse::log::getLog() << __FUNCTION__ << ": Feature #" << id << " is disabled." << std::endl;
#endif
					featureStore[id] = false;
				}
			}

			// Close the file handle.
			CloseHandle(fileHandle);

			return !featureStore.empty();
		}

		bool getFeatureEnabled(long id) {
			FeatureStoreMap_t::iterator i = featureStore.find(id);
			if (i != featureStore.end()) {
				return i->second;
			}
			
			return false;
		}

		bool hasFeaturesFound() {
			return !featureStore.empty();
		}
	}
}