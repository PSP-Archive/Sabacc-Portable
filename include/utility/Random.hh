#ifndef __SMF_RANDOM_HH
#define __SMF_RANDOM_HH

/*
 * Sabacc Media Framework
 *
 * Author: goumba
 * Date: August 26, 2008
 *
 * File: Random.hh
 *
 * A random integer generator
 */

#include <time.h>

class Random {
public:
    Random() {
        srand(time(0));
    }

    int operator()(int ceiling) {
        // return(static_cast<int>(((0.0 + ceiling) * rand()) / (RAND_MAX + 1.0)));
        return(static_cast<int>(rand() % (ceiling + 1)));
    }
};	// class random

#endif	// __SMF_RANDOM_HH
