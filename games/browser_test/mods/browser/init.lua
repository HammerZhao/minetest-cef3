minetest.register_node("browser:browser1x1", {
    drawtype = "interactive",
	description = "Browser",
	paramtype2 = "facedir",
	groups = { oddly_breakable_by_hand = 3 },
	tiles = {
		"google.png",
		"google.png",
		"google.png",
		"google.png",
		"google.png",
--		"google.png",
--		"^[browser:1024x1024,ingamebrowser1,https://www.google.com/ncr"
--		"^[browser:320x320,ingamebrowser1_1,https://www.youtube.com/embed/vbU2VXGjQj8?autoplay=1&loop=1"
--        "^[browser:320x320,ingamebrowser1_1,https://www.youtube.com/tv#/watch?v=dQw4w9WgXcQ"
        "^[browser:400x400,luabrowsertest,http://www.minetest.net/"
	},
	on_punch = function(pos, node, player)
--		local fspec = "size[14,10] browser[1,1;12,8;https://www.youtube.com/embed/C-idtmkukBk?autoplay=1]"
--		local fspec = "size[14,10] browser[1,1;12,8;http://minetest.net/]"
--		local fspec = "size[14,10] browser[1,1;12,8;https://www.google.com/ncr]"
--		minetest.show_formspec(player:get_player_name(), "browser:open", fspec)

		minetest.show_formspec(
			player:get_player_name(), "browser:url",
			"size[11,3;]"..
			"field[1,1;10,1;url;URL;]"
		)

	end

--	on_use = function(pos, node, player)
--		local fspec = "size[14,10] browser[1,1;12,8;https://www.youtube.com/embed/C-idtmkukBk?autoplay=1]"
--		minetest.show_formspec(player:get_player_name(), "browser:open", fspec)
--	end

})

minetest.register_on_player_receive_fields(function(player, formname, fields)
	if formname == "browser:url" then
		print("Getting web page")
		local web_page = core:get_web_page("luabrowsertest")
		print("Opening URL")
		web_page:open_url(fields.url)
	end
end)


--[[minetest.register_node("browser:browser1x2", {
	description = "Browser",
	paramtype2 = "facedir",
	can_dig = true,
	tiles = {
		"youtube.png",
		"youtube.png",
		"youtube.png",
		"youtube.png",
		"youtube.png",
		"^[browser:320x320,ingamebrowser1_2,https://www.youtube.com/embed/BqO0dFxUMtI?autoplay=1&loop=1"
	},
})

minetest.register_node("browser:browser1x3", {
	description = "Browser",
	paramtype2 = "facedir",
	can_dig = true,
	tiles = {
		"youtube.png",
		"youtube.png",
		"youtube.png",
		"youtube.png",
		"youtube.png",
		"^[browser:320x320,ingamebrowser1_3,https://www.youtube.com/embed/Ha2f8zU3QP0?autoplay=1&loop=1"
	},
})

minetest.register_node("browser:browser2x1", {
	description = "Browser",
	paramtype2 = "facedir",
	can_dig = true,
	tiles = {
		"youtube.png",
		"youtube.png",
		"youtube.png",
		"youtube.png",
		"youtube.png",
		"^[browser:320x320,ingamebrowser2_1,https://www.youtube.com/embed/ZVv9hx-jkos?autoplay=1&loop=1"
	},
})

minetest.register_node("browser:browser2x2", {
	description = "Browser",
	paramtype2 = "facedir",
	can_dig = true,
	tiles = {
		"youtube.png",
		"youtube.png",
		"youtube.png",
		"youtube.png",
		"youtube.png",
		"^[browser:320x320,ingamebrowser2_2,https://www.youtube.com/tv#/watch?v=dQw4w9WgXcQ"
	},
})

minetest.register_node("browser:browser2x3", {
	description = "Browser",
	paramtype2 = "facedir",
	can_dig = true,
	tiles = {
		"youtube.png",
		"youtube.png",
		"youtube.png",
		"youtube.png",
		"youtube.png",
		"^[browser:320x320,ingamebrowser2_3,https://www.youtube.com/tv#/watch?v=P5MLKUnnT_A"
	},
})

minetest.register_node("browser:browser3x1", {
	description = "Browser",
	paramtype2 = "facedir",
	can_dig = true,
	tiles = {
		"youtube.png",
		"youtube.png",
		"youtube.png",
		"youtube.png",
		"youtube.png",
		"^[browser:320x320,ingamebrowser3_1,https://www.youtube.com/embed/fFQSEg89_nE?autoplay=1&loop=1"
	},
})

minetest.register_node("browser:browser3x2", {
	description = "Browser",
	paramtype2 = "facedir",
	can_dig = true,
	tiles = {
		"youtube.png",
		"youtube.png",
		"youtube.png",
		"youtube.png",
		"youtube.png",
		"^[browser:320x320,ingamebrowser3_2,https://www.youtube.com/embed/kx3RPqmhJrE?autoplay=1&loop=1"
	},
})

minetest.register_node("browser:browser3x3", {
	description = "Browser",
	paramtype2 = "facedir",
	can_dig = true,
	tiles = {
		"youtube.png",
		"youtube.png",
		"youtube.png",
		"youtube.png",
		"youtube.png",
		"^[browser:320x320,ingamebrowser3_3,https://www.youtube.com/embed/8M_oBYNb6UU?autoplay=1&loop=1"
	},
})
]]--

-- Silent audio:
-- https://www.youtube.com/embed/vbU2VXGjQj8
-- https://www.youtube.com/embed/BqO0dFxUMtI
-- https://www.youtube.com/embed/Ha2f8zU3QP0

-- https://www.youtube.com/embed/ZVv9hx-jkos
-- https://www.youtube.com/embed/P5MLKUnnT_A

-- https://www.youtube.com/embed/fFQSEg89_nE
-- https://www.youtube.com/embed/kx3RPqmhJrE
-- https://www.youtube.com/embed/8M_oBYNb6UU

-- Music
-- ??? https://www.youtube.com/v/dQw4w9WgXcQ
