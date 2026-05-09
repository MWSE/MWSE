#include "NICopyTransformController.h"

#include "NIAVObject.h"
#include "NIBinaryStream.h"
#include "NIRTTI.h"
#include "NIStream.h"

namespace NI {
	CopyTransformController::CopyTransformController(AVObject* from) {
		TimeController::ctor();
		vTable.asController = &CopyTransformController_vTable::Instance;
		copyFrom = from;
	}

	CopyTransformController::~CopyTransformController() {
		copyFrom = nullptr;
		TimeController::dtor();
	}

	void CopyTransformController::destructor(bool deleting) {
		CopyTransformController::~CopyTransformController();
		if (deleting) {
			delete this;
		}
	}

	RTTI* CopyTransformController::getRunTimeTypeInformation() const {
		return &CopyTransformController_RTTI;
	}

	Object* CopyTransformController::createClone() const {
		auto copy = new CopyTransformController();
		TimeController::_copy(this, copy);
		copy->copyFrom = static_cast<AVObject*>(Stream::getCopiedObject(copyFrom));

		return copy;
	}

	bool CopyTransformController::registerStreamables(Stream* stream) {
		if (!TimeController::_registerStreamables(this, stream)) {
			return false;
		}

		if (copyFrom) {
			if (!copyFrom->registerStreamables(stream)) {
				return false;
			}
		}

		return true;
	}

	void CopyTransformController::linkObject(Stream* stream) {
		TimeController::_linkObject(this, stream);
		copyFrom = static_cast<AVObject*>(stream->getLinkObject(stream->currentlyLoading->linkId));
	}

	void CopyTransformController::loadBinary(Stream* stream) {
		TimeController::_loadBinary(this, stream);
		stream->inStream->read(&stream->currentlyLoading->linkId);
	}

	void CopyTransformController::saveBinary(Stream* stream) {
		TimeController::_saveBinary(this, stream);
		stream->writeLinkId(copyFrom);
	}

	bool CopyTransformController::isEqual(const CopyTransformController* other) const {
		if (!TimeController::_isEqual(this, other)) {
			return false;
		}

		if (copyFrom && other->copyFrom) {
			return copyFrom->isEqual(other->copyFrom);
		}

		return copyFrom == nullptr && other->copyFrom == nullptr;
	}

	void CopyTransformController::update(float fTime) {
		if (!target || !copyFrom) {
			return;
		}

		auto validTarget = static_cast<AVObject*>(target);
		validTarget->localTranslate = copyFrom->localTranslate;
		validTarget->setLocalRotationMatrix(copyFrom->localRotation);
		validTarget->localScale = copyFrom->localScale;
	}

	bool CopyTransformController::targetIsRequiredType() const {
		if (target == nullptr) {
			return false;
		}

		return target->isInstanceOfType(RTTIStaticPtr::NiAVObject);
	}

	Object* CopyTransformController::loader(Stream* stream) {
		auto object = new CopyTransformController();

		auto loader = new Stream::LoadingObject();

		stream->loadingObjects.add(loader);
		stream->currentlyLoading = loader;

		object->loadBinary(stream);

		return object;
	}

	RTTI CopyTransformController::CopyTransformController_RTTI(CopyTransformController_Name, RTTIStaticPtr::NiTimeController);

	template <typename T, typename F>
	constexpr inline T ConvertThisCall(F&& value) {
		static_assert(sizeof(T) == sizeof(F), "Types must be of the same size.");
		return *reinterpret_cast<T*>(&value);
	}

	CopyTransformController_vTable::CopyTransformController_vTable() {
		destructor = ConvertThisCall<decltype(destructor)>(&CopyTransformController::destructor);
		getRTTI = ConvertThisCall<decltype(getRTTI)>(&CopyTransformController::getRunTimeTypeInformation);
		createClone = ConvertThisCall<decltype(createClone)>(&CopyTransformController::createClone);
		registerStreamables = ConvertThisCall<decltype(registerStreamables)>(&CopyTransformController::registerStreamables);
		linkObject = ConvertThisCall<decltype(linkObject)>(&CopyTransformController::linkObject);
		loadBinary = ConvertThisCall<decltype(loadBinary)>(&CopyTransformController::loadBinary);
		saveBinary = ConvertThisCall<decltype(saveBinary)>(&CopyTransformController::saveBinary);
		isEqual = ConvertThisCall<decltype(isEqual)>(&CopyTransformController::isEqual);
		targetIsRequiredType = ConvertThisCall<decltype(targetIsRequiredType)>(&CopyTransformController::getRunTimeTypeInformation);
		update = ConvertThisCall<decltype(update)>(&CopyTransformController::update);
	}

	CopyTransformController_vTable CopyTransformController_vTable::Instance;
}
