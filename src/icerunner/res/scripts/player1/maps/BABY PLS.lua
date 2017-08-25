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
name = "BABY PLS",
level = 1,
kit = MapKit({size = 15, walls = 225 })
})

map:add_walls(Wall(0, 0), Right(14))
map:add_walls(Wall(1, 0), Down(13))
map:add_walls(Wall(1, 14), Down(13))
map:add_walls(Wall(14, 1), Right(13))
map:add_walls(Wall(4,3), Up(0))
map:add_walls(Wall(5,3), Up(0))
map:add_walls(Wall(5,13), Up(0))
map:add_walls(Wall(6,3), Up(0))
map:add_walls(Wall(6,6), Up(0))
map:add_walls(Wall(6,8), Up(0))
map:add_walls(Wall(6,10), Up(0))
map:add_walls(Wall(6,13), Up(0))
map:add_walls(Wall(7,3), Up(0))
map:add_walls(Wall(7,5), Up(0))
map:add_walls(Wall(7,6), Up(0))
map:add_walls(Wall(7,8), Up(0))
map:add_walls(Wall(7,10), Up(0))
map:add_walls(Wall(7,13), Up(0))
map:add_walls(Wall(8,3), Up(0))
map:set_goal(Goal(8,5))
map:add_walls(Wall(8,6), Up(0))
map:add_walls(Wall(8,8), Up(0))
map:add_walls(Wall(8,10), Up(0))
map:add_walls(Wall(8,13), Up(0))
map:add_walls(Wall(9,3), Up(0))
map:add_walls(Wall(9,6), Up(0))
map:add_walls(Wall(9,8), Up(0))
map:add_walls(Wall(9,10), Up(0))
map:add_walls(Wall(9,13), Up(0))
map:add_walls(Wall(10,3), Up(0))
map:add_walls(Wall(10,4), Up(0))
map:add_walls(Wall(10,5), Up(0))
map:add_walls(Wall(10,6), Up(0))
map:add_walls(Wall(10,7), Up(0))
map:add_walls(Wall(10,8), Up(0))
map:add_walls(Wall(10,9), Up(0))
map:add_walls(Wall(10,10), Up(0))
map:add_walls(Wall(10,11), Up(0))
map:add_walls(Wall(10,12), Up(0))
map:add_walls(Wall(10,13), Up(0))
map:add_walls(Wall(11,3), Up(0))
map:add_walls(Wall(11,4), Up(0))
map:add_walls(Wall(11,5), Up(0))
map:add_walls(Wall(11,6), Up(0))
map:add_walls(Wall(11,7), Up(0))
map:add_walls(Wall(11,8), Up(0))
map:add_walls(Wall(11,9), Up(0))
map:add_walls(Wall(11,10), Up(0))
map:add_walls(Wall(11,11), Up(0))
map:add_walls(Wall(11,12), Up(0))
map:add_walls(Wall(11,13), Up(0))
map:set_player(Player(13,1))

IceRunner.register_map(map);
