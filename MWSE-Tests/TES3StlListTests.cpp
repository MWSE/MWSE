#include "pch.h"
#include "CppUnitTest.h"

#define MWSE_NO_CUSTOM_ALLOC 1
#include "..\MWSE\TES3StlList.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CustomContainers {
	TEST_CLASS(StlList) {
public:

	//
	// Insertion tests
	//

	TEST_METHOD(InsertAtIterator) {
		TES3::StlList<int> container;
		for (auto i = 0; i < 10; i++) {
			container.push_back(rand());
		}

		container.insert(container.begin(), 2);
		Assert::AreEqual(2, container.front());

		container.insert(container.end(), 4);
		Assert::AreEqual(4, container.back());

		container.insert(++container.begin(), 6);
		Assert::AreEqual(6, container[1]);
	}

	TEST_METHOD(InsertAtPosition) {
		TES3::StlList<int> container;
		for (auto i = 0; i < 10; i++) {
			container.push_front(rand());
		}

		container.insert(0, 2);
		Assert::AreEqual(2, container.front());

		container.insert(1, 2);
		Assert::AreEqual(2, *(container.begin() + 1));

		container.insert(container.size(), 4);
		Assert::AreEqual(4, container.back());

		container.insert(1, 6);
		Assert::AreEqual(6, container[1]);
	}

	TEST_METHOD(PushFront) {
		TES3::StlList<int> container;
		for (auto i = 0; i < 10; i++) {
			container.push_front(rand());
		}

		container.push_front(9001);
		Assert::AreEqual(9001, container.front());
	}

	TEST_METHOD(PushBack) {
		TES3::StlList<int> container;
		container.push_back(42);
		Assert::AreEqual(42, container.back());

		for (auto i = 0; i < 10; i++) {
			container.push_front(rand());
		}

		container.push_back(9001);
		Assert::AreEqual(9001, container.back());
	}

	//
	// Erasure tests
	//

	TEST_METHOD(EraseAtIterator) {
		TES3::StlList<int> container;
		for (auto i = 0; i < 10; i++) {
			container.push_front(rand() % 1000);
		}
		const int sumBefore = std::accumulate(container.begin(), container.end(), 0);

		container.insert(container.begin() + 4, 999);
		container.erase(container.begin() + 4);

		const int sumAfter = std::accumulate(container.begin(), container.end(), 0);
		Assert::AreEqual(sumBefore, sumAfter);
	}

	TEST_METHOD(EraseAtPosition) {
		TES3::StlList<int> container;
		for (auto i = 0; i < 10; i++) {
			container.push_front(rand() % 1000);
		}
		const int sumBefore = std::accumulate(container.begin(), container.end(), 0);

		container.insert(4, 999);
		container.erase(4);

		const int sumAfter = std::accumulate(container.begin(), container.end(), 0);
		Assert::AreEqual(sumBefore, sumAfter);
	}

	//
	// Other access tests
	//

	TEST_METHOD(StandardForLoop) {
		TES3::StlList<int> container;
		for (auto i = 0; i < 10; i++) {
			container.push_front(3);
		}

		size_t elements = 0;
		size_t sum = 0;
		for (auto& value : container) {
			elements++;
			sum += value;
		}

		Assert::AreEqual(container.size(), elements);
		Assert::AreEqual(container.size() * 3, sum);
	}

	TEST_METHOD(ReverseForLoop) {
		TES3::StlList<int> container;
		for (auto i = 0; i < 10; i++) {
			container.push_front(rand() % 1000);
		}

		size_t elements = 0;
		for (auto it = container.rbegin(); it != container.rend(); ++it) {
			elements++;
		}

		Assert::AreEqual(container.size(), elements);
	}

	TEST_METHOD(Accumulate) {
		TES3::StlList<int> container;
		for (auto i = 0; i < 10; i++) {
			container.push_front(5);
		}

		unsigned int sum = std::accumulate(container.begin(), container.end(), 0);
		Assert::AreEqual(container.size() * 5, sum);
	}

	TEST_METHOD(AccumulateEmpty) {
		TES3::StlList<int> container;
		unsigned int sum = std::accumulate(container.begin(), container.end(), 0);
		Assert::AreEqual(0U, sum);
	}

	TEST_METHOD(AccumulateConstant) {
		TES3::StlList<int> container;
		for (auto i = 0; i < 10; i++) {
			container.push_front(5);
		}

		unsigned int sum = std::accumulate(container.cbegin(), container.cend(), 0);
		Assert::AreEqual(container.size() * 5, sum);
	}

	TEST_METHOD(AccumulateReversed) {
		TES3::StlList<int> container;
		for (auto i = 0; i < 10; i++) {
			container.push_front(5);
		}

		unsigned int sum = std::accumulate(container.rbegin(), container.rend(), 0);
		Assert::AreEqual(container.size() * 5, sum);
	}

	/*
	// TODO: Figure out why this fails to compile.
	TEST_METHOD(AccumulateConstantReversed) {
		TES3::StlList<int> container;
		for (auto i = 0; i < 10; i++) {
			container.push_front(5);
		}

		// Causes compiler errors that need to be figured out...
		//unsigned int sum = std::accumulate(container.crbegin(), container.crend(), 0);
		//Assert::AreEqual(container.size() * 5, sum);
		Assert::Fail(L"Compiler says no.");
	}
	*/

	//
	// Lua test functions.
	//

	static void lua_log(sol::this_state ts, sol::object object) {
		sol::state_view state = ts;
		std::string result = state["tostring"](object);
		result += "\n";
		Logger::WriteMessage(result.c_str());
	}

	TEST_METHOD(LuaLength) {
		TES3::StlList<int> container;
		for (auto i = 0; i < 10; i++) {
			container.push_back(rand());
		}

		sol::state lua;
		lua.open_libraries();

		try {
			lua["container"] = &container;
			size_t length = lua.safe_script("return #container");
			Assert::AreEqual(container.size(), length);
		}
		catch (std::exception& e) {
			std::wstringstream ss;
			ss << "Lua exception: " << e.what();
			Assert::Fail(ss.str().c_str());
		}
	}

	TEST_METHOD(LuaIPairs) {
		TES3::StlList<int> container;
		for (auto i = 0; i < 3; i++) {
			container.push_back(5);
		}

		sol::state lua;
		lua.open_libraries();

		try {
			lua["container"] = &container;
			size_t sum = lua.safe_script(R"(
				local sum = 0
				for _, value in ipairs(container) do
					sum = sum + value
				end
				return sum
				)");
			Assert::AreEqual(container.size() * 5, sum);
		}
		catch (std::exception& e) {
			std::wstringstream ss;
			ss << "Lua exception: " << e.what();
			Assert::Fail(ss.str().c_str());
		}
	}

	TEST_METHOD(LuaPairs) {
		TES3::StlList<int> container;
		for (auto i = 0; i < 3; i++) {
			container.push_back(5);
		}

		sol::state lua;
		lua.open_libraries();
		lua["print"] = &lua_log;
		lua["container"] = &container;

		try {
			size_t sum = lua.safe_script(R"(
				local sum = 0
				for _, value in pairs(container) do
					sum = sum + value
					print(string.format("Value: %d; Sum: %d", value, sum))
				end
				return sum
				)");
			Assert::AreEqual(container.size() * 5, sum);
		}
		catch (std::exception& e) {
			std::wstringstream ss;
			ss << "Lua exception: " << e.what();
			Assert::Fail(ss.str().c_str());
		}
	}

	};
}
