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
name = "GETTING THERE",
level = 2,
kit = MapKit({size = 20, walls = 400 })
})

map:add_walls(Wall(0, 0), Right(19))
map:add_walls(Wall(1, 0), Down(18))
map:add_walls(Wall(1, 19), Down(18))
map:add_walls(Wall(19, 1), Right(18))
map:set_player(Player(1,1))
map:add_walls(Wall(1,3), Up(0))
map:add_walls(Wall(1,12), Up(0))
map:add_walls(Wall(1,13), Up(0))
map:add_walls(Wall(1,14), Up(0))
map:add_walls(Wall(1,15), Up(0))
map:add_walls(Wall(1,16), Up(0))
map:add_walls(Wall(1,17), Up(0))
map:add_walls(Wall(2,1), Up(0))
map:add_walls(Wall(2,8), Up(0))
map:add_walls(Wall(2,17), Up(0))
map:add_walls(Wall(3,1), Up(0))
map:add_walls(Wall(3,8), Up(0))
map:add_walls(Wall(4,1), Up(0))
map:add_walls(Wall(4,2), Up(0))
map:add_walls(Wall(4,3), Up(0))
map:add_walls(Wall(4,8), Up(0))
map:add_walls(Wall(9,13), Up(0))
map:add_walls(Wall(10,7), Up(0))
map:add_walls(Wall(10,13), Up(0))
map:add_walls(Wall(11,3), Up(0))
map:add_walls(Wall(11,4), Up(0))
map:add_walls(Wall(11,5), Up(0))
map:add_walls(Wall(11,6), Up(0))
map:add_walls(Wall(11,7), Up(0))
map:add_walls(Wall(11,11), Up(0))
map:add_walls(Wall(11,12), Up(0))
map:add_walls(Wall(11,13), Up(0))
map:add_walls(Wall(12,3), Up(0))
map:set_goal(Goal(12,12))
map:add_walls(Wall(12,13), Up(0))
map:add_walls(Wall(13,3), Up(0))
map:add_walls(Wall(14,3), Up(0))
map:add_walls(Wall(14,4), Up(0))
map:add_walls(Wall(15,4), Up(0))
map:add_walls(Wall(15,12), Up(0))
map:add_walls(Wall(15,17), Up(0))
map:add_walls(Wall(16,1), Up(0))
map:add_walls(Wall(16,9), Up(0))
map:add_walls(Wall(16,12), Up(0))
map:add_walls(Wall(16,13), Up(0))
map:add_walls(Wall(16,16), Up(0))
map:add_walls(Wall(16,17), Up(0))
map:add_walls(Wall(17,1), Up(0))
map:add_walls(Wall(17,2), Up(0))
map:add_walls(Wall(17,3), Up(0))
map:add_walls(Wall(17,4), Up(0))
map:add_walls(Wall(17,9), Up(0))
map:add_walls(Wall(18,9), Up(0))

IceRunner.register_map(map);
