#include "pintos_thread.h"

struct station {
    int seats;
    int boarding;
    int waiting;

    struct condition pass_queue;
    struct condition train_queue;
    struct lock lock;
};

void
station_init(struct station *station)
{
    station->boarding = 0;
    station->waiting = 0;
    station->seats = 0;

    cond_init(&station->pass_queue);
    cond_init(&station->train_queue);
    lock_init(&station->lock);
}

void
station_load_train(struct station *station, int count)
{
	lock_acquire(&station->lock);
    station->seats = count;

    while ((station->seats > 0 && station->waiting > 0) || station->boarding > 0) {
        cond_broadcast(&station->train_queue, &station->lock);
        cond_wait(&station->pass_queue, &station->lock);
    }

    station->seats = 0;
    lock_release(&station->lock);
}

void
station_wait_for_train(struct station *station)
{
    lock_acquire(&station->lock);
    station->waiting += 1;

    while (station->seats == 0)
        cond_wait(&station->train_queue, &station->lock);

    station->boarding += 1;
    station->seats -= 1;
    station->waiting -= 1;

    lock_release(&station->lock);
}

void
station_on_board(struct station *station)
{
	lock_acquire(&station->lock);
    station->boarding -= 1;

    if ((station->seats == 0 || station->waiting == 0) && station->boarding == 0)
        cond_signal(&station->pass_queue, &station->lock);

    lock_release(&station->lock);
}