#include "pintos_thread.h"

// Forward declaration. This function is implemented in reaction-runner.c,
// but you needn't care what it does. Just be sure it's called when
// appropriate within reaction_o()/reaction_h().
void make_water();

struct reaction {
    int needed_h, used_h;
    struct lock lock;
    struct condition now_o;
    struct condition now_h;
};

void
reaction_init(struct reaction *reaction)
{
    reaction->needed_h = 0;
    reaction->used_h = 0;
    lock_init(&reaction->lock);
    cond_init(&reaction->now_o);
    cond_init(&reaction->now_h);
}

void
reaction_h(struct reaction *reaction)
{
    lock_acquire(&reaction->lock);
    reaction->needed_h++;
    cond_signal(&reaction->now_h, &reaction->lock);
    while (reaction->used_h == 0){
		cond_wait(&reaction->now_o, &reaction->lock);
	}
    reaction->used_h--;
    lock_release(&reaction->lock);
}

void
reaction_o(struct reaction *reaction)
{
    lock_acquire(&reaction->lock);
    while (reaction->needed_h < 2){
		cond_wait(&reaction->now_h, &reaction->lock);
	}
    reaction->used_h += 2;
    reaction->needed_h -= 2;
    cond_broadcast(&reaction->now_o, &reaction->lock);
    make_water();
    lock_release(&reaction->lock);
}