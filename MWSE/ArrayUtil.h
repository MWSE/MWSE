#pragma once

namespace mwse {
	typedef long ArrayItem_t;
	typedef std::vector<ArrayItem_t> ContainedArray_t;
	typedef std::vector<ContainedArray_t> Arrays_t;

	class Arrays {
	public:
		static Arrays& getInstance() { return singleton; };

		size_t create(std::string const& caller);

		ArrayItem_t getValue(std::string const& caller, size_t const id, size_t const index);

		short setValue(std::string const& caller, size_t const id, size_t const index, ArrayItem_t const value);

		size_t getSize(std::string const& caller, size_t const id);

		long clear(std::string const& caller, size_t const id);

		Arrays_t& get();

		ContainedArray_t& get(size_t index);

	private:
		Arrays();

		static Arrays singleton;

		static size_t const maxArrayId = 16777215; // max 24 bit int - avoid exceding MW global precision

		Arrays_t arrays;
	};
};
