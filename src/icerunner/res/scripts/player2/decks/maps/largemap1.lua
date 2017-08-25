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
name = "LARGEMAP1",
level = 3,
kit = MapKit({size = 25, walls = 625 })
})

map:add_walls(Wall(0, 0), Right(24))
map:add_walls(Wall(1, 0), Down(23))
map:add_walls(Wall(1, 24), Down(23))
map:add_walls(Wall(24, 1), Right(23))
map:add_walls(Wall(1,1), Up(0))
map:add_walls(Wall(1,2), Up(0))
map:add_walls(Wall(1,3), Up(0))
map:add_walls(Wall(1,6), Up(0))
map:set_player(Player(2,2))
map:add_walls(Wall(2,3), Up(0))
map:add_walls(Wall(3,1), Up(0))
map:add_walls(Wall(3,20), Up(0))
map:add_walls(Wall(4,16), Up(0))
map:add_walls(Wall(4,20), Up(0))
map:add_walls(Wall(5,16), Up(0))
map:add_walls(Wall(5,20), Up(0))
map:add_walls(Wall(5,21), Up(0))
map:add_walls(Wall(5,22), Up(0))
map:add_walls(Wall(6,13), Up(0))
map:add_walls(Wall(6,16), Up(0))
map:add_walls(Wall(6,19), Up(0))
map:add_walls(Wall(7,13), Up(0))
map:add_walls(Wall(7,14), Up(0))
map:add_walls(Wall(7,15), Up(0))
map:add_walls(Wall(7,16), Up(0))
map:add_walls(Wall(7,17), Up(0))
map:add_walls(Wall(7,18), Up(0))
map:add_walls(Wall(7,19), Up(0))
map:add_walls(Wall(7,21), Up(0))
map:add_walls(Wall(7,22), Up(0))
map:add_walls(Wall(7,23), Up(0))
map:add_walls(Wall(8,6), Up(0))
map:add_walls(Wall(8,7), Up(0))
map:add_walls(Wall(8,8), Up(0))
map:add_walls(Wall(8,9), Up(0))
map:add_walls(Wall(8,10), Up(0))
map:add_walls(Wall(8,11), Up(0))
map:add_walls(Wall(9,11), Up(0))
map:add_walls(Wall(10,4), Up(0))
map:add_walls(Wall(10,11), Up(0))
map:add_walls(Wall(10,16), Up(0))
map:add_walls(Wall(10,17), Up(0))
map:add_walls(Wall(11,4), Up(0))
map:add_walls(Wall(11,11), Up(0))
map:add_walls(Wall(11,12), Up(0))
map:add_walls(Wall(11,16), Up(0))
map:add_walls(Wall(11,17), Up(0))
map:add_walls(Wall(12,4), Up(0))
map:add_walls(Wall(12,14), Up(0))
map:add_walls(Wall(12,15), Up(0))
map:add_walls(Wall(12,16), Up(0))
map:add_walls(Wall(12,17), Up(0))
map:add_walls(Wall(12,18), Up(0))
map:add_walls(Wall(12,19), Up(0))
map:add_walls(Wall(13,4), Up(0))
map:add_walls(Wall(13,17), Up(0))
map:add_walls(Wall(14,2), Up(0))
map:add_walls(Wall(14,3), Up(0))
map:add_walls(Wall(14,4), Up(0))
map:add_walls(Wall(14,8), Up(0))
map:add_walls(Wall(14,16), Up(0))
map:add_walls(Wall(15,2), Up(0))
map:add_walls(Wall(15,6), Up(0))
map:add_walls(Wall(15,17), Up(0))
map:add_walls(Wall(15,22), Up(0))
map:add_walls(Wall(15,23), Up(0))
map:add_walls(Wall(16,2), Up(0))
map:add_walls(Wall(16,4), Up(0))
map:add_walls(Wall(16,6), Up(0))
map:add_walls(Wall(16,7), Up(0))
map:add_walls(Wall(16,12), Up(0))
map:add_walls(Wall(16,13), Up(0))
map:add_walls(Wall(16,14), Up(0))
map:add_walls(Wall(16,15), Up(0))
map:add_walls(Wall(16,16), Up(0))
map:add_walls(Wall(16,22), Up(0))
map:set_goal(Goal(16,23))
map:add_walls(Wall(17,4), Up(0))
map:add_walls(Wall(17,7), Up(0))
map:add_walls(Wall(17,8), Up(0))
map:add_walls(Wall(17,17), Up(0))
map:add_walls(Wall(17,21), Up(0))
map:add_walls(Wall(17,22), Up(0))
map:add_walls(Wall(18,1), Up(0))
map:add_walls(Wall(18,2), Up(0))
map:add_walls(Wall(18,3), Up(0))
map:add_walls(Wall(18,4), Up(0))
map:add_walls(Wall(18,8), Up(0))
map:add_walls(Wall(19,5), Up(0))
map:add_walls(Wall(19,7), Up(0))
map:add_walls(Wall(20,22), Up(0))
map:add_walls(Wall(21,4), Up(0))
map:add_walls(Wall(21,5), Up(0))
map:add_walls(Wall(21,6), Up(0))
map:add_walls(Wall(21,7), Up(0))
map:add_walls(Wall(21,12), Up(0))
map:add_walls(Wall(21,18), Up(0))
map:add_walls(Wall(21,19), Up(0))
map:add_walls(Wall(21,22), Up(0))
map:add_walls(Wall(22,22), Up(0))
map:add_walls(Wall(23,21), Up(0))
map:add_walls(Wall(23,22), Up(0))

IceRunner.register_map(map);
