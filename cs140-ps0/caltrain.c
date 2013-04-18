#include "pintos_thread.h"

struct station {
	struct lock lock;
	struct condition train_arrive;
	struct condition passanger_assigned;
	struct condition train_full;
	int waiting_passengers;
	int boarding_passengers;
};

void
station_init(struct station *station)
{
	station->waiting_passengers = 0;
	station->boarding_passengers = 0;
	lock_init(&(station->lock));
	cond_init(&(station->train_arrive));
	cond_init(&(station->passanger_assigned));
	cond_init(&(station->train_full));
}

void
station_load_train(struct station *station, int count)
{
	lock_acquire(&(station->lock));
	while ((station->waiting_passengers > 0) && (count > 0)) {
		cond_signal(&(station->train_arrive), &(station->lock));
		count--;
		//printf("Allow passenger to board. Seats remaining: %d\n", count);
		cond_wait(&(station->passanger_assigned), &(station->lock));
	} 
	if (station->boarding_passengers > 0)
		cond_wait(&(station->train_full), &(station->lock));
	lock_release(&(station->lock));
}

void
station_wait_for_train(struct station *station)
{
	lock_acquire(&(station->lock));
	station->waiting_passengers++;
	//printf("Waiting passangers: %d\n", station->waiting_passengers);
	cond_wait(&(station->train_arrive), &(station->lock));
	station->waiting_passengers--;
	station->boarding_passengers++;
	//printf("Waiting passangers: %d\n", station->waiting_passengers);
	cond_signal(&(station->passanger_assigned), &(station->lock));
	lock_release(&(station->lock));
}

void
station_on_board(struct station *station)
{
	//printf("\n*******************\nOn board\n");
	lock_acquire(&(station->lock));
	station->boarding_passengers--;
	if (station->boarding_passengers == 0)
	  cond_broadcast(&(station->train_full), &(station->lock));
	lock_release(&(station->lock));
}
	