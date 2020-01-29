#pragma once

#include "sol.hpp"

#include "LuaUtil.h"
#include "TES3Util.h"

#include "TES3Activator.h"
#include "TES3Misc.h"
#include "TES3Static.h"
#include "TES3Enchantment.h"
#include "TES3WorldController.h"

namespace mwse::lua
{

class ObjectCreatorBase
{
public:
	virtual sol::object create( sol::table, bool ) const = 0;
};

template< typename ObjectType >
class ObjectCreator : public ObjectCreatorBase
{
public:
	sol::object create( sol::table, bool ) const override
	{
		throw std::runtime_error{ "tes3.createObject: Cannot create an object of that type." };
	}
};

using InvalidObjectCreator = ObjectCreator< void >;

template<>
class ObjectCreator< TES3::Activator > : public ObjectCreatorBase
{
public:
	sol::object create( sol::table params, bool getIfExists ) const override
	{
		std::string id = getOptionalParam< std::string >( params, "id", {} );

		if( id.size() > 31 )
			throw std::invalid_argument{ "tes3activator.create: 'id' parameter must be less than 32 character long." };

		if( auto existingObject = TES3::DataHandler::get()->nonDynamicData->resolveObject( id.c_str() ); existingObject != nullptr )
			return ( getIfExists && ( existingObject->objectType == TES3::ObjectType::Activator ) ) ?
			makeLuaObject( existingObject ) :
			throw std::invalid_argument{ "tes3activator.create: 'id' parameter already assigned to an existing object that is not an activator." };

		std::string name = getOptionalParam< std::string >( params, "name", "Activator" );
		if( name.size() > 31 )
			throw std::invalid_argument{ "tes3activator.create: 'name' parameter must be less than 32 character long." };

		auto mesh = getOptionalParam< std::string >( params, "mesh", {} );
		if( mesh.size() > 31 )
			throw std::invalid_argument{ "tes3activator.create: 'mesh' parameter must be less than 32 character long." };

		auto activator = new TES3::Activator();

		activator->setID( id.c_str() );
		activator->setName( name.c_str() );
		activator->setModelPath( mesh.c_str() );

		auto script = getOptionalParamScript( params, "script" );

		if( script != nullptr )
			activator->script = script;

		activator->objectFlags = getOptionalParam< double >( params, "objectFlags", 0.0 );

		activator->objectFlags |= TES3::ObjectFlag::Modified;

		if( !TES3::DataHandler::get()->nonDynamicData->addNewObject( activator ) )
			throw std::runtime_error( "tes3activator.create: could not add the newly created activator in its proper collection." );

		// If created outside of a save game, mark the object as sourceless.
		if (getOptionalParam<bool>(params, "sourceless", false) || TES3::WorldController::get()->getMobilePlayer() == nullptr) {
			TES3::BaseObject::setSourcelessObject(activator);
		}

		return makeLuaObject( activator );
	}
};

template<>
class ObjectCreator< TES3::Misc > : public ObjectCreatorBase
{
public:
	sol::object create( sol::table params, bool getIfExists ) const override
	{
		std::string id = getOptionalParam< std::string >( params, "id", {} );

		if( id.size() > 31 )
			throw std::invalid_argument{ "tes3misc.create: 'id' parameter must be less than 32 character long." };

		if( auto existingObject = TES3::DataHandler::get()->nonDynamicData->resolveObject( id.c_str() ); existingObject != nullptr )
			return ( getIfExists && existingObject->objectType == TES3::ObjectType::Misc ) ?
			makeLuaObject( existingObject ) :
			throw std::invalid_argument{ "tes3misc.create: 'id' parameter already assigned to an existing object that is not a misc item." };

		std::string name = getOptionalParam< std::string >( params, "name", "Miscellaneous item" );
		if( name.size() > 31 )
			throw std::invalid_argument{ "tes3misc.create: 'name' parameter must be less than 32 character long." };

		std::string mesh = getOptionalParam< std::string >( params, "mesh", {} );
		if( mesh.size() > 31 )
			throw std::invalid_argument{ "tes3misc.create: 'mesh' parameter must be less than 32 character long." };

		std::string icon = getOptionalParam< std::string >( params, "icon", {} );
		if( icon.size() > 31 )
			throw std::invalid_argument{ "tes3misc.create: 'icon' parameter must be less than 32 character long." };

		auto miscItem = new TES3::Misc();

		miscItem->setID( id.c_str() );
		miscItem->setName( name.c_str() );
		miscItem->setModelPath( mesh.c_str() );
		tes3::setDataString( &miscItem->icon, icon.c_str() );

		auto script = getOptionalParamScript( params, "script" );
		if( script != nullptr )
			miscItem->script = script;

		miscItem->objectFlags = getOptionalParam< double >( params, "objectFlags", 0.0 );
		miscItem->weight = getOptionalParam< double >( params, "weight", 0.0 );
		miscItem->value = getOptionalParam< double >( params, "value", 0.0 );
		miscItem->flags = getOptionalParam< double >( params, "flags", 0.0 );

		miscItem->objectFlags |= TES3::ObjectFlag::Modified;

		if( !TES3::DataHandler::get()->nonDynamicData->addNewObject( miscItem ) )
			throw std::runtime_error( "tes3misc.create: could not add the newly created misc item in its proper collection." );

		// If created outside of a save game, mark the object as sourceless.
		if (getOptionalParam<bool>(params, "sourceless", false) || TES3::WorldController::get()->getMobilePlayer() == nullptr) {
			TES3::BaseObject::setSourcelessObject(miscItem);
		}

		return makeLuaObject( miscItem );
	}
};

template<>
class ObjectCreator< TES3::Static > : public ObjectCreatorBase
{
public:
	sol::object create( sol::table params, bool getIfExists ) const override
	{
		std::string id = getOptionalParam< std::string >( params, "id", {} );

		if( id.size() > 31 )
			throw std::invalid_argument{ "tes3static.create: 'id' parameter must be less than 32 character long." };

		if( auto existingObject = TES3::DataHandler::get()->nonDynamicData->resolveObject( id.c_str() ); existingObject != nullptr )
			return ( getIfExists && existingObject->objectType == TES3::ObjectType::Static ) ?
			makeLuaObject( existingObject ) :
			throw std::invalid_argument{ "tes3static.create: 'id' parameter already assigned to an existing object that is not a static." };

		std::string mesh = getOptionalParam< std::string >( params, "mesh", {} );
		if( mesh.size() > 31 )
			throw std::invalid_argument{ "tes3static.create: 'mesh' parameter must be less than 32 character long." };

		auto staticObject = new TES3::Static();

		staticObject->setID( id.c_str() );
		staticObject->setModelPath( mesh.c_str() );

		staticObject->objectFlags = getOptionalParam< double >( params, "objectFlags", 0.0 );

		staticObject->objectFlags |= TES3::ObjectFlag::Modified;

		if( !TES3::DataHandler::get()->nonDynamicData->addNewObject( staticObject ) )
			throw std::runtime_error( "tes3static.create: could not add the newly created static in its proper collection." );

		// If created outside of a save game, mark the object as sourceless.
		if (getOptionalParam<bool>(params, "sourceless", false) || TES3::WorldController::get()->getMobilePlayer() == nullptr) {
			TES3::BaseObject::setSourcelessObject(staticObject);
		}

		return makeLuaObject( staticObject );
	}
};

template<>
class ObjectCreator< TES3::Enchantment > : public ObjectCreatorBase
{
public:
	sol::object create( sol::table params, bool getIfExists ) const override
	{
		std::string id = getOptionalParam< std::string >( params, "id", {} );

		if( id.size() > 31 )
			throw std::invalid_argument{ "tes3enchantment.create: 'id' parameter must be less than 32 character long." };

		if( auto existingObject = TES3::DataHandler::get()->nonDynamicData->resolveObject( id.c_str() ); existingObject != nullptr )
			return ( getIfExists && existingObject->objectType == TES3::ObjectType::Enchantment ) ?
			makeLuaObject( existingObject ) :
			throw std::invalid_argument{ "tes3enchantment.create: 'id' parameter already assigned to an existing object that is not an enchantment." };

		auto castType = getOptionalParam( params, "castType", TES3::EnchantmentCastType::Invalid );
		if( castType >= TES3::EnchantmentCastType::Invalid )
			throw std::invalid_argument{ "tes3enchantment.create: 'castType' parameter as an incorrect value. Use values in tes3.enchantmentType" };

		unsigned short chargeCost = getOptionalParam< double >( params, "chargeCost", 0.0 );
		if( chargeCost == 0 )
			throw std::invalid_argument{ "tes3enchantment.create: 'chargeCost' parameter must be greater than 0." };

		unsigned short maxCharge = getOptionalParam< double >( params, "maxCharge", 0.0 );
		if( maxCharge == 0 )
			throw std::invalid_argument{ "tes3enchantment.create: 'maxCharge' parameter must be greater than 0." };

		auto enchantment = new TES3::Enchantment();

		enchantment->setID( id.c_str() );
		enchantment->castType = castType;
		enchantment->chargeCost = chargeCost;
		enchantment->maxCharge = maxCharge;
		enchantment->flags = getOptionalParam< double >( params, "flags", 0.0 );
		enchantment->objectFlags = getOptionalParam< double >( params, "objectFlags", 0.0 );
		enchantment->objectFlags |= TES3::ObjectFlag::Modified;

		if( !TES3::DataHandler::get()->nonDynamicData->addNewObject( enchantment ) )
			throw std::runtime_error( "tes3enchantment.create: could not add the newly created enchantment in its proper collection." );

		// If created outside of a save game, mark the object as sourceless.
		if (getOptionalParam<bool>(params, "sourceless", false) || TES3::WorldController::get()->getMobilePlayer() == nullptr) {
			TES3::BaseObject::setSourcelessObject(enchantment);
		}

		return makeLuaObject( enchantment );
	}
};

std::unique_ptr< ObjectCreatorBase > makeObjectCreator( TES3::ObjectType::ObjectType objectType );

}
