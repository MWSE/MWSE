#include "ArrayUtil.h"

#include "Log.h"

namespace mwse {
	Arrays Arrays::singleton;

	Arrays::Arrays() {
		arrays = Arrays_t();
	}

	size_t Arrays::create(std::string_view caller) {
		size_t id = 0;
		if (arrays.size() < maxArrayId) {
			id = arrays.size() + 1;
			arrays.push_back(ContainedArray_t());
		}
		else {
			mwse::log::getLog() << caller << ": Unable to create array. Maximum number of arrays reached. id: " << id << std::endl;
		}
		return id;
	}

	ArrayItem_t Arrays::getValue(std::string_view caller, size_t const id, size_t const index) {
		ArrayItem_t value = 0;
		if (id > 0 && id <= arrays.size()) {
			ContainedArray_t const& a = arrays[id - 1];
			if (index >= 0 && index < a.size()) {
				value = a[index];
			}
			else {
				mwse::log::getLog() << caller << ": Array index out of bounds. id: " << id << " index: " << index << std::endl;
			}
		}
		else {
			mwse::log::getLog() << caller << ": Invalid array id: " << id << std::endl;
		}
		return value;
	}

	long Arrays::setValue(std::string_view caller, size_t const id, size_t const index, ArrayItem_t const value) {
		long success = 0;
		if (id > 0 && id <= arrays.size()) {
			if (index >= 0) {
				ContainedArray_t& a = arrays[id - 1];
				if (index + 1 > a.size()) {
					a.resize(index + 1);
				}
				a[index] = value;
				success = 1;
			}
			else {
				mwse::log::getLog() << caller << ": Array index out of bounds. id: " << id << " index: " << index << std::endl;
			}
		}
		else {
			mwse::log::getLog() << caller << ": Invalid array id: " << id << std::endl;
		}
		return success;
	}

	size_t Arrays::getSize(std::string_view caller, size_t const id)
	{
		size_t size = 0;
		if (id > 0 && id <= arrays.size()) {
			size = arrays[id - 1].size();
		}
		else {
			mwse::log::getLog() << caller << ": Invalid array id: " << id << std::endl;
		}
		return size;
	}

	long Arrays::clear(std::string_view caller, size_t const id)
	{
		long success = 0;
		if (id > 0 && id <= arrays.size()) {
			arrays[id - 1].clear();
			success = 1;
		}
		else {
			mwse::log::getLog() << caller << ": Invalid array id: " << id << std::endl;
		}
		return success;
	}

	Arrays_t& Arrays::get() {
		return arrays;
	}

	ContainedArray_t& Arrays::get(size_t index) {
		return arrays[index - 1];
	}
}
