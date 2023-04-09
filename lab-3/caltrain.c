#include <pthread.h>
#include <stdio.h>
#include "caltrain.h"
/**
 * station_init - initialize the station object
 */
void station_init(struct station *station)
{
	// FILL ME IN
    station->WaitingPassengers = 0;
    station->OnBoardPassengers = 0;
    station->FreeSeats = 0;
    pthread_mutex_init(&station->MutexLock, NULL);
    pthread_cond_init(&station->TrainArrive, NULL);
    pthread_cond_init(&station->TrainLeft, NULL);
}
/**
 * station_load_train - When a train arrives in the station
 * and has opened its doors, it invokes the function
  * The function must not return until the train is satisfactorily loaded
 * @count: indicates how many seats are available on the train.
 */
void station_load_train(struct station *station, int count)
{
	// FILL ME IN
    /** If there are no available seats or no waiting passengers, return */
    if (count == 0 || station->WaitingPassengers == 0)
        return;
    pthread_mutex_lock(&station->MutexLock);
    station->FreeSeats = count;             /** update the number of free seats on the train */
    /** wait until all passengers are on board or there are no more free seats */
    if (station->WaitingPassengers > 0 && station->FreeSeats > 0)
    {
        pthread_cond_broadcast(&station->TrainArrive);
        pthread_cond_wait(&station->TrainLeft, &station->MutexLock);
    }

    pthread_mutex_unlock(&station->MutexLock);
}
/**
 * station_wait_for_train - When a passenger robot arrives in a station
 * it first invokes the function
 * This function must not return until a train is in the station and
 * there are enough free seats on the train for this passenger to sit down
 */
void station_wait_for_train(struct station *station)
{
	// FILL ME IN
    pthread_mutex_lock(&station->MutexLock);
    station->WaitingPassengers ++;
    /** Wait until a train arrives and there are free seats */
    while (station->FreeSeats == 0)
    {
        pthread_cond_wait(&station->TrainArrive, &station->MutexLock);
    }
    station->FreeSeats--;
    station->OnBoardPassengers ++;
    station->WaitingPassengers--;
    pthread_mutex_unlock(&station->MutexLock);
}
/**
 * station_on_board - Once the passenger is seated, it will call the function
 */
void station_on_board(struct station *station)
{
	// FILL ME IN
    pthread_mutex_lock(&station->MutexLock);
    station->OnBoardPassengers--;
    /** If there are no more waiting passengers or free seats and all passengers are on board, signal that the train has left */
    if ((station->WaitingPassengers == 0 || station->FreeSeats == 0) && station->OnBoardPassengers == 0)
    {
        /** Signal that the passenger has boarded the train */
        pthread_cond_signal(&station->TrainLeft);
    }
    pthread_mutex_unlock(&station->MutexLock);
}
