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
name = "MAPTEST",
level = 1,
kit = MapKit({size = 15, walls = 225 })
})

map:add_walls(Wall(0, 0), Right(14))
map:add_walls(Wall(1, 0), Down(13))
map:add_walls(Wall(1, 14), Down(13))
map:add_walls(Wall(14, 1), Right(13))
map:add_walls(Wall(1,6), Up(0))
map:set_goal(Goal(1,7))
map:add_walls(Wall(1,8), Up(0))
map:add_walls(Wall(2,1), Up(0))
map:add_walls(Wall(2,13), Up(0))
map:add_walls(Wall(3,1), Up(0))
map:add_walls(Wall(3,13), Up(0))
map:add_walls(Wall(4,6), Up(0))
map:add_walls(Wall(5,4), Up(0))
map:add_walls(Wall(5,5), Up(0))
map:add_walls(Wall(5,6), Up(0))
map:add_walls(Wall(5,7), Up(0))
map:add_walls(Wall(5,8), Up(0))
map:add_walls(Wall(5,9), Up(0))
map:add_walls(Wall(5,10), Up(0))
map:add_walls(Wall(6,7), Up(0))
map:add_walls(Wall(7,5), Up(0))
map:add_walls(Wall(7,6), Up(0))
map:add_walls(Wall(7,7), Up(0))
map:add_walls(Wall(7,8), Up(0))
map:add_walls(Wall(7,9), Up(0))
map:add_walls(Wall(8,7), Up(0))
map:add_walls(Wall(9,4), Up(0))
map:add_walls(Wall(9,5), Up(0))
map:add_walls(Wall(9,6), Up(0))
map:add_walls(Wall(9,7), Up(0))
map:add_walls(Wall(9,8), Up(0))
map:add_walls(Wall(9,9), Up(0))
map:add_walls(Wall(9,10), Up(0))
map:add_walls(Wall(10,4), Up(0))
map:add_walls(Wall(10,10), Up(0))
map:add_walls(Wall(11,2), Up(0))
map:add_walls(Wall(11,3), Up(0))
map:add_walls(Wall(11,4), Up(0))
map:add_walls(Wall(11,10), Up(0))
map:add_walls(Wall(11,11), Up(0))
map:add_walls(Wall(11,12), Up(0))
map:set_player(Player(13,7))

IceRunner.register_map(map);
