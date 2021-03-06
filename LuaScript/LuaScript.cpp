// Copyright � 2013 Tom Tondeur
// 
// This file is part of LuaLink.
// 
// LuaLink is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// LuaLink is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with LuaLink.  If not, see <http://www.gnu.org/licenses/>.

#include "LuaScript.h"
#include "../LuaClass/LuaClass.h"
#include <map>

using namespace LuaLink;

#define LUA_STATE s_pLuaState.get()

//Initialize static members
std::unique_ptr<lua_State> LuaScript::s_pLuaState = std::unique_ptr<lua_State>();

//Constructor & destructor

LuaScript::LuaScript(const std::string& filename) : m_Filename(filename), InitializeEnvironment(nullptr) {}
LuaScript::~LuaScript(void) {}

//Methods

// // Opens and loads the file linked to this object
void LuaScript::Load(void(*initializeEnvironmentFn)(void), bool bOpenLibs, bool bResetState)
{
	InitializeEnvironment = initializeEnvironmentFn; //Set initializer callback

	//Allocate new lua_State if necessary
	if(bResetState || !s_pLuaState){
		s_pLuaState = std::unique_ptr<lua_State>(lua_newstate(&LuaAllocate, nullptr)); //Unique pointer automatically destroys previous lua_State
	
		if(!s_pLuaState)
			throw LuaLoadException("Error allocating new lua state");
	}

	//Opens commonly used libraries
	if(bOpenLibs)
		luaL_openlibs(LUA_STATE);
	
	// Load a Lua script chunk without executing it
	switch(luaL_loadfile(s_pLuaState.get(), m_Filename.c_str() ))
	{
	case 0:
		break;
	case LUA_ERRFILE:	// Unable to open the file from luaL_loadfile()
	case LUA_ERRSYNTAX: // Syntax error in the lua code in the file from lua_load()
	case LUA_ERRMEM:	// Memory allocation error from lua_load()
		throw LuaLoadException(lua_tostring(LUA_STATE, -1));
		break;
	default:
		throw LuaLoadException("An unknown error has occured while loading file " + m_Filename);
	}
}

// // Adds all registered C++ functions and classes to the environment and performs an initial run
void LuaScript::Initialize(void)
{
	if(LUA_STATE == nullptr)
		Load();

	LuaFunction::Release(); //Release any leftover functions

	if (InitializeEnvironment)
		InitializeEnvironment();

	LuaFunction::Commit(LUA_STATE); //Commit all functions registered in 'InitializeEnvironment'
	
	//Runs the script a first time to register functions and classes declared in the Lua script
	switch(lua_pcall(LUA_STATE, 0, LUA_MULTRET, 0))
	{
	case 0:
		break;
	case LUA_ERRRUN:
	case LUA_ERRMEM:
	case LUA_ERRERR:
		throw LuaCallException(lua_tostring(LUA_STATE, -1));
		break;
	default:
		throw LuaCallException("An unknown error has occured while executing file " + m_Filename);
	}	
}

//  The type of the memory-allocation function used by Lua states. The allocator function
//  must provide a functionality similar to realloc, but not exactly the same. Its arguments
//  are:
//     ud, an opaque pointer passed to lua_newstate;
//     ptr, a pointer to the block being allocated/reallocated/freed;
//     osize, the original size of the block;
//     nsize, the new size of the block.
//
//  ptr is NULL if and only if osize is zero. When nsize is zero, the allocator
//  must return NULL; if osize is not zero, it should free the block pointed to by ptr. When nsize
//  is not zero, the allocator returns NULL if and only if it cannot fill the request. When nsize
//  is not zero and osize is zero, the allocator should behave like malloc. When nsize and osize
//  are not zero, the allocator behaves like realloc().
//
//  Lua assumes that the allocator never fails when osize >= nsize.
//
//  See also this documentation http://www.lua.org/manual/5.2/manual.html#lua_Alloc
//
void* LuaScript::LuaAllocate(void *ud, void *ptr, size_t osize, size_t nsize)
{
	void *pOut = nullptr;

	if (osize && nsize && ptr) {
		if (osize < nsize)
			pOut = realloc (ptr, nsize);
		else
			pOut = ptr;
	}
	else if (nsize == 0)
		free(ptr);
	else
		pOut = malloc (nsize);

	return pOut;
}

lua_State* LuaScript::GetLuaState(void)
{
	return LUA_STATE;
}

#undef LUA_STATE