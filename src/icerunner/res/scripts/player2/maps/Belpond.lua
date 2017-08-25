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
    name = "Belpond", 
    level = 1,
    kit = MapKit({size = 15, walls = 100})
})

map:add_walls(Wall(0, 0), Right(14))
map:add_walls(Wall(1, 0), Down(13))
map:add_walls(Wall(1, 14), Down(13))
map:add_walls(Wall(14, 1), Right(13))
map:add_walls(Wall(13, 10), Right(0))
map:add_walls(Wall(12, 13), Right(0))
map:add_walls(Wall(4, 1), Right(0))
map:add_walls(Wall(5, 9), Up(2))
map:add_walls(Wall(7, 1), Right(1))
map:add_walls(Wall(4, 9), Left(5))
map:add_walls(Wall(4, 13), Left(2))
map:add_walls(Wall(6, 13), Left(7))
map:set_player(Player(13, 13))
map:set_goal(Goal(5, 13))

IceRunner.register_map(map);
