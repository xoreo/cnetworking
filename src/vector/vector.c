#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "../chat.h"

void vector_init(Vector *vector) {
	// initialize size and capacity
	vector->size = 0;
	vector->capacity = VECTOR_INITIAL_CAPACITY;

	// allocate memory for vector->socket
	vector->client_sockets = malloc(sizeof(int) * vector->capacity);
}

void vector_append(Vector *vector, int value) {
	// make sure there's room to expand into
	vector_double_capacity_if_full(vector);

	// append the value and increment vector->size
	vector->client_sockets[vector->size++] = value;
}

int vector_get(Vector *vector, int index) {
	if (index >= vector->size || index < 0) {
		printf("Index %d out of bounds for vector of size %d\n", index, vector->size);
		exit(1);
	}
	return vector->client_sockets[index];
}

void vector_double_capacity_if_full(Vector *vector) {
	if (vector->size >= vector->capacity) {
		// double vector->capacity and resize the allocated memory accordingly
		vector->capacity *= 2;
		vector->client_sockets = realloc(vector->client_sockets, sizeof(int) * vector->capacity);
	}
}

void vector_free(Vector *vector) {
	free(vector->client_sockets);
}