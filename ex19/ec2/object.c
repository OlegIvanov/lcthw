// At the top of each function, add asserts that make sure the input parameters are correct. 
// For example, in Object_new you want a assert(description != NULL).

// Go through each line of the function, and find any functions being called. 
// Read the documentation (man page) for that function, and confirm what it returns for an error. 
// Add another assert to check that the error didn't happen. 
// For example, in Object_new you need one after the call to calloc that does assert(el != NULL).

// If a function is expected to return a value, 
// either make sure it returns an error value (like NULL), 
// or have an assert to make sure that the returned variable isn't invalid. 
// For example, in Object_new, you need to have assert(el != NULL) again 
// before the last return since that part can never be NULL.

// For every if-statement you write, make sure there's an else clause unless 
// that if is an error check that causes an exit.

// For every switch-statement you write, make sure that 
// there's a default case that handles anything you didn't anticipate.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "object.h"
#include <assert.h>

void Object_destroy(void *self)
{
	Object *obj = self;

	if(obj) {
		if(obj->description) free(obj->description);
		free(obj);
	}
}

void Object_describe(void *self)
{
	assert(self);
	assert(((Object*)self)->description);

	Object *obj = self;
	printf("%s.\n", obj->description);
}

int Object_init(void *self)
{
	assert(self);

	// do nothing really
	return 1;
}

void *Object_move(void *self, Direction direction)
{
	assert(self);

	printf("You can't go that direction.\n");
	return NULL;
}

int Object_attack(void *self, int damage)
{
	assert(self);
	assert(damage >= 0);

	printf("You can't attack that.\n");
	return 0;
}

void *Object_new(size_t size, Object proto, char *description)
{
	assert(size >= 1);
	assert(description != NULL);

	// setup the default functions in case they aren't set
	if(!proto.init) proto.init = Object_init;
	if(!proto.describe) proto.describe = Object_describe;
	if(!proto.destroy) proto.destroy = Object_destroy;
	if(!proto.attack) proto.attack = Object_attack;
	if(!proto.move) proto.move = Object_move;

	// this seems weird, but we can make a struct of one size,
	// then point a different pointer at it to "cast" it
	Object *el = calloc(1, size);

	assert(el != NULL);
	
	*el = proto;

	// copy the description over
	el->description = strdup(description);

	assert(el->description);

	// initialize it with whatever init we were given
	if(!el->init(el)) {
		// looks like it didn't initialize properly
		el->destroy(el);
		return NULL;
	} else {
		assert(el);

		// all done, we made an object of any type
		return el;
	}
}
