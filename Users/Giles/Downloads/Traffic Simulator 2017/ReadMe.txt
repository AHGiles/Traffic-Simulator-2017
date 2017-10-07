Started class structure for Car object with implementation for A*

Texture for objects not working, but possibly because of indexing vertices in OBJ
---------------------------------------------------------------------------------------
Car has been added with basic movemnet applied, needs testing
Car needs to be added to the world and rendered etc.
---------------------------------------------------------------------------------------
3D world created and extends 2Dworld, this creates the car list and draws and updates list
currently creates 1 car but its pathfinding is broken so will need to be fixed
---------------------------------------------------------------------------------------
Car now fixed and navigating the world.  press G to generate world the space to start car
trafic lights need to be implements as well as other cars.
---------------------------------------------------------------------------------------
New camera system implemented with better view controls and a zoom, more cars can be produced but pathfinding error often occurs
traffic lights still needs implementation on both pathfinding and road side
---------------------------------------------------------------------------------------
3D world camera implemented and pathfinding error on cars fixed, but a bug still occurs when entering a 1 way loop
Traffic lights and car movement is next with the ability for the cars to not collide in process of implementation
---------------------------------------------------------------------------------------
Car collision done in basic cell checking method, also convieneintly worked as a simple junction control
Traffic lights and GUI control is next
---------------------------------------------------------------------------------------
Simple Traffic lights done but needs checking and fixing as they aren't always obeyed
GUI ray casting still needs work
---------------------------------------------------------------------------------------
Ray casting is 'working' but innaccurate needs someone better than us to fix it
Next is to get road pieces as dragable into scene 
---------------------------------------------------------------------------------------
Ray casting is now more accurate, still needs dragable road pieces from GUI
probably start work on procedual generation of environment or stats
---------------------------------------------------------------------------------------
GUI is now draggable and works fine, need to add GUI images of road pieces
3D models of cars to be updated next
---------------------------------------------------------------------------------------
GUI changed to work off of button class, added textures for road piece buttons
3D models of cars to be updated next
---------------------------------------------------------------------------------------
Added buttons to GUI for gen random map, grid size, load save, and start sim (num cars needs adding)
3D models of cars to be updated next and textures for all added buttons
---------------------------------------------------------------------------------------
traffic lights working properly.  cars now avoid large volumes of traffic with A* where possible.  cars make new paths if they havent moved for 15 seconds
road shader set up to dissplay active traffic light by highlighting the neighbour cell that is active. 
couldnt get fancy shader working properly to do it within the junction cell itself.
Need to address memory problems with re-sizing the grid.
---------------------------------------------------------------------------------------
Road shader now is same for all road types and on junctions will colour the traffic lights in 'simple' locations
Memory problems with re-sizing grid still exist and better calculations for road shader mixing locations needed
---------------------------------------------------------------------------------------
