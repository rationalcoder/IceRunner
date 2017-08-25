local Map = IceRunner.Map
local MapKit = IceRunner.MapKit
local Up = IceRunner.MapTools.UpExtent
local Down = IceRunner.MapTools.DownExtent
local Left = IceRunner.MapTools.LeftExtent
local Right = IceRunner.MapTools.RightExtent
local Map = IceRunner.Map
local Wall = IceRunner.Map.Wall
local MapKit = IceRunner.MapTools.MapKit
local Player = Map.Player
local Goal = Map.Goal

local map = Map({
    name = "Vertigo", 
    level = 2,
    kit = MapKit({size = 25, walls = 500})
})

map:add_walls(Wall(0, 0), Right(24)) -- top wall
map:add_walls(Wall(1, 0), Down(23)) --  left wall
map:add_walls(Wall(1, 24), Down(23)) -- right wall
map:add_walls(Wall(24, 1), Right(23)) -- bottom wall
map:add_walls(Wall(22, 22), Up(15))
map:add_walls(Wall(22, 22), Left(7))
map:add_walls(Wall(22, 15), Up(7))
map:add_walls(Wall(23, 1), Right(7))
map:add_walls(Wall(17, 1), Right(2))
map:add_walls(Wall(6, 14), Down(7))
map:add_walls(Wall(6, 22), Left(15))
map:add_walls(Wall(4, 23), Left(16))
map:add_walls(Wall(5, 1), Right(3))
map:add_walls(Wall(13, 1), Right(4))
map:add_walls(Wall(5, 4), Up(1))
map:add_walls(Wall(2, 4), Up(0))
map:add_walls(Wall(3, 20), Up(0))
map:add_walls(Wall(2, 15), Up(0))
map:add_walls(Wall(3, 23), Up(0))
map:add_walls(Wall(1, 22), Up(0))
map:add_walls(Wall(6, 6), Down(5)) 


map:set_player(Player(23, 23)) -- player start
map:set_goal(Goal(3, 2)) -- player finish

IceRunner.register_map(map);
