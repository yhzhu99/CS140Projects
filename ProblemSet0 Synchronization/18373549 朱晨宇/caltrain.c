#include "pintos_thread.h"
struct station
{
	struct condition *arriveTrain;
	struct condition *gotrain;
	struct lock *someLock;
	int step1;
	int step2;
	int step3;
	int seat;
};
void station_init(struct station *station)
{
	station->arriveTrain=malloc(sizeof(struct condition));
	cond_init(station->arriveTrain);
	station->gotrain=malloc(sizeof(struct condition));
	cond_init(station->gotrain);
	station->someLock=malloc(sizeof(struct lock));
	lock_init(station->someLock);
	station->step1=0;
	station->step2=0;
	station->step3=0;
	station->seat=0;
	return;
}
void station_load_train(struct station *station, int count)
{
	lock_acquire(station->someLock);
	station->seat=count;
	station->step2=0;
	station->step3=0;
	cond_broadcast(station->arriveTrain,station->someLock);
	while (station->step3<station->seat&&(station->step1>0||station->step2>0))
	{
		cond_wait(station->gotrain,station->someLock);
	}
	station->seat=0;
	station->step3=0;
	lock_release(station->someLock);
	return;
}

void station_wait_for_train(struct station *station)
{
	lock_acquire(station->someLock);
	station->step1+=1;
	while (station->step2 + station->step3 >= station->seat)
		cond_wait(station->arriveTrain,station->someLock);
	station->step1-=1;
	station->step2+=1;
	lock_release(station->someLock);
	return;
}

void station_on_board(struct station *station)
{
	lock_acquire(station->someLock);
	station->step3+=1;
	station->step2-=1;
	cond_signal(station->gotrain,station->someLock);
	lock_release(station->someLock);
	return;
}