#ifndef COMPONENTS_LUA_YAMLLOADER_H
#define COMPONENTS_LUA_YAMLLOADER_H

/*
	Adapted from https://gitlab.com/OpenMW/openmw/-/blob/master/components/lua/yamlloader.hpp under GPLv3

	We want to preserve this to work just like OpenMW. The only use of YAML in this project should be OpenMW compatibility.
*/

#include <iosfwd>
#include <string>

#include <sol/forward.hpp>

namespace OpenMW::LuaUtil
{
    sol::object loadYaml(const std::string& input, sol::this_state lua);
}

#endif // COMPONENTS_LUA_YAMLLOADER_H
