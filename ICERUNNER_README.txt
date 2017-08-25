************** CONTROLS *****************

NOTE: XBOX CONTROLLER REQUIRED TO PLAY GAME!!!

MENU NAVIGATION:
	CYCLE THROUGH BUTTONS:  LEFT ANALOG STICK
	SELECT MENU OPTION: 	A
	GO BACK:                B

RACING:
	MOVE PLAYER:            LEFT ANALOG STICK
	RESET PLAYER:           Y
	EXIT TO DECK CHOOSING:  B (player 1 only)
	
RESULTS:
	BACK TO MAIN MENU:      B (player1 only)

********************************************************

****** RACE SETUP *************

1. Cycle through the available decks
2. Select the deck that you want the other player to play
3. Have both players press their select button.


****** MAP TESTING **********

1. Cycle through the available maps
2. Select the one you to test.

If you make changes to the config file or add a map, you can press the
configure/reload maps buttons respectively to see your changes real-time. 

****** MAP EDITOR (SEPARATE APPLICATION) *******
1. In EXE there is a executable(jar) file called "Map Maker 2.0" This is the map editor, and can be used to make maps for ice runner.
   (Note: You were originally intended to write the lua code manually; however, for ease of use, use the map editor instead)
   (ANOTHER NOTE: You can still manually create maps in Lua, if you desire.)
   
2. INSTRUCTIONS:

a) Open Map Maker 2.0
b) Select one of the three available map sizes in the bottom left window pane
c) Select the intended difficulty via radio button in the bottom left window pane.
d) Start building a new map by clicking on one of the three colored 'tiles' in the top left corner. The currently seleceted tile is indicated by a yellow bounding box.
e) Left clicking on any space in the grid will color that space with the currently selected tile.
d) Right clicking on any space will fill it with an empty tile, which represents empty space.
f) Once satisfied with your map click "file" and then "Save" in the top left corner of the window.
d) A Save Dialog will appear, the default directory is the desktop.
e) Name your map whatever you like and click save. You do not need to include the .lua extension, this is done automatically.
f) Once you have saved your map, move the file into the exe/player[1|2]/maps folder.

MAP COLOR LEGEND:
	Blue = Wall
	Green = Player Finish
	Gold = Player Starting Point
	White = Empty Space
	

******* FILE SYSTEM ******

Each player has their own directory under the res folder (located at the same level as the executable).
Inside, there two directories: "maps" and "decks".

DECK FILE FORMAT:

A deck consists of a mapping between the level of difficulty of a map and the name of the map
to be found in the corresponding player's map directory.

For example:
-- Example file

local Deck = IceRunner.Deck

local maps = {}
maps[1] = "<MAP WITH DIFFICULTY 1>"
maps[2] = "<MAP WITH DIFFICULTY 2>"
maps[3] = "<MAP WITH DIFFICULTY 3>"
maps[4] = "<MAP WITH DIFFICULTY 4>"
maps[5] = "<MAP WITH DIFFICULTY 5>"

local deckname = Deck({ name = "<deck name>", map_lists = maps})
IceRunner.register_deck(deckname)

-- end Example file

The index of the map table is the difficulty slot in your deck that that specific map will take up. i.e. maps[1] => deck slot at difficulty 1.
