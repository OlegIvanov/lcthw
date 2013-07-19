// Go research "stack data structure" and write one in your favorite language, then try to do it in C.

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

struct Stack {
	int capacity;
	int *top;
	int *elements;
};

void Stack_destruct(struct Stack *st)
{
	if(st) {
		if(st->elements) {
			free(st->elements);
		}
		free(st);
	}
}

void die(const char *message)
{
	if(errno) {
		perror(message);
	} else {
		printf("ERROR: %s\n", message);
	}

	exit(1);
}

struct Stack *Stack_create(int capacity)
{
	struct Stack *st = malloc(sizeof(struct Stack));
	if(!st) die("Memory error.");

	st->capacity = capacity;

	st->elements = malloc(st->capacity * sizeof(int));
	if(!st->elements) die("Memory error.");

	st->top = st->elements + st->capacity;

	return st;
}

int Stack_push(struct Stack *st, int push_value)
{
	if((st->top - st->elements - 1) >= 0) {
		*(--st->top) = push_value;
		return 0;
	}
	
	return 1;
}

int Stack_pop(struct Stack *st, int *pop_value)
{	
	if(st->top < st->elements + st->capacity) {
		*pop_value = *(st->top++);
		return 0;
	}
	
	return 1;
}

int main(int argc, char **argv)
{
	int stack_capacity;

	if(argc < 2) {
		die("Stack capacity hasn't been specified as parameter.");
	} else {
		stack_capacity = atoi(*(argv + 1));
	}

	if (stack_capacity < 1) die("Stack size is too small.");

	struct Stack *st = Stack_create(stack_capacity);

	int push_value;
	int pop_value;
	
	push_value = 100;
	if(Stack_push(st, push_value)){
		printf("Stack overflow.\n");
	} else {
		printf("%d has been successfully pushed.\n", push_value);
	};
	
	push_value = 10;
	if(Stack_push(st, push_value)){
		printf("Stack overflow.\n");
	} else {
		printf("%d has been successfully pushed.\n", push_value);
	};

	push_value = 1;
	if(Stack_push(st, push_value)){
		printf("Stack overflow.\n");
	} else {
		printf("%d has been successfully pushed.\n", push_value);
	};

	if(Stack_pop(st, &pop_value)) {
		printf("Stack is empty.\n");	
	} else {
		printf("%d has been successfully popped.\n", pop_value);
	};

	if(Stack_pop(st, &pop_value)) {
		printf("Stack is empty.\n");	
	} else {
		printf("%d has been successfully popped.\n", pop_value);
	};

	if(Stack_pop(st, &pop_value)) {
		printf("Stack is empty.\n");	
	} else {
		printf("%d has been successfully popped.\n", pop_value);
	};

	Stack_destruct(st);

	return 0;
}
