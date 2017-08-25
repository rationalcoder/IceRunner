local MapKit = IceRunner.MapTools.MapKit
local RGB = IceRunner.General.RGB
local MapFactory = IceRunner.DeckSettings.MapFactory

local settings = IceRunner.DeckSettings()
local factories = {}

factories[1] = MapFactory({
    map_color = RGB(16/255, 126/255, 204/255),
    wall_color = RGB(32/255, 204/255, 16/255),
    goal_color = RGB(204/255, 16/255, 126/255),
    player_color = RGB(1, .85, .2),
    kit = MapKit({size = 20, walls = 100})
})

factories[2] = MapFactory({
    map_color = RGB(.8, .1, .8),
    player_color = RGB(.9, 0, .9),
    wall_color = RGB(0, 0, 0),
    goal_color = RGB(.6, .6, .99),
    kit = MapKit({size = 20, walls = 225})
})

factories[3] = MapFactory({
    map_color = RGB(167/255, 181/255, 11/255),
    player_color = RGB(0, .47, 1),
    wall_color = RGB(1, 0, 0),
    goal_color = RGB(.59, .5, 1),
    kit = MapKit({size = 25, walls = 625})
})

factories[4] = MapFactory({
    map_color = RGB(.2, .9, .85),
    player_color = RGB(0, .85, 1),
    wall_color = RGB(1, 0, 0),
    goal_color = RGB(.5, .5, 1),
    kit = MapKit({size = 25, walls = 625})
})

factories[5] = MapFactory({
    map_color = RGB(0, .85, .85),
    player_color = RGB(0, .85, 1),
    wall_color = RGB(1, 0, 0),
    goal_color = RGB(.5, .5, 1),
    kit = MapKit({size = 25, walls = 625})
})

settings:set_factories(factories)
IceRunner.configure(settings)
