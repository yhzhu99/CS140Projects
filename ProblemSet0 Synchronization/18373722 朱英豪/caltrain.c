#include "pintos_thread.h"
struct station {
    int waiting;
	int seats;
	int boarding;
	int boarded;
    struct lock lock;
    struct condition train_available;
    struct condition train_to_leave;
};

void
station_init(struct station *station)
{
    station->waiting = 0;
    station->seats = 0;
    station->boarding = 0;
    station->boarded = 0;
    lock_init(&station->lock);
    cond_init(&station->train_available);
    cond_init(&station->train_to_leave);
}

void
station_load_train(struct station *station, int count)
{
    lock_acquire(&station->lock);
    station->boarding = 0;
    station->boarded = 0;
    station->seats = count;
    cond_broadcast(&station->train_available, &station->lock);
    while (station->boarded < station->seats && (station->waiting > 0 || station->boarding > 0)){
        cond_wait(&station->train_to_leave, &station->lock);
	}
    station->seats = 0;
    station->boarded = 0;
    lock_release(&station->lock);
}

void
station_wait_for_train(struct station *station)
{
    lock_acquire(&station->lock);
    station->waiting++;
    while (station->boarding + station->boarded >= station->seats){
		cond_wait(&station->train_available, &station->lock);
	}
    station->boarding++;
    station->waiting--;
    lock_release(&station->lock);
}

void
station_on_board(struct station *station)
{
    lock_acquire(&station->lock);
    station->boarded++;
    station->boarding--;
    cond_signal(&station->train_to_leave, &station->lock);
    lock_release(&station->lock);
}