/************************************************************************

	ArrayUtil.h - Copyright (c) 2008 The MWSE Project
	https://github.com/MWSE/MWSE/

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

**************************************************************************/

#pragma once

namespace mwse {
	typedef long ArrayItem_t;
	typedef std::vector<ArrayItem_t> ContainedArray_t;
	typedef std::vector<ContainedArray_t> Arrays_t;

	class Arrays {
	public:
		static Arrays &getInstance() { return singleton; };

		size_t create(std::string const& caller);

		ArrayItem_t getValue(std::string const& caller, size_t const id, size_t const index);

		long setValue(std::string const& caller, size_t const id, size_t const index, ArrayItem_t const value);

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
