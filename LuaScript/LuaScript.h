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

#pragma once

#include <lua.hpp>
#include <functional>
#include <memory>

#include <map>

template<>
//Specify policy to release lua_State*
struct std::default_delete<lua_State>{
	void operator()(lua_State* ptr){
		if(ptr)
			lua_close(ptr);
	}
};

namespace LuaLink
{
	template<typename T>class LuaClass;

	class LuaScript
	{
	public:
		//Constructor & destructor

		LuaScript(const ::std::string& filename);
		virtual ~LuaScript(void);

		//Methods

		// // Opens and loads the file linked to this object
		void Load(void(*initializeEnvironmentFn)(void) = nullptr, bool bOpenLibs = true, bool bResetState = false);
		// // Adds all registered C++ functions and classes to the environment and performs an initial run
		void Initialize (void);

		template<typename _RetType>
		struct Call;
	
	private:
		template<typename T> friend class LuaClass;
	
		//Returns lua_State* (to use when commiting classes)
		static lua_State* GetLuaState(void);

		//Custom Lua allocator
		static void* LuaAllocate(void *ud, void *ptr, size_t osize, size_t nsize);
	
		//Datamembers
	
		::std::string m_Filename;
		void(*InitializeEnvironment)(void); //Function where all needed variables/functions/classes are registeredd to the lua_State

		static ::std::unique_ptr<lua_State> s_pLuaState;

		//Disabling default copy ConstructorWrapper & assignment operator
		LuaScript(const LuaScript& src);
		LuaScript& operator=(const LuaScript& src);
	};
}
	#include "LuaScript.inl"
