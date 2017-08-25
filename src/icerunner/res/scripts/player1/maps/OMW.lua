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
name = "OMW",
level = 2,
kit = MapKit({size = 20, walls = 400 })
})

map:add_walls(Wall(0, 0), Right(19))
map:add_walls(Wall(1, 0), Down(18))
map:add_walls(Wall(1, 19), Down(18))
map:add_walls(Wall(19, 1), Right(18))
map:set_player(Player(1,9))
map:add_walls(Wall(1,18), Up(0))
map:add_walls(Wall(4,6), Up(0))
map:add_walls(Wall(4,12), Up(0))
map:add_walls(Wall(5,6), Up(0))
map:add_walls(Wall(5,7), Up(0))
map:add_walls(Wall(5,8), Up(0))
map:add_walls(Wall(5,9), Up(0))
map:add_walls(Wall(5,10), Up(0))
map:add_walls(Wall(5,11), Up(0))
map:add_walls(Wall(5,12), Up(0))
map:add_walls(Wall(6,1), Up(0))
map:add_walls(Wall(8,8), Up(0))
map:add_walls(Wall(8,9), Up(0))
map:add_walls(Wall(8,10), Up(0))
map:add_walls(Wall(8,18), Up(0))
map:add_walls(Wall(9,9), Up(0))
map:add_walls(Wall(9,17), Up(0))
map:add_walls(Wall(9,18), Up(0))
map:add_walls(Wall(10,9), Up(0))
map:add_walls(Wall(11,9), Up(0))
map:add_walls(Wall(12,5), Up(0))
map:add_walls(Wall(12,6), Up(0))
map:add_walls(Wall(12,13), Up(0))
map:add_walls(Wall(13,1), Up(0))
map:add_walls(Wall(13,3), Up(0))
map:add_walls(Wall(13,4), Up(0))
map:add_walls(Wall(13,5), Up(0))
map:add_walls(Wall(13,6), Up(0))
map:add_walls(Wall(13,7), Up(0))
map:add_walls(Wall(13,11), Up(0))
map:add_walls(Wall(13,12), Up(0))
map:add_walls(Wall(13,13), Up(0))
map:add_walls(Wall(13,14), Up(0))
map:add_walls(Wall(13,15), Up(0))
map:add_walls(Wall(15,4), Up(0))
map:add_walls(Wall(15,5), Up(0))
map:add_walls(Wall(15,6), Up(0))
map:add_walls(Wall(15,7), Up(0))
map:add_walls(Wall(15,11), Up(0))
map:add_walls(Wall(15,12), Up(0))
map:add_walls(Wall(15,13), Up(0))
map:add_walls(Wall(15,14), Up(0))
map:add_walls(Wall(15,16), Up(0))
map:add_walls(Wall(16,16), Up(0))
map:set_goal(Goal(16,17))
map:add_walls(Wall(17,16), Up(0))
map:add_walls(Wall(17,17), Up(0))
map:add_walls(Wall(17,18), Up(0))
map:add_walls(Wall(18,15), Up(0))
map:add_walls(Wall(18,16), Up(0))
map:add_walls(Wall(18,17), Up(0))
map:add_walls(Wall(18,18), Up(0))

IceRunner.register_map(map);
