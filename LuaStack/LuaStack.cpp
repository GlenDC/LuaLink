#include "LuaStack.h"

using namespace LuaLink;

//getVariable

template<> 
std::string LuaStack::getVariable<std::string>(lua_State* pLua, int varIdx, bool& isOk) 
{
	const char* str = lua_tostring(pLua, varIdx);
	isOk = str != nullptr;

	return std::string(str);	
}

template<>
int LuaStack::getVariable<int>(lua_State* pLua, int varIdx, bool& isOk) 
{
	int isnum;
	lua_Integer i = lua_tointegerx(pLua, varIdx, &isnum);
	isOk = isnum != 0;

	return static_cast<int>(i);	
}

template<>
unsigned int LuaStack::getVariable<unsigned int>(lua_State* pLua, int varIdx, bool& isOk) 
{ 
	int isnum;
	lua_Unsigned u = lua_tounsignedx(pLua, varIdx, &isnum);
	isOk = isnum != 0;

	return static_cast<unsigned int>(u); 
}

template<>
bool LuaStack::getVariable<bool>(lua_State* pLua, int varIdx, bool& isOk) 
{ 
	isOk = lua_isboolean(pLua, varIdx) != 0;
	
	if(isOk)
		return lua_toboolean(pLua, varIdx) == 0 ? false : true;
	
	return false;
}

template<>
double LuaStack::getVariable<double>(lua_State* pLua, int varIdx, bool& isOk) 
{
	int isnum;
	lua_Number d = lua_tonumberx(pLua, varIdx, &isnum);
	isOk = isnum != 0;

	return static_cast<double>(d);	
}

template<>
void* LuaStack::getVariable<void*>(lua_State* pLua, int varIdx, bool& isOk)
{
	isOk = lua_isuserdata(pLua, varIdx) != 0;

	return lua_touserdata(pLua, varIdx);
}

//getVariable (no checking)

template<> 
std::string LuaStack::getVariable<std::string>(lua_State* pLua, int varIdx) 
{
	const char* str = lua_tostring(pLua, varIdx);
	return std::string(str);	
}

template<>
int LuaStack::getVariable<int>(lua_State* pLua, int varIdx) 
{
	lua_Integer i = lua_tointeger(pLua, varIdx);
	return static_cast<int>(i);	
}

template<>
unsigned int LuaStack::getVariable<unsigned int>(lua_State* pLua, int varIdx) 
{ 
	lua_Unsigned u = lua_tounsigned(pLua, varIdx);
	return static_cast<unsigned int>(u); 
}

template<>
bool LuaStack::getVariable<bool>(lua_State* pLua, int varIdx) 
{ 
	return lua_toboolean(pLua, varIdx) == 0 ? false : true;
}

template<>
double LuaStack::getVariable<double>(lua_State* pLua, int varIdx) 
{
	lua_Number d = lua_tonumber(pLua, varIdx);
	return static_cast<double>(d);	
}

template<>
void* LuaStack::getVariable<void*>(lua_State* pLua, int varIdx)
{
	return lua_touserdata(pLua, varIdx);
}

//pushVariable

template<> 
void LuaStack::pushVariable<const char*>(lua_State* pLua, const char* data)
{ 
	lua_pushstring(pLua, data);
}

template<> 
void LuaStack::pushVariable<std::string>(lua_State* pLua, std::string data)
{ 
	lua_pushstring( pLua, data.c_str() );
}

template<> 
void LuaStack::pushVariable<const wchar_t*>(lua_State* pLua, const wchar_t* data)
{ 
	lua_pushlstring( pLua, reinterpret_cast<const char*>(data), (wcslen(data)+1)*sizeof(wchar_t) );
}

template<> 
void LuaStack::pushVariable<std::wstring>(lua_State* pLua, std::wstring data)
{ 
	lua_pushlstring( pLua, reinterpret_cast<const char*>(data.c_str() ), (data.size()+1)*sizeof(wchar_t) );
}

template<> 
void LuaStack::pushVariable<int>(lua_State* pLua, int data)
{ 
	lua_pushinteger(pLua,	static_cast<lua_Integer>(data) );
}

template<> 
void LuaStack::pushVariable<unsigned int>(lua_State* pLua, unsigned int data)
{ 
	lua_pushunsigned(pLua, static_cast<lua_Unsigned>(data) );
}

template<> 
void LuaStack::pushVariable<bool>(lua_State* pLua, bool data)
{ 
	lua_pushboolean(pLua, static_cast<int>(data) );
}

template<> 
void LuaStack::pushVariable<float>(lua_State* pLua, float data)
{ 
	lua_pushnumber(pLua, static_cast<lua_Number>(data) );
}

template<> 
void LuaStack::pushVariable<double>(lua_State* pLua, double data)
{ 
	lua_pushnumber(pLua, static_cast<lua_Number>(data) );
}

template<>
void LuaStack::pushVariable<void*>(lua_State* pLua, void* data)
{
	lua_pushlightuserdata(pLua, data);
}