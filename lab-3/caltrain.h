#include <pthread.h>

struct station {
	// FILL ME IN
    pthread_mutex_t MutexLock;
    pthread_cond_t TrainLeft;
    pthread_cond_t TrainArrive;
    int WaitingPassengers;
    int OnBoardPassengers;
    int FreeSeats;
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);