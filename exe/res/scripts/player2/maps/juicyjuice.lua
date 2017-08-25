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
name = "JUICYJUICE",
level = 2,
kit = MapKit({size = 20, walls = 400 })
})

map:add_walls(Wall(0, 0), Right(19))
map:add_walls(Wall(1, 0), Down(18))
map:add_walls(Wall(1, 19), Down(18))
map:add_walls(Wall(19, 1), Right(18))
map:add_walls(Wall(1,1), Up(0))
map:add_walls(Wall(1,7), Up(0))
map:add_walls(Wall(1,14), Up(0))
map:add_walls(Wall(1,15), Up(0))
map:add_walls(Wall(1,16), Up(0))
map:add_walls(Wall(1,17), Up(0))
map:add_walls(Wall(1,18), Up(0))
map:set_player(Player(2,1))
map:add_walls(Wall(2,11), Up(0))
map:add_walls(Wall(2,14), Up(0))
map:add_walls(Wall(3,7), Up(0))
map:add_walls(Wall(3,17), Up(0))
map:add_walls(Wall(3,18), Up(0))
map:add_walls(Wall(4,6), Up(0))
map:add_walls(Wall(4,7), Up(0))
map:add_walls(Wall(4,18), Up(0))
map:add_walls(Wall(5,3), Up(0))
map:add_walls(Wall(5,14), Up(0))
map:add_walls(Wall(6,9), Up(0))
map:add_walls(Wall(6,13), Up(0))
map:add_walls(Wall(6,14), Up(0))
map:add_walls(Wall(7,1), Up(0))
map:add_walls(Wall(7,6), Up(0))
map:add_walls(Wall(7,7), Up(0))
map:add_walls(Wall(7,8), Up(0))
map:add_walls(Wall(7,9), Up(0))
map:add_walls(Wall(8,11), Up(0))
map:add_walls(Wall(8,12), Up(0))
map:add_walls(Wall(8,13), Up(0))
map:add_walls(Wall(8,14), Up(0))
map:add_walls(Wall(9,17), Up(0))
map:add_walls(Wall(9,18), Up(0))
map:add_walls(Wall(10,7), Up(0))
map:add_walls(Wall(10,8), Up(0))
map:add_walls(Wall(10,9), Up(0))
map:add_walls(Wall(10,10), Up(0))
map:add_walls(Wall(11,10), Up(0))
map:add_walls(Wall(12,4), Up(0))
map:add_walls(Wall(12,5), Up(0))
map:add_walls(Wall(12,12), Up(0))
map:add_walls(Wall(12,13), Up(0))
map:add_walls(Wall(13,5), Up(0))
map:add_walls(Wall(14,3), Up(0))
map:add_walls(Wall(14,8), Up(0))
map:add_walls(Wall(14,14), Up(0))
map:add_walls(Wall(15,5), Up(0))
map:add_walls(Wall(15,6), Up(0))
map:add_walls(Wall(15,7), Up(0))
map:add_walls(Wall(15,8), Up(0))
map:add_walls(Wall(16,1), Up(0))
map:add_walls(Wall(16,4), Up(0))
map:add_walls(Wall(16,11), Up(0))
map:add_walls(Wall(17,3), Up(0))
map:add_walls(Wall(17,5), Up(0))
map:add_walls(Wall(17,16), Up(0))
map:add_walls(Wall(17,17), Up(0))
map:set_goal(Goal(17,18))
map:add_walls(Wall(18,1), Up(0))
map:add_walls(Wall(18,18), Up(0))

IceRunner.register_map(map);
