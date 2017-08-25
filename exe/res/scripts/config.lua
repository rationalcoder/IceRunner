local MapKit = IceRunner.MapTools.MapKit
local RGB = IceRunner.General.RGB
local MapFactory = IceRunner.DeckSettings.MapFactory

local settings = IceRunner.DeckSettings()
local factories = {}

factories[1] = MapFactory({
    map_color = RGB(0, .85, 1),
    wall_color = RGB(.3, .3, 1),
    goal_color = RGB(1, .5, 1),
    player_color = RGB(0, .85, .2),
    kit = MapKit({size = 18, walls = 70})
})

factories[2] = MapFactory({
    map_color = RGB(.5, 0, .7),
    player_color = RGB(1, 0, 1),
    wall_color = RGB(0, 0, 0),
    goal_color = RGB(.5, .5, 1),
    kit = MapKit({size = 19, walls = 90})
})

factories[3] = MapFactory({
    map_color = RGB(255/255, 200/255,  11/255),
    player_color = RGB(118/255, 255/255, 185/255),
    wall_color = RGB(251/255, 11/255, 63/255),
    goal_color = RGB(76/255, 89/255, 247/255),
    kit = MapKit({size = 20, walls = 110})
})

factories[4] = MapFactory({
    map_color = RGB(103/255, 42/255, 83/255),
    player_color = RGB(30/255, 171/255, 39/255),
    wall_color = RGB(1, 0, 0),
    goal_color = RGB(.5, .5, 1),
    kit = MapKit({size = 21, walls = 130})
})

factories[5] = MapFactory({
    map_color = RGB(36/255, 63/255, 112/255),
    player_color = RGB(133/255, 181/255, 51/255),
    wall_color = RGB(55/255, 40/255, 116/255),
    goal_color = RGB(141/255, 94/255, 0/255),
    kit = MapKit({size = 22, walls = 150})
})

settings:set_factories(factories)
IceRunner.configure(settings)
