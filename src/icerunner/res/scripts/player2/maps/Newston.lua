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
    name = "Newston", 
    level = 1,
    kit = MapKit({size = 15, walls = 100})
})

map:add_walls(Wall(0, 0), Right(14))
map:add_walls(Wall(1, 0), Down(13))
map:add_walls(Wall(1, 14), Down(13))
map:add_walls(Wall(14, 1), Right(13))
map:add_walls(Wall(3, 5), Right(3))
map:add_walls(Wall(6, 13), Left(2))
map:add_walls(Wall(6, 6), Down(1))
map:add_walls(Wall(4, 8), Down(3))
map:add_walls(Wall(6, 7), Down(0))
map:add_walls(Wall(12, 9), Left(0))
map:add_walls(Wall(11, 2), Down(0))
map:add_walls(Wall(4, 3), Down(0))
map:set_player(Player(7, 7))
map:set_goal(Goal(5, 7))

IceRunner.register_map(map);
