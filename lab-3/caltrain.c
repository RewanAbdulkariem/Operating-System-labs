#include <pthread.h>
#include "caltrain.h"
/**
 * station_init - initialize the station object
 * @station:
 */
void station_init(struct station *station)
{
	// FILL ME IN
}
/**
 * station_load_train - When a train arrives in the station
 * and has opened its doors, it invokes the function
  * The function must not return until the train is satisfactorily loaded
 * @count: indicates how many seats are available on the train.
 * @station:
 */
void station_load_train(struct station *station, int count)
{
	// FILL ME IN
}
/**
 * station_wait_for_train - When a passenger robot arrives in a station
 * it first invokes the function
 * This function must not return until a train is in the station and
 * there are enough free seats on the train for this passenger to sit down
 * @station:
 */
void station_wait_for_train(struct station *station)
{
	// FILL ME IN
}
/**
 * station_on_board - Once the passenger is seated, it will call the function
 * @station:
 */
void station_on_board(struct station *station)
{
	// FILL ME IN
}
