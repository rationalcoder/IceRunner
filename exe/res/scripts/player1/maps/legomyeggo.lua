local Map = IceRunner.Map
local MapKit = IceRunner.MapKit
local Up = IceRunner.MapTools.UpExtent
local Down = IceRunner.MapTools.DownExtent
local Left = IceRunner.MapTools.LeftExtent
local Right = IceRunner.MapTools.RightExtent
local Wall = IceRunner.Map.Wall
local MapKit = IceRunner.MapTools.MapKit
local Player = Map.Player
local Goal = Map.Goal

local map = Map({
name = "LEGOMYEGGO",
level = 4,
kit = MapKit({size = 25, walls = 625 })
})

map:add_walls(Wall(0, 0), Right(24))
map:add_walls(Wall(1, 0), Down(23))
map:add_walls(Wall(1, 24), Down(23))
map:add_walls(Wall(24, 1), Right(23))
map:add_walls(Wall(1,13), Up(0))
map:add_walls(Wall(1,16), Up(0))
map:add_walls(Wall(1,21), Up(0))
map:add_walls(Wall(2,2), Up(0))
map:add_walls(Wall(2,3), Up(0))
map:add_walls(Wall(2,4), Up(0))
map:add_walls(Wall(3,5), Up(0))
map:add_walls(Wall(3,16), Up(0))
map:add_walls(Wall(3,20), Up(0))
map:add_walls(Wall(3,22), Up(0))
map:add_walls(Wall(4,5), Up(0))
map:add_walls(Wall(4,6), Up(0))
map:add_walls(Wall(4,7), Up(0))
map:add_walls(Wall(4,11), Up(0))
map:add_walls(Wall(5,11), Up(0))
map:add_walls(Wall(5,16), Up(0))
map:add_walls(Wall(5,22), Up(0))
map:add_walls(Wall(6,8), Up(0))
map:add_walls(Wall(6,19), Up(0))
map:add_walls(Wall(7,4), Up(0))
map:add_walls(Wall(7,5), Up(0))
map:add_walls(Wall(7,17), Up(0))
map:add_walls(Wall(8,4), Up(0))
map:add_walls(Wall(8,11), Up(0))
map:add_walls(Wall(8,15), Up(0))
map:add_walls(Wall(8,20), Up(0))
map:add_walls(Wall(9,1), Up(0))
map:add_walls(Wall(9,2), Up(0))
map:add_walls(Wall(9,7), Up(0))
map:add_walls(Wall(9,10), Up(0))
map:add_walls(Wall(9,12), Up(0))
map:add_walls(Wall(9,15), Up(0))
map:add_walls(Wall(9,19), Up(0))
map:set_player(Player(10,2))
map:add_walls(Wall(10,3), Up(0))
map:add_walls(Wall(10,5), Up(0))
map:add_walls(Wall(10,6), Up(0))
map:add_walls(Wall(10,22), Up(0))
map:add_walls(Wall(11,17), Up(0))
map:add_walls(Wall(11,20), Up(0))
map:add_walls(Wall(12,5), Up(0))
map:add_walls(Wall(13,5), Up(0))
map:add_walls(Wall(13,7), Up(0))
map:add_walls(Wall(13,10), Up(0))
map:add_walls(Wall(13,17), Up(0))
map:add_walls(Wall(13,20), Up(0))
map:add_walls(Wall(14,6), Up(0))
map:add_walls(Wall(15,6), Up(0))
map:add_walls(Wall(15,13), Up(0))
map:add_walls(Wall(15,14), Up(0))
map:add_walls(Wall(15,20), Up(0))
map:add_walls(Wall(16,1), Up(0))
map:add_walls(Wall(16,7), Up(0))
map:add_walls(Wall(16,11), Up(0))
map:add_walls(Wall(16,16), Up(0))
map:add_walls(Wall(16,17), Up(0))
map:add_walls(Wall(16,19), Up(0))
map:add_walls(Wall(17,6), Up(0))
map:add_walls(Wall(17,8), Up(0))
map:add_walls(Wall(17,20), Up(0))
map:add_walls(Wall(18,6), Up(0))
map:add_walls(Wall(18,18), Up(0))
map:set_goal(Goal(18,19))
map:add_walls(Wall(19,4), Up(0))
map:add_walls(Wall(19,6), Up(0))
map:add_walls(Wall(19,17), Up(0))
map:add_walls(Wall(19,19), Up(0))
map:add_walls(Wall(20,4), Up(0))
map:add_walls(Wall(21,3), Up(0))
map:add_walls(Wall(21,6), Up(0))
map:add_walls(Wall(21,8), Up(0))
map:add_walls(Wall(21,18), Up(0))
map:add_walls(Wall(22,8), Up(0))
map:add_walls(Wall(23,3), Up(0))
map:add_walls(Wall(23,14), Up(0))

IceRunner.register_map(map);
