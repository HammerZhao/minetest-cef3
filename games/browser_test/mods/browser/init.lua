minetest.register_node("browser:browser", {
	description = "Browser",
	tiles = {
		"bones_top.png^[transform2",
		"bones_bottom.png",
		"bones_side.png",
		"bones_side.png",
		"bones_rear.png",
		"bones_front.png"
	},
	on_punch = function(pos, node, player)
		local fspec = "size[10,10] browser[1,1;8,8;https://www.youtube.com/embed/diubaOKUzLs?autoplay=1]"
		minetest.show_formspec(player:get_player_name(), "browser:open", fspec)
	end

})
