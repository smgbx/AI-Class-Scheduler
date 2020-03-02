/*
* Shelby Mohar
* CS 461 - Intro to Artificial Intelligence
* 1 March 2020
*
* A program designed to find an acceptably optimal schedule using simulated annealing. 
*/

#include "Schedule.h"
#include <iostream>
#include <time.h>
#include <math.h>
#include <algorithm>
using namespace std;

int main() {
	srand(time(NULL));

	do {
		Schedule currentSchedule = Schedule();
		
		double T = 100;
		int attempts = 0;
		int changes = 0;

		while (attempts < 4000) {
			attempts += 1;

			Schedule nextSchedule = Schedule(currentSchedule.getSchedule());
			int deltaEnergy = nextSchedule.getFitness() - currentSchedule.getFitness();
			float random = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			
			if (deltaEnergy > 0) {
				currentSchedule = nextSchedule;
				changes += 1;
			}
			else if (exp((deltaEnergy) / T) >= random) {
				currentSchedule = nextSchedule;
			}

			if (changes == 400 || (attempts == 4000 && changes != 0)) {
				T *= 0.95;
				changes = 0;
				attempts = 0;
			}
		}

		cout << "Score: " << currentSchedule.getFitness() << endl;
		cout << endl;

		currentSchedule.printSchedule();
		cout << endl;

		system("pause");

	} while (true);

	return 0;
}
