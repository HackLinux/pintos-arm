#include "pintos_thread.h"

struct station {
	struct lock lock;
	struct condition cond;
	int waiting_passengers;
};

void
station_init(struct station *station)
{
	station->waiting_passengers = 0;
	lock_init(&(station->lock));
	cond_init(&(station->cond));
}

void
station_load_train(struct station *station, int count)
{
	lock_acquire(&(station->lock));
	while ((station->waiting_passengers > 0) && (count > 0)) {
		cond_signal(&(station->cond), &(station->lock));
		count--;
		printf("Allow passenger to board. Seats remaining: %d\n", count);
	} 
	lock_release(&(station->lock));
}

void
station_wait_for_train(struct station *station)
{
	lock_acquire(&(station->lock));
	station->waiting_passengers++;
	printf("Waiting passangers: %d\n", station->waiting_passengers);
	cond_wait(&(station->cond), &(station->lock));
	station->waiting_passengers--;
	printf("Waiting passangers: %d\n", station->waiting_passengers);
	lock_release(&(station->lock));
}

void
station_on_board(struct station *station)
{
	//lock_acquire(&(station->lock));
	printf("On board");
	//lock_release(&(station->lock));
}
