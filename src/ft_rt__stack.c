#include "ft_rt.h"

#include <stdlib.h>
#include <string.h>

#define DEFAULT_NODE_CAPACITY 8

typedef struct {
	void *resource;
	void (*dispose)(void *resource);
	bool transient;
} node;

typedef struct {
	ft_rt__stack expose;
	size_t count;
	size_t capacity;
	node *nodes;
} internal;

static err_t transient(
	struct _ft_rt__stack *self,
	void *resource,
	void (*dispose)(void *resource));
static err_t
	add(struct _ft_rt__stack *self,
		void *resource,
		void (*dispose)(void *resource));
static err_t ok(struct _ft_rt__stack *self);
static err_t unwind(struct _ft_rt__stack *self);

static err_t dummy_transient(
	struct _ft_rt__stack *self,
	void *resource,
	void (*dispose)(void *resource));
static err_t dummy_add(
	struct _ft_rt__stack *self,
	void *resource,
	void (*dispose)(void *resource));
static err_t dummy_ok(struct _ft_rt__stack *self);
static err_t dummy_unwind(struct _ft_rt__stack *self);

static const ft_rt__stack dummy = {
	dummy_transient,
	dummy_add,
	dummy_ok,
	dummy_unwind,
};

ft_rt__stack *ft_rt__stack__new(void) {
	internal *result = malloc(sizeof(internal));
	if (!result) {
		return (ft_rt__stack *)&dummy;
	}
	result->nodes = malloc(sizeof(node) * DEFAULT_NODE_CAPACITY);
	if (!result->nodes) {
		free(result);
		return (ft_rt__stack *)&dummy;
	}
	result->capacity = DEFAULT_NODE_CAPACITY;
	result->count = 0;
	*((ft_rt__stack__transient *)&result->expose.transient) = transient;
	*((ft_rt__stack__add *)&result->expose.add) = add;
	*((ft_rt__stack__ok *)&result->expose.ok) = ok;
	*((ft_rt__stack__unwind *)&result->expose.unwind) = unwind;
	return &result->expose;
}

static err_t transient(
	struct _ft_rt__stack *self,
	void *resource,
	void (*dispose)(void *resource)) {
	if (!resource) return true;
	internal *const this = (internal *)self;
	if (this->capacity == this->count) {
		node *const nodes = malloc(sizeof(node) * this->capacity * 2);
		if (!nodes) {
			dispose(resource);
			return true;
		}
		memcpy(nodes, this->nodes, sizeof(node) * this->capacity);
		free(this->nodes);
		this->nodes = nodes;
		this->capacity *= 2;
	}
	this->nodes[this->count++] = (node){ resource, dispose, true };
	return false;
}

static err_t
	add(struct _ft_rt__stack *self,
		void *resource,
		void (*dispose)(void *resource)) {
	if (!resource) return true;
	internal *const this = (internal *)self;
	if (this->capacity == this->count) {
		node *const nodes = malloc(sizeof(node) * this->capacity * 2);
		if (!nodes) {
			dispose(resource);
			return true;
		}
		memcpy(nodes, this->nodes, sizeof(node) * this->capacity);
		free(this->nodes);
		this->nodes = nodes;
		this->capacity *= 2;
	}
	this->nodes[this->count++] = (node){ resource, dispose, false };
	return false;
}

static err_t ok(struct _ft_rt__stack *self) {
	internal *const this = (internal *)self;
	for (size_t i = this->count; i; i--)
		if (this->nodes[i - 1].transient)
			this->nodes[i - 1].dispose(this->nodes[i - 1].resource);
	free(this->nodes);
	free(self);
	return false;
}

static err_t unwind(struct _ft_rt__stack *self) {
	internal *const this = (internal *)self;
	for (size_t i = this->count; i; i--)
		this->nodes[i - 1].dispose(this->nodes[i - 1].resource);
	free(this->nodes);
	free(self);
	return true;
}

static err_t dummy_transient(
	struct _ft_rt__stack *self,
	void *resource,
	void (*dispose)(void *resource)) {
	(void)self;
	dispose(resource);
	return true;
}

static err_t dummy_add(
	struct _ft_rt__stack *self,
	void *resource,
	void (*dispose)(void *resource)) {
	(void)self;
	dispose(resource);
	return true;
}

static err_t dummy_ok(struct _ft_rt__stack *self) {
	(void)self;
	return false;
}

static err_t dummy_unwind(struct _ft_rt__stack *self) {
	(void)self;
	return true;
}
