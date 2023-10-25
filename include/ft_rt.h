#pragma once

#include <stdbool.h>

typedef bool err_t;

#define NONNULL
#define NULLABLE

typedef struct {
	bool success;
	void *disposable;
} ft_rt__resource_result;

struct _ft_rt__stack;

typedef err_t (*NONNULL ft_rt__stack__transient)(
	struct _ft_rt__stack *NONNULL self,
	void *NULLABLE resource,
	void (*NONNULL dispose)(void *resource));

typedef err_t (*NONNULL ft_rt__stack__add)(
	struct _ft_rt__stack *NONNULL self,
	void *NULLABLE resource,
	void (*NONNULL dispose)(void *resource));

typedef err_t (*NONNULL ft_rt__stack__ok)(struct _ft_rt__stack *NONNULL self);

typedef err_t (*NONNULL ft_rt__stack__unwind)(
	struct _ft_rt__stack *NONNULL self);

typedef struct _ft_rt__stack {
	const ft_rt__stack__transient transient;
	const ft_rt__stack__add add;
	const ft_rt__stack__ok ok;
	const ft_rt__stack__unwind unwind;
} ft_rt__stack;

ft_rt__stack *NONNULL ft_rt__stack__new(void);
