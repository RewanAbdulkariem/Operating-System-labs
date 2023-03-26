//
// Created by rewan on 3/26/23.
//

#ifndef LAB_3_CALTRAIN_H
#define LAB_3_CALTRAIN_H
#include <pthread.h>

struct station {
    // FILL ME IN
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);
#endif //LAB_3_CALTRAIN_H
