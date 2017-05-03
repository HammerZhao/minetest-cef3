/*
Minetest
Copyright (C) 2017 Beerholder, Emiel van Rooijen <evrooije@outlook.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "lua_api/l_webpage.h"
#include "lua_api/l_internal.h"
#include "common/c_converter.h"
#include "common/c_content.h"
#include "mt_cef.h"

/* Main API calls */

// local webpage = minetest.get_web_page("browser name")
int ModApiWebPage::l_get_web_page(lua_State *L)
{
	NO_MAP_LOCK_REQUIRED;

	if (!lua_isstring(L, 1))
		return 0;

	std::string webPageName = luaL_checkstring(L, 1);

	WebPage* webPage = MinetestBrowser::GetInstance()->GetWebPage(webPageName);

	if (webPage != NULL) {
		LuaWebPage::create(L, webPage);
	} else {
		lua_pushnil(L);
	}

	return 1;
}

// webpage:open_url("http://minetest.net")
int LuaWebPage::l_open_url(lua_State *L)
{
	NO_MAP_LOCK_REQUIRED;

	LuaWebPage *o = checkobject(L, 1);

	if (!lua_isstring(L, 2))
		return 0;

	std::string url = luaL_checkstring(L, 2);

	WebPage *webPage = o->m_webPage;
	webPage->Open(url);

	return 1;
}

// webpage:back()
int LuaWebPage::l_back(lua_State *L)
{
	NO_MAP_LOCK_REQUIRED;

	LuaWebPage *o = checkobject(L, 1);
	WebPage *webPage = o->m_webPage;

	webPage->Back();

	return 1;
}

// webpage:forward()
int LuaWebPage::l_forward(lua_State *L)
{
	NO_MAP_LOCK_REQUIRED;

	LuaWebPage *o = checkobject(L, 1);
	WebPage *webPage = o->m_webPage;

	webPage->Forward();

	return 1;
}

/* Other crud needed for Lua integration */

LuaWebPage::LuaWebPage(WebPage *webPage):
	m_webPage(webPage)
{
}

LuaWebPage::~LuaWebPage()
{
}

int LuaWebPage::gc_object(lua_State *L)
{
	LuaWebPage *o = *(LuaWebPage **)(lua_touserdata(L, 1));
	delete o;
	return 0;
}

int LuaWebPage::create(lua_State *L, WebPage* webPage)
{
	NO_MAP_LOCK_REQUIRED;
	LuaWebPage *o = new LuaWebPage(webPage);
	*(void **)(lua_newuserdata(L, sizeof(void *))) = o;
	luaL_getmetatable(L, className);
	lua_setmetatable(L, -2);
	return 1;
}

LuaWebPage* LuaWebPage::checkobject(lua_State *L, int narg)
{
	luaL_checktype(L, narg, LUA_TUSERDATA);
	void *ud = luaL_checkudata(L, narg, className);
	if(!ud) luaL_typerror(L, narg, className);
	return *(LuaWebPage**)ud;  // unbox pointer
}

void LuaWebPage::Register(lua_State *L)
{
	lua_newtable(L);
	int methodtable = lua_gettop(L);
	luaL_newmetatable(L, className);
	int metatable = lua_gettop(L);

	lua_pushliteral(L, "__metatable");
	lua_pushvalue(L, methodtable);
	lua_settable(L, metatable);  // hide metatable from Lua getmetatable()

	lua_pushliteral(L, "__index");
	lua_pushvalue(L, methodtable);
	lua_settable(L, metatable);

	lua_pushliteral(L, "__gc");
	lua_pushcfunction(L, gc_object);
	lua_settable(L, metatable);

	lua_pop(L, 1);  // drop metatable

	luaL_openlib(L, 0, methods, 0);  // fill methodtable
	lua_pop(L, 1);  // drop methodtable

}

const char LuaWebPage::className[] = "WebPage";
const luaL_Reg LuaWebPage::methods[] = {
	luamethod(LuaWebPage, open_url),
	{0,0}
};



void ModApiWebPage::InitializeClient(lua_State *L, int top)
{
	API_FCT(get_web_page);
}

// Probably do not need/ want this!
/*int LuaWebPage::create_object(lua_State *L)
{
	NO_MAP_LOCK_REQUIRED;

	LuaWebPage *o = new LuaWebPage();

	*(void **)(lua_newuserdata(L, sizeof(void *))) = o;
	luaL_getmetatable(L, className);
	lua_setmetatable(L, -2);
	return 1;
}
*/
