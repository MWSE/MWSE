#include "BookTextStore.h"

namespace mwse::tes3
{
BookDynamicTextStore &BookDynamicTextStore::getInstance()
{
	static BookDynamicTextStore store;

	return store;
}

std::string &BookDynamicTextStore::operator[]( const std::string &key )
{
	return textStore_[ key ];
}

bool BookDynamicTextStore::hasTextForBookId( const std::string &key )
{
	return textStore_.count( key ) > 0;
}

void BookDynamicTextStore::clearText( const std::string &key )
{
	textStore_.erase( key );
}

}