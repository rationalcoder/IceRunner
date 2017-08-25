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
    name = "Cliffholt", 
    level = 2,
    kit = MapKit({size = 25, walls = 500})
})

map:add_walls(Wall(0, 0), Right(24)) -- top wall
map:add_walls(Wall(1, 0), Down(23)) --  left wall
map:add_walls(Wall(1, 24), Down(23)) -- right wall
map:add_walls(Wall(24, 1), Right(23)) -- bottom wall
map:add_walls(Wall(1, 4), Down(5)) 
map:add_walls(Wall(8, 13), Right(5))
map:add_walls(Wall(8, 13), Down(2))
map:add_walls(Wall(10,14), Right(5))
map:add_walls(Wall(9, 23), Up(0))
map:add_walls(Wall(10, 21), Up(0))
map:add_walls(Wall(1, 22), Down(3))
map:add_walls(Wall(3, 10), Down(8))
map:add_walls(Wall(14, 10), Right(3))
map:add_walls(Wall(5, 4), Right(5))
map:add_walls(Wall(23, 8), Up(1))
map:add_walls(Wall(22, 7), Left(1))
map:add_walls(Wall(22, 7), Left(1))
map:add_walls(Wall(15, 23), Left(4))     
map:add_walls(Wall(23, 1), Up(4))
map:add_walls(Wall(20, 3), Right(3))
map:set_player(Player(2, 5)) -- player start

map:set_goal(Goal(23, 7)) -- player finish

IceRunner.register_map(map);
