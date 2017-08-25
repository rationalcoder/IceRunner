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
name = "CREEPY PASTA",
level = 1,
kit = MapKit({size = 15, walls = 225 })
})

map:add_walls(Wall(0, 0), Right(14))
map:add_walls(Wall(1, 0), Down(13))
map:add_walls(Wall(1, 14), Down(13))
map:add_walls(Wall(14, 1), Right(13))
map:set_player(Player(1,1))
map:add_walls(Wall(1,4), Up(0))
map:add_walls(Wall(1,5), Up(0))
map:add_walls(Wall(1,6), Up(0))
map:add_walls(Wall(1,7), Up(0))
map:add_walls(Wall(1,8), Up(0))
map:add_walls(Wall(1,9), Up(0))
map:add_walls(Wall(1,10), Up(0))
map:add_walls(Wall(2,4), Up(0))
map:add_walls(Wall(2,5), Up(0))
map:add_walls(Wall(2,6), Up(0))
map:add_walls(Wall(2,7), Up(0))
map:add_walls(Wall(2,8), Up(0))
map:add_walls(Wall(2,9), Up(0))
map:add_walls(Wall(2,10), Up(0))
map:add_walls(Wall(3,4), Up(0))
map:add_walls(Wall(3,5), Up(0))
map:add_walls(Wall(3,7), Up(0))
map:add_walls(Wall(3,9), Up(0))
map:add_walls(Wall(3,10), Up(0))
map:add_walls(Wall(4,4), Up(0))
map:add_walls(Wall(4,5), Up(0))
map:add_walls(Wall(4,6), Up(0))
map:add_walls(Wall(4,7), Up(0))
map:add_walls(Wall(4,8), Up(0))
map:add_walls(Wall(4,9), Up(0))
map:add_walls(Wall(4,10), Up(0))
map:add_walls(Wall(5,4), Up(0))
map:add_walls(Wall(5,5), Up(0))
map:add_walls(Wall(5,6), Up(0))
map:add_walls(Wall(5,7), Up(0))
map:add_walls(Wall(5,8), Up(0))
map:add_walls(Wall(5,9), Up(0))
map:add_walls(Wall(5,10), Up(0))
map:add_walls(Wall(6,4), Up(0))
map:add_walls(Wall(6,6), Up(0))
map:set_goal(Goal(6,7))
map:add_walls(Wall(6,8), Up(0))
map:add_walls(Wall(6,10), Up(0))
map:add_walls(Wall(7,4), Up(0))
map:add_walls(Wall(7,6), Up(0))
map:add_walls(Wall(7,8), Up(0))
map:add_walls(Wall(7,10), Up(0))
map:add_walls(Wall(8,4), Up(0))
map:add_walls(Wall(8,6), Up(0))
map:add_walls(Wall(8,8), Up(0))
map:add_walls(Wall(8,10), Up(0))
map:add_walls(Wall(9,4), Up(0))
map:add_walls(Wall(9,6), Up(0))
map:add_walls(Wall(9,8), Up(0))
map:add_walls(Wall(9,10), Up(0))
map:add_walls(Wall(10,4), Up(0))
map:add_walls(Wall(10,6), Up(0))
map:add_walls(Wall(10,8), Up(0))
map:add_walls(Wall(10,10), Up(0))
map:add_walls(Wall(11,6), Up(0))
map:add_walls(Wall(11,8), Up(0))
map:add_walls(Wall(12,6), Up(0))
map:add_walls(Wall(13,11), Up(0))

IceRunner.register_map(map);
