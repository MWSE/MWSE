#pragma once

#include <unordered_map>
#include <string>
#include <utility>

namespace mwse::tes3
{

// holds dynamic texts for books
// dynamic texts are set at runtime and do not persist between saved games.
// A good way to use that new feature is to :
// - use tes3book:setDynamicText function at every load
// - using this binding is a way to set text for book created at runtime with tes3book.create
class BookDynamicTextStore
{
public:
	static BookDynamicTextStore &getInstance();

public:
	std::string &operator[]( const std::string &key );
	bool hasTextForBookId( const std::string &key );
	void clearText( const std::string &key );

private:
	BookDynamicTextStore() = default;
	~BookDynamicTextStore() = default;
	BookDynamicTextStore( const BookDynamicTextStore & ) = default;
	BookDynamicTextStore( BookDynamicTextStore && ) = default;

	BookDynamicTextStore &operator=( const BookDynamicTextStore & ) = default;
	BookDynamicTextStore &operator=( BookDynamicTextStore && ) = default;

private:
	std::unordered_map< std::string, std::string > textStore_;
};

}
