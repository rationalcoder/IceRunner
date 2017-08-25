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
name = "KK LEGO",
level = 2,
kit = MapKit({size = 20, walls = 400 })
})

map:add_walls(Wall(0, 0), Right(19))
map:add_walls(Wall(1, 0), Down(18))
map:add_walls(Wall(1, 19), Down(18))
map:add_walls(Wall(19, 1), Right(18))
map:add_walls(Wall(1,8), Up(0))
map:add_walls(Wall(1,9), Up(0))
map:add_walls(Wall(1,10), Up(0))
map:add_walls(Wall(1,11), Up(0))
map:add_walls(Wall(2,8), Up(0))
map:add_walls(Wall(2,9), Up(0))
map:add_walls(Wall(2,10), Up(0))
map:add_walls(Wall(2,11), Up(0))
map:add_walls(Wall(3,6), Up(0))
map:add_walls(Wall(3,7), Up(0))
map:add_walls(Wall(3,10), Up(0))
map:add_walls(Wall(3,12), Up(0))
map:add_walls(Wall(3,13), Up(0))
map:add_walls(Wall(4,6), Up(0))
map:add_walls(Wall(4,10), Up(0))
map:add_walls(Wall(4,13), Up(0))
map:add_walls(Wall(5,3), Up(0))
map:add_walls(Wall(5,4), Up(0))
map:add_walls(Wall(5,15), Up(0))
map:add_walls(Wall(5,16), Up(0))
map:add_walls(Wall(6,3), Up(0))
map:add_walls(Wall(6,4), Up(0))
map:add_walls(Wall(6,15), Up(0))
map:add_walls(Wall(6,16), Up(0))
map:add_walls(Wall(11,3), Up(0))
map:add_walls(Wall(11,8), Up(0))
map:add_walls(Wall(11,9), Up(0))
map:add_walls(Wall(11,10), Up(0))
map:add_walls(Wall(11,11), Up(0))
map:add_walls(Wall(11,13), Up(0))
map:add_walls(Wall(11,14), Up(0))
map:add_walls(Wall(11,15), Up(0))
map:add_walls(Wall(11,16), Up(0))
map:add_walls(Wall(12,3), Up(0))
map:add_walls(Wall(12,11), Up(0))
map:add_walls(Wall(13,3), Up(0))
map:add_walls(Wall(13,11), Up(0))
map:add_walls(Wall(14,3), Up(0))
map:add_walls(Wall(15,11), Up(0))
map:set_goal(Goal(15,12))
map:add_walls(Wall(16,8), Up(0))
map:add_walls(Wall(16,9), Up(0))
map:add_walls(Wall(16,11), Up(0))
map:add_walls(Wall(16,12), Up(0))
map:add_walls(Wall(17,1), Up(0))
map:add_walls(Wall(17,2), Up(0))
map:add_walls(Wall(17,3), Up(0))
map:add_walls(Wall(17,8), Up(0))
map:add_walls(Wall(17,9), Up(0))
map:add_walls(Wall(17,11), Up(0))
map:add_walls(Wall(17,12), Up(0))
map:add_walls(Wall(17,16), Up(0))
map:add_walls(Wall(17,17), Up(0))
map:add_walls(Wall(17,18), Up(0))
map:add_walls(Wall(18,1), Up(0))
map:add_walls(Wall(18,2), Up(0))
map:add_walls(Wall(18,3), Up(0))
map:add_walls(Wall(18,8), Up(0))
map:add_walls(Wall(18,9), Up(0))
map:set_player(Player(18,10))
map:add_walls(Wall(18,11), Up(0))
map:add_walls(Wall(18,12), Up(0))
map:add_walls(Wall(18,16), Up(0))
map:add_walls(Wall(18,17), Up(0))
map:add_walls(Wall(18,18), Up(0))

IceRunner.register_map(map);
