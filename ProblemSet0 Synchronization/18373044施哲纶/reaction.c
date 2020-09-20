#include "pintos_thread.h"

void make_water();

struct reaction {
	struct lock *thread_lock;
	int H_num;
	struct condition *new_H;
	struct condition *release_H;
};

void
reaction_init(struct reaction *reaction)
{
	reaction->thread_lock = malloc(sizeof(struct lock));
	reaction->new_H = malloc(sizeof(struct condition));
	reaction->release_H = malloc(sizeof(struct condition));
	reaction->H_num = 0;
	lock_init(reaction->thread_lock);
	cond_init(reaction->new_H);
	cond_init(reaction->release_H);
	reaction->new_H=0;
}

void
reaction_h(struct reaction *reaction)
{
	lock_acquire(reaction->thread_lock);
	reaction->H_num++;
	cond_signal(reaction->new_H,reaction->thread_lock);
	cond_wait(reaction->release_H,reaction->thread_lock);
	lock_release(reaction->thread_lock);
}

void
reaction_o(struct reaction *reaction)
{
	lock_acquire(reaction->thread_lock);
	while(reaction->H_num<2){
		cond_wait(reaction->new_H,reaction->thread_lock);
	}
	make_water();
	reaction->H_num-=2;
	cond_signal(reaction->release_H,reaction->thread_lock);
	cond_signal(reaction->release_H,reaction->thread_lock);
	lock_release(reaction->thread_lock);
}
