#include <iostream>
using namespace std;

void Target_speed_distance(double frequency);
void Targetnotmoving();

#include <iostream>
using namespace std;
#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <cmath>


void Target_speed_distance(double frequency);
void Targetnotmoving();

int main()
{
	long long frequency;

	cout << "Enter frequency (Hz): ";
	cin >> frequency;

	if (frequency > 0) {
		Target_speed_distance(frequency);
	} else {
		Targetnotmoving();
	}

	return 0;
}


void Target_speed_distance(double frequency);
void Targetnotmoving();

#endif

const double c = 3e8;
const double wavelength = 0.5; // 0.5 meters //
void Targetnotmoving()
{
	cout << "Target is not moving, location and speed not found" << endl;
}

void Target_speed_distance(double frequency)
{
	double speed = 0.0;
	double distance = 0.0;

	double time_delay = 0.001; // 1 ms

	speed = (wavelength * frequency) / 2;

	distance = (c * time_delay) / 2;


	if (frequency >= 3 && frequency <= 3e10) // X-band
	{
		cout << "Target detected." << endl;
	}
	else
	{
		cout << "Frequency range not supported." << endl;
	}

	cout << "Speed: " << speed << " m/s" << endl;
	cout << "Distance: " << distance << " meters" << endl;


	double xcoordinate = distance * cos(speed);
	double ycoordinate = distance * sin(speed);
	cout << "Location: (" << xcoordinate << ", " << ycoordinate << ")" << endl;
}