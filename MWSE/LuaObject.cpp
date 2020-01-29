#include "LuaObject.h"

namespace mwse::lua
{

std::unique_ptr< ObjectCreatorBase > makeObjectCreator( TES3::ObjectType::ObjectType objectType )
{
	switch( objectType )
	{
	case TES3::ObjectType::Activator:
		return std::make_unique< ObjectCreator< TES3::Activator > >();
	case TES3::ObjectType::Book:
		return std::make_unique< ObjectCreator< TES3::Book > >();
	case TES3::ObjectType::Misc:
		return std::make_unique< ObjectCreator< TES3::Misc > >();
	case TES3::ObjectType::Static:
		return std::make_unique< ObjectCreator< TES3::Static > >();
	case TES3::ObjectType::Enchantment:
		return std::make_unique< ObjectCreator< TES3::Enchantment> >();
	default:
		return std::make_unique< InvalidObjectCreator >();
	}
}

}