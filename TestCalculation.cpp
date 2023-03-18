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
    int pwm_value;
    double temp_hk = 66;

    double test;

    test = -4.41176;
    cout << test << endl;
    
    pwm_value = -4.41176*temp_hk + 357.35256;

    cout << pwm_value << endl;
}