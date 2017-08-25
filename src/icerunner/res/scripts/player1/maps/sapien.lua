local Map = IceRunner.Map
local MapKit = IceRunner.MapKit
local Up = IceRunner.MapTools.UpExtent
local Down = IceRunner.MapTools.DownExtent
local Left = IceRunner.MapTools.LeftExtent
local Right = IceRunner.MapTools.RightExtent
local Map = IceRunner.Map
local Wall = IceRunner.Map.Wall
local MapKit = IceRunner.MapTools.MapKit
local Player = Map.Player
local Goal = Map.Goal

local map = Map({
    name = "SAPIEN", 
    level = 2,
    kit = MapKit({size = 25, walls = 300})
})

map:add_walls(Wall(0, 0), Right(14))
map:add_walls(Wall(1, 0), Down(13))
map:add_walls(Wall(1, 14), Down(13))
map:add_walls(Wall(14, 1), Right(13))
map:add_walls(Wall(14, 1), Right(13))
map:set_player(Player(4, 5))
map:set_goal(Goal(1, 7))

IceRunner.register_map(map);
