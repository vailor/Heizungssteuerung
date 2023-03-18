#include <pigpio.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <ctime>
#include <chrono>
#include <sstream>

//digitemp_DS9097  -t 0  -q -o 2 | awk 'NF>1 {OFS=""; $1=""; print}'
//Befehle: gpio -g write 27 1
//Befehle: gpio -g write 17 1
//Build command: g++ -Wall -pthread -o PumpeAnAus PumpeAnAus.cpp -lpigpiod -lrt

using namespace std;


void actPumpe(int i){
    if(gpioRead(17) == i)
	{
	}
	else
	{
        gpioWrite(17, i);
		ofstream pumplog;
		time_t now;
		time(&now);
		pumplog.open("tmp/pumplog", ios::out | ios_base::app);
		pumplog << ctime(&now) << "Pumperelais wurde auf " << i << " gesetzt" << endl;
		pumplog.close();
	}
}

void actVentil(int i){
    if(gpioRead(27) == i)
	{
    
    }
	else
	{
        //digitalWrite(2, i);
        gpioWrite(27, i);
        ofstream ventillog;
		time_t now;
		time(&now);
        ventillog.open("tmp/ventillog", ios::out | ios_base::app);
        ventillog << ctime(&now) << "Ventilrelais wurde auf " << i << " gesetzt" << endl;
        ventillog.close();
    }

}

void SetSolarCell(void){

	string time;
	int time_int;

	auto start = chrono::system_clock::now();
	// Some computation here
	auto end = chrono::system_clock::now();

	chrono::duration<double> elapsed_seconds = end - start;
	time_t end_time = chrono::system_clock::to_time_t(end);

	time = ctime(&end_time);		// Prints the current time in Wochentag/Monat/Tag hh:mm:ss Jahr
	time = time.substr(11, 2);		// cuts out the hour
	time_int = stoi(time);			// Converts time string to integer
	
	if(time_int <= 12){					// if-statement checks value of time_int and writes GPIO_22
		gpioWrite(22, 0);
	}
	else{
		gpioWrite(22, 1);
	}
}


int main(int argc, char **argv)
{
    if(gpioInitialise() < 0)
    {
	cout << "Test failed";
        	return 1;
    }

    gpioSetMode(17, PI_OUTPUT);
    gpioSetMode(27, PI_OUTPUT);
    gpioSetMode(18, PI_ALT5);

	ifstream heizkessel;
	ifstream puffer_mitte;
	ifstream puffer_unten;
	ifstream aussen;
	
    gpioSetPWMrange(18, 100);

	heizkessel.open("ramtmp_ram/heizkessel", ios::in);
	puffer_mitte.open("ramtmp_ram/puffer_mitte", ios::in);
	puffer_unten.open("ramtmp_ram/puffer_unten", ios::in);
	aussen.open("ramtmp_ram/aussen", ios::in);

/********************** Reads Pumpe and Oelofen status START **********************/
	ofstream pumpe;
	ofstream oelofen;
	pumpe.open("ramtmp_ram/pumpe", ios::in);
	oelofen.open("ramtmp_ram/oelofen", ios::in);

	pumpe << gpioRead(17);
	oelofen << gpioRead(27);

	pumpe.close();
	oelofen.close();
/********************** Reads Pumpe and Oelofen status END **********************/

	SetSolarCell(); //Set GPIO_22

	if(heizkessel)
	{
		double temp_hk;
		double puff_unten;
		double temp_diff;
		int pwm_value;  // pwm_value = -(75/17)*temp_hk + 371,62

		heizkessel >> temp_hk;
		puffer_unten >> puff_unten;
		temp_diff = puff_unten + 6;

		cout << temp_hk << endl;
		cout << puff_unten << endl;
		cout << temp_diff << endl;

		if((temp_hk > 50)&&(temp_hk != 85))
		{
			if((temp_hk > 65) && (temp_hk < 82))
			{
				if(temp_diff <= temp_hk)
				{
					pwm_value = -4.41176*temp_hk + 357.35256;
					actPumpe(0);
                    gpioPWM(18, pwm_value);
				}
				else if(temp_diff > temp_hk)
				{
					gpioPWM(18, 100);
				}
			}
			if(temp_hk <= 64)
			{
				actPumpe(1);
			}
			if(temp_hk >= 82)
			{
				actPumpe(0);
				gpioPWM(18, 0);
			}

			heizkessel.close();
			puffer_unten.close();
		} /* End if((temp_hk > 50)&&(temp_hk < 100)&&(temp_hk != 85)) */
	} /* End if(heizkessel) */

	if(puffer_mitte)
	{
		double puff_mitte;
		double aus;
		double oel_aussen;
		double oel_vorlauf;

		puffer_mitte >> puff_mitte;
		aussen >> aus;

		oel_aussen = aus - 3;

//		cout << puff_mitte << endl;
//		cout << aus << endl;
		cout << oel_aussen << endl;


		if((15 < puff_mitte)&&(puff_mitte < 80))
		{
			oel_vorlauf = 20.0 + 27 * pow(((20.0 - oel_aussen) * 0.05) , 0.8);

			ofstream vorlauf;
			vorlauf.open("ramtmp_ram/vorlauf", ios::out | ios::trunc);
			vorlauf << oel_vorlauf<<endl;
			vorlauf.close();

			if(puff_mitte >= oel_vorlauf)
			{
				actVentil(0);
				cout << oel_vorlauf << endl;
			}
			else
			if(puff_mitte < oel_vorlauf)
			{
				actVentil(1);
			}
		}
		puffer_mitte.close();
		aussen.close();
	}

	gpioTerminate(); // Terminates the library. Resets the used DMA channels, releases memory and terminates any running threads.
}
