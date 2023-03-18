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
    if(gpioInitialise() < 0)
    {
        	return 1;
    }

	int x = 0;

	if(gpioSetMode(18, PI_ALT5) == 0){
		cout << "gpioSetMode OK!" << endl;
	}
	if(gpioSetPWMrange(18, 100) == PI_BAD_USER_GPIO){
		cout << "gpioSetPWMrange not working" << endl;
	}
	else{
		cout << "gpioSetPWMrange OK!" << endl;
	}

	while(x <= 2){
		gpioPWM(18, 100);
		cout << "Drücke Enter um Pumpe schneller laufen zu lassen";
		cin >> x;

gpioPWM(18,80);
cout << "Drücke Enter um Pumpe schneller laufen zu lassen";
cin >> x;

gpioPWM(18,60);
cout << "Drücke Enter um Pumpe schneller laufen zu lassen";
cin >> x;

gpioPWM(18,40);
cout << "Drücke Enter um Pumpe schneller laufen zu lassen";
cin >> x;

gpioPWM(18,20);
cout << "Drücke Enter um Pumpe schneller laufen zu lassen";
cin >> x;

gpioPWM(18,0);
cout << "Drücke Enter um Pumpe schneller laufen zu lassen";
cin >> x;

x++;
	}
}
