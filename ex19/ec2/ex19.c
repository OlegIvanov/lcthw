// Put the game mechanics into a third file, compile it to .o, 
// and then use that to write another little game. 
// If you're doing it right you should only have a new Map and a main function in the new game.

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "ex19.h"

extern Object MonsterProto;
extern Object RoomProto;
extern Object MapProto;

int Map_init(void *self)
{
	assert(self);

	Map *map = self;
	
	// make some room for a small map
	Room *hall = NEW(Room, "The great Hall");
	Room *throne = NEW(Room, "The throne room");
	Room *arena = NEW(Room, "The arena, with the minotaur");
	Room *kitchen = NEW(Room,"Kitchen, you have the knife now");

	Room *pantry = NEW(Room, "Pantry, eat something unless goddamned monsters have eaten everything");
	Room *bedroom = NEW(Room, "Bedroom, you've successfully banged monster's daughters");
	Room *toilet = NEW(Room, "Toilet, you can finally take a crap");

	// put the bad guy in the arena
	arena->bad_guy = NEW(Monster, "The evil minotaur");

	// put the bad guy in the bedroom
	bedroom->bad_guy = NEW(Monster, "Troll for fucking in the asshole");

	// setup the map rooms
	hall->north = throne;
	
	throne->west = arena;
	throne->east = kitchen;
	throne->south = hall;

	arena->east = throne;
	arena->north = pantry;

	kitchen->west = throne;
	kitchen->north = bedroom;

	pantry->south = arena;

	bedroom->south = kitchen;
	bedroom->east = toilet;

	toilet->west = bedroom;

	// start the map and the character off in the hall
	map->start = hall;
	map->location = hall;

	return 1;
}

int main(int argc, char *argv[])
{
	// simple way to setup the randomness
	srand(time(NULL));
	
	// prepare Map_init function for NEW below
	MapProto.init = Map_init;

	// make our map to work with
	Map *game = NEW(Map, "The Hall of the Minotaur.");
	
	printf("You enter the ");
	game->location->_(describe)(game->location);

	while(process_input(game)) {}

	return 0;
}
