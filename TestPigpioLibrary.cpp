#include <pigpio.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <math.h>

using namespace std;

int main(int argc, char **argv)
{
    gpioSetMode(17, PI_OUTPUT);
    gpioSetMode(27, PI_OUTPUT);

    gpioWrite(17, 1);
    cout << gpioRead(17) << endl;

    gpioWrite(27, 0);
    cout << gpioRead(27) << endl;
}