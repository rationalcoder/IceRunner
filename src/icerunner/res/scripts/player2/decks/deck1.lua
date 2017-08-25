local Deck = IceRunner.Deck

local maps = {}
maps[1] = "EASYMAP1"
maps[2] = "MEDIUMMAP1"
maps[3] = "LARGEMAP1"
maps[4] = "LARGEMAP3"
maps[5] = "LARGEMAP2"

local cory_sucks = Deck({ name = "CORY SUCKS", map_list = maps })
IceRunner.register_deck(cory_sucks)