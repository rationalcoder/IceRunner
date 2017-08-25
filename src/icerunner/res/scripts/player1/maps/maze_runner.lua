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
    name = "MAZE RUNNER", 
    level = 2,
    kit = MapKit({size = 20, walls = 200})
})

map:add_walls(Wall(0, 0), Right(19))
map:add_walls(Wall(1, 0), Down(18))
map:add_walls(Wall(1, 19), Down(18))
map:add_walls(Wall(19, 1), Right(18))
map:add_walls(Wall(7, 7), Down(3))
map:add_walls(Wall(10, 8), Right(1))
map:add_walls(Wall(7, 11), Down(6))
map:add_walls(Wall(7, 7), Right(4))
map:add_walls(Wall(13, 11), Left(8))
map:add_walls(Wall(13, 5), Up(10))
map:add_walls(Wall(17, 2), Right(4))
map:add_walls(Wall(18, 13), Up(0))
map:add_walls(Wall(15, 12), Up(0))
map:add_walls(Wall(16, 3), Up(0))
map:add_walls(Wall(1, 5), Up(0))

map:set_player(Player(1, 1))
map:set_goal(Goal(9, 9))

IceRunner.register_map(map);
