/* COMP2004-W2 - Solomon Cammack (solo) - 10613265 */

#ifndef MBED_SLMN_NETWORK
#define MBED_SLMN_NETWORK
#include "mbed.h"

void network_main();
int send_request(char method[], char url[], char headers[], char data[]);
int send_data(string data);

#endif