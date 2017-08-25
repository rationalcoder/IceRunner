local Deck = IceRunner.Deck

local maps = {}
maps[1] = "TONGUEFART"
maps[2] = "JUICYJUICE"
maps[3] = "MATADOR"
maps[4] = "LEGOMYEGGO"
maps[5] = "BEEZLEDUNG"

local dank_memes = Deck({ name = "DANK MEMES", map_list = maps })
IceRunner.register_deck(dank_memes)