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

#include "lua_api/l_internal.h"
#include "common/c_converter.h"
#include "common/c_content.h"
#include "lua_api/l_cef.h"
#include "mt_cef.h"
#include "settings.h"
#include "debug.h"
#include "log.h"

int ModApiBrowser::l_create(lua_State *L)
{
	return 1;
}

