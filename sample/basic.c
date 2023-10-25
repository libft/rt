#include "ft_rt.h"

#include <stdlib.h>

err_t basic(void **out) {
	ft_rt__stack *const s = ft_rt__stack__new();
	void *a, *b, *c;
	if (false || s->transient(s, a = malloc(1), free)
		|| s->add(s, b = malloc(42), free)
		|| s->transient(s, c = malloc(42), free))
		return s->unwind(s);
	*out = b;
	return s->ok(s);
}
