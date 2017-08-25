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
    kit = MapKit({size = 20, walls = 100})
})

factories[2] = MapFactory({
    map_color = RGB(66/255, 134/255, 244/255),
    player_color = RGB(90/255, 134/255, 244/255),
    wall_color = RGB(.3, .3, 1),
    goal_color = RGB(.5, .5, 1),
    kit = MapKit({size = 20, walls = 200})
})

factories[3] = MapFactory({
    map_color = RGB(0, .85, .85),
    player_color = RGB(0, .85, 1),
    wall_color = RGB(1, 0, 0),
    goal_color = RGB(.5, .5, 1),
    kit = MapKit({size = 10, walls = 20})
})

factories[4] = MapFactory({
    map_color = RGB(0, .85, .85),
    player_color = RGB(0, .85, 1),
    wall_color = RGB(1, 0, 0),
    goal_color = RGB(.5, .5, 1),
    kit = MapKit({size = 10, walls = 20})
})

factories[5] = MapFactory({
    map_color = RGB(0, .85, .85),
    player_color = RGB(0, .85, 1),
    wall_color = RGB(1, 0, 0),
    goal_color = RGB(.5, .5, 1),
    kit = MapKit({size = 10, walls = 20})
})

settings:set_factories(factories)
IceRunner.configure(settings)
