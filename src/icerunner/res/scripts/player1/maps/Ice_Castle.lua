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
    name = "Ice Tunnel", 
    level = 1,
    kit = MapKit({size = 21, walls = 500})
})

map:add_walls(Wall(0, 0), Right(19))
map:add_walls(Wall(1, 0), Down(18))
map:add_walls(Wall(1, 19), Down(18))
map:add_walls(Wall(19, 1), Right(18))
map:add_walls(Wall(1, 4), Left(2))
map:add_walls(Wall(18, 7), Right(0))
map:add_walls(Wall(16, 9), Up(3))
map:add_walls(Wall(16, 5), Up(0))
map:add_walls(Wall(15, 16), Right(2))
map:add_walls(Wall(15, 16), Up(8))
map:add_walls(Wall(5, 15), Right(2))
map:add_walls(Wall(2, 7), Down(1))
map:add_walls(Wall(8, 5), Down(0))
map:add_walls(Wall(8, 10), Up(1))
map:add_walls(Wall(8, 7), Down(1))
map:add_walls(Wall(3, 2), Down(14))
map:add_walls(Wall(13, 4), Up(10))
map:set_player(Player(1, 1))
map:set_goal(Goal(2, 2))

IceRunner.register_map(map);