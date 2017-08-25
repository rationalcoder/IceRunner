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
name = "FUNNEL",
level = 1,
kit = MapKit({size = 15, walls = 225 })
})

map:add_walls(Wall(0, 0), Right(14))
map:add_walls(Wall(1, 0), Down(13))
map:add_walls(Wall(1, 14), Down(13))
map:add_walls(Wall(14, 1), Right(13))
map:add_walls(Wall(1,4), Up(0))
map:add_walls(Wall(1,5), Up(0))
map:set_goal(Goal(1,7))
map:add_walls(Wall(1,9), Up(0))
map:add_walls(Wall(1,10), Up(0))
map:add_walls(Wall(2,2), Up(0))
map:add_walls(Wall(2,4), Up(0))
map:add_walls(Wall(2,7), Up(0))
map:add_walls(Wall(2,10), Up(0))
map:add_walls(Wall(2,12), Up(0))
map:add_walls(Wall(3,2), Up(0))
map:add_walls(Wall(3,4), Up(0))
map:add_walls(Wall(3,7), Up(0))
map:add_walls(Wall(3,10), Up(0))
map:add_walls(Wall(3,12), Up(0))
map:add_walls(Wall(4,2), Up(0))
map:add_walls(Wall(4,4), Up(0))
map:add_walls(Wall(4,7), Up(0))
map:add_walls(Wall(4,10), Up(0))
map:add_walls(Wall(4,12), Up(0))
map:add_walls(Wall(5,2), Up(0))
map:add_walls(Wall(5,4), Up(0))
map:add_walls(Wall(5,7), Up(0))
map:add_walls(Wall(5,10), Up(0))
map:add_walls(Wall(5,12), Up(0))
map:add_walls(Wall(6,2), Up(0))
map:add_walls(Wall(6,4), Up(0))
map:add_walls(Wall(6,7), Up(0))
map:add_walls(Wall(6,10), Up(0))
map:add_walls(Wall(6,12), Up(0))
map:add_walls(Wall(7,2), Up(0))
map:add_walls(Wall(7,4), Up(0))
map:add_walls(Wall(7,7), Up(0))
map:add_walls(Wall(7,10), Up(0))
map:add_walls(Wall(7,12), Up(0))
map:add_walls(Wall(8,2), Up(0))
map:add_walls(Wall(8,4), Up(0))
map:add_walls(Wall(8,7), Up(0))
map:add_walls(Wall(8,10), Up(0))
map:add_walls(Wall(8,12), Up(0))
map:add_walls(Wall(9,2), Up(0))
map:add_walls(Wall(9,4), Up(0))
map:add_walls(Wall(9,7), Up(0))
map:add_walls(Wall(9,10), Up(0))
map:add_walls(Wall(9,12), Up(0))
map:add_walls(Wall(10,2), Up(0))
map:add_walls(Wall(10,4), Up(0))
map:add_walls(Wall(10,7), Up(0))
map:add_walls(Wall(10,10), Up(0))
map:add_walls(Wall(10,12), Up(0))
map:add_walls(Wall(11,2), Up(0))
map:add_walls(Wall(11,4), Up(0))
map:add_walls(Wall(11,7), Up(0))
map:add_walls(Wall(11,10), Up(0))
map:add_walls(Wall(11,12), Up(0))
map:add_walls(Wall(12,2), Up(0))
map:add_walls(Wall(12,4), Up(0))
map:add_walls(Wall(12,7), Up(0))
map:add_walls(Wall(12,10), Up(0))
map:add_walls(Wall(12,12), Up(0))
map:add_walls(Wall(13,2), Up(0))
map:add_walls(Wall(13,6), Up(0))
map:add_walls(Wall(13,7), Up(0))
map:add_walls(Wall(13,8), Up(0))
map:add_walls(Wall(13,12), Up(0))
map:set_player(Player(13,13))

IceRunner.register_map(map);
