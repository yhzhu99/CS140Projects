#include "pintos_thread.h"

// Forward declaration. This function is implemented in reaction-runner.c,
// but you needn't care what it does. Just be sure it's called when
// appropriate within reaction_o()/reaction_h().
void make_water();

struct reaction {
	// FILL ME IN
	int num_h;
	struct condition queue_1;
	struct condition queue_o;
	struct lock lock;
};

void
reaction_init(struct reaction *reaction)
{
	// FILL ME IN
	reaction->num_h = 0;
	cond_init(&reaction->queue_1);
	cond_init(&reaction->queue_o);
	lock_init(&reaction->lock);
}

void
reaction_h(struct reaction *reaction)
{
	// FILL ME IN
	lock_acquire(&reaction->lock);
	reaction->num_h += 1;
	cond_signal(&reaction->queue_1, &reaction->lock);
	cond_wait(&reaction->queue_o, &reaction->lock);
	lock_release(&reaction->lock);

}

void
reaction_o(struct reaction *reaction)
{
	// FILL ME IN
	lock_acquire(&reaction->lock);
	while(reaction->num_h <= 1) {
		cond_wait(&reaction->queue_1, &reaction->lock);
	}
	make_water();
	reaction->num_h -= 2;
	cond_signal(&reaction->queue_o, &reaction->lock);
	cond_signal(&reaction->queue_o, &reaction->lock);
	lock_release(&reaction->lock);
}
