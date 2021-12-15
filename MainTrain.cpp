/*
 * MainTrain.cpp
 *
 *  Created on: 11 áàå÷× 2020
 *      Author: Eli
 */

#include <iostream>
#include <vector>
#include "HybridAnomalyDetector.h"
#include "CLI.h"
#include <fstream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <math.h>

using namespace std;

int main(){
    CLI* cli = new CLI(new StandardIO());
    cli->start();
    return 0;
}