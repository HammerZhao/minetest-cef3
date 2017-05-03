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

#ifndef MINETEST_L_WEBPAGE_H
#define MINETEST_L_WEBPAGE_H

#include "l_base.h"
#include "mt_cef.h"

class LuaWebPage : public ModApiBase
{
private:
	WebPage* m_webPage;

	static const char className[];
	static const luaL_Reg methods[];

	// garbage collector
	static int gc_object(lua_State *L);

	// Open URL, i.e. browser:open(url = "http://minetest.net")
	static int l_open_url(lua_State *L);
	static int l_back(lua_State *L);
	static int l_forward(lua_State *L);

public:
	LuaWebPage(WebPage *m);
	~LuaWebPage();

	static int create(lua_State *L, WebPage *m);

	static LuaWebPage *checkobject(lua_State *L, int narg);
	static WebPage *getobject(LuaWebPage *ref);
	static WebPage *getobject(lua_State *L, int narg);

	static void Register(lua_State *L);
};

class ModApiWebPage : public ModApiBase {
private:
	// get_page({name=})
	static int l_get_web_page(lua_State *L);

public:
	static void InitializeClient(lua_State *L, int top);
};

#endif // MINETEST_L_WEBPAGE_H
