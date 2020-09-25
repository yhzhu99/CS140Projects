#include "pintos_thread.h"

// Forward declaration. This function is implemented in reaction-runner.c,
// but you needn't care what it does. Just be sure it's called when
// appropriate within reaction_o()/reaction_h().
void make_water();

struct reaction {
	// FILL ME IN
	struct lock *someLock;
	int h_num;
	struct condition *new_h;
	struct condition *cur_h;
};

void
reaction_init(struct reaction *reaction)
{
	reaction->someLock = malloc(sizeof(struct lock));
	reaction->new_h = malloc(sizeof(struct condition));
	reaction->cur_h = malloc(sizeof(struct condition));
	reaction->h_num = 0;
	lock_init(reaction->someLock);
	cond_init(reaction->new_h);
	cond_init(reaction->cur_h);
	// FILL ME IN
}

void
reaction_h(struct reaction *reaction)
{
	lock_acquire(reaction->someLock);
	reaction->h_num+=1;
	cond_signal(reaction->new_h,reaction->someLock);
	cond_wait(reaction->cur_h,reaction->someLock);
	lock_release(reaction->someLock);
}

void
reaction_o(struct reaction *reaction)
{
	lock_acquire(reaction->someLock);
	while(reaction->h_num<=1)
	{
		cond_wait(reaction->new_h,reaction->someLock);
	}

	make_water();
	
	reaction->h_num-=2;
	cond_signal(reaction->cur_h,reaction->someLock);
	cond_signal(reaction->cur_h,reaction->someLock);
	lock_release(reaction->someLock);
	// FILL ME IN
}