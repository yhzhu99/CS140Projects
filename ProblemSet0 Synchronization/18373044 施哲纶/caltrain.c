#include "pintos_thread.h"
struct station
{
	struct condition *train_arrived;
	struct condition *train_loaded;
	struct lock *thread_lock;
	int waiting_passenger;
	int empty_seat;
	int ready_to_seat;
};
void station_init(struct station *station)
{
	station->train_arrived = malloc(sizeof(struct condition));
	station->train_loaded = malloc(sizeof(struct condition));
	station->thread_lock = malloc(sizeof(struct lock));
	cond_init(station->train_arrived);
	cond_init(station->train_loaded);
	lock_init(station->thread_lock);
	station->ready_to_seat = 0;
	station->waiting_passenger = 0;
	station->empty_seat = 0;
}
void station_load_train(struct station *station, int count)
{
	lock_acquire(station->thread_lock);
	station->empty_seat = count;
	while (station->empty_seat > 0 && station->waiting_passenger > 0)
	{
		cond_broadcast(station->train_arrived, station->thread_lock);
		cond_wait(station->train_loaded, station->thread_lock);
	}
	station->empty_seat = 0;
	lock_release(station->thread_lock);
	return;
}

void station_wait_for_train(struct station *station)
{
	lock_acquire(station->thread_lock);
	station->waiting_passenger++;
	while (station->empty_seat==0||station->empty_seat<=station->ready_to_seat)
	{
		cond_wait(station->train_arrived, station->thread_lock);
	}
	station->ready_to_seat++;
	lock_release(station->thread_lock);
}

void station_on_board(struct station *station)
{
	lock_acquire(station->thread_lock);
	station->waiting_passenger--;
	station->empty_seat--;
	station->ready_to_seat--;
	cond_signal(station->train_loaded, station->thread_lock);
	lock_release(station->thread_lock);
}