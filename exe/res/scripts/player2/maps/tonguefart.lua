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
name = "TONGUEFART",
level = 1,
kit = MapKit({size = 15, walls = 225 })
})

map:add_walls(Wall(0, 0), Right(14))
map:add_walls(Wall(1, 0), Down(13))
map:add_walls(Wall(1, 14), Down(13))
map:add_walls(Wall(14, 1), Right(13))
map:set_player(Player(1,4))
map:add_walls(Wall(1,5), Up(0))
map:add_walls(Wall(2,3), Up(0))
map:add_walls(Wall(2,4), Up(0))
map:add_walls(Wall(2,5), Up(0))
map:add_walls(Wall(3,10), Up(0))
map:add_walls(Wall(3,11), Up(0))
map:add_walls(Wall(4,4), Up(0))
map:add_walls(Wall(4,11), Up(0))
map:add_walls(Wall(5,4), Up(0))
map:add_walls(Wall(5,8), Up(0))
map:add_walls(Wall(5,11), Up(0))
map:add_walls(Wall(6,4), Up(0))
map:add_walls(Wall(6,8), Up(0))
map:add_walls(Wall(6,9), Up(0))
map:add_walls(Wall(7,7), Up(0))
map:add_walls(Wall(7,8), Up(0))
map:add_walls(Wall(9,11), Up(0))
map:add_walls(Wall(10,1), Up(0))
map:add_walls(Wall(10,2), Up(0))
map:add_walls(Wall(10,7), Up(0))
map:add_walls(Wall(11,1), Up(0))
map:add_walls(Wall(11,13), Up(0))
map:add_walls(Wall(12,1), Up(0))
map:add_walls(Wall(12,8), Up(0))
map:add_walls(Wall(12,10), Up(0))
map:add_walls(Wall(13,1), Up(0))
map:add_walls(Wall(13,9), Up(0))
map:add_walls(Wall(13,10), Up(0))
map:add_walls(Wall(13,11), Up(0))
map:set_goal(Goal(13,13))

IceRunner.register_map(map);
