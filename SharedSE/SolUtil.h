#pragma once

#include "NIPointer.h"

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
// Pointer overloads only. `Pointer<T>` is returned by value from C++ APIs,
// so the rvalue overload is required under /permissive- (lvalue ref alone
// won't bind to prvalues, and sol2 silently falls back to base usertype).
#define MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(T) \
int sol_lua_push(sol::types<T*>, lua_State* L, T* obj); \
int sol_lua_push(sol::types<NI::Pointer<T>>, lua_State* L, NI::Pointer<T>& obj); \
int sol_lua_push(sol::types<NI::Pointer<T>>, lua_State* L, NI::Pointer<T>&& obj); \
int sol_lua_push(sol::types<NI::Pointer<T>*>, lua_State* L, NI::Pointer<T>* obj);

#define MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(T) \
int sol_lua_push(sol::types<T*>, lua_State* L, T* obj) { return obj->getOrCreateLuaObject(L).push(L); } \
int sol_lua_push(sol::types<NI::Pointer<T>>, lua_State* L, NI::Pointer<T>& obj) { return obj->getOrCreateLuaObject(L).push(L); } \
int sol_lua_push(sol::types<NI::Pointer<T>>, lua_State* L, NI::Pointer<T>&& obj) { return obj->getOrCreateLuaObject(L).push(L); } \
int sol_lua_push(sol::types<NI::Pointer<T>*>, lua_State* L, NI::Pointer<T>* obj) { return obj->get()->getOrCreateLuaObject(L).push(L); }
#endif
