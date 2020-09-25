#include "pintos_thread.h"
struct station
{
	struct condition *arriveTrain;
	struct condition *gotrain;
	struct lock *someLock;
	int passengerNum;
	int emptySeat;
	int occupiedSeat;
};
void station_init(struct station *station)
{
	station->arriveTrain=malloc(sizeof(struct condition));
	cond_init(station->arriveTrain);
	station->gotrain=malloc(sizeof(struct condition));
	cond_init(station->gotrain);
	station->someLock=malloc(sizeof(struct lock));
	lock_init(station->someLock);
	station->occupiedSeat=0;
	station->passengerNum=0;
	station->emptySeat=0;
}
void station_load_train(struct station *station, int count)
{
	lock_acquire(station->someLock);
	station->emptySeat=count;
	while (station->emptySeat > 0 && station->passengerNum > 0)
	{
		cond_broadcast(station->arriveTrain, station->someLock);
		cond_wait(station->gotrain, station->someLock);
	}
	lock_release(station->someLock);
	return;
}

void station_wait_for_train(struct station *station)
{
	lock_acquire(station->someLock);
	station->passengerNum++;
	while (station->emptySeat<=station->occupiedSeat)
	{
		cond_wait(station->arriveTrain,station->someLock);
	}
	station->passengerNum--;
	station->occupiedSeat++;
	lock_release(station->someLock);
}

void station_on_board(struct station *station)
{
	lock_acquire(station->someLock);
	station->occupiedSeat--;
	station->emptySeat--;
	cond_signal(station->gotrain, station->someLock);
	lock_release(station->someLock);
}