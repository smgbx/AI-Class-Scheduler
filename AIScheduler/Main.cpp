#include "Schedule.h"
#include <iostream>
#include <time.h>
#include <math.h>
#include <algorithm>
using namespace std;

int main() {
	srand(time(NULL));

	//do {
	//	Schedule test = Schedule();
	//	test.printSchedule();
	//	cout << test.getFitness() << endl;
	//	system("pause");
	//} while (true);

	do {
		Schedule initialSchedule = Schedule();

		Schedule solutionSchedule = initialSchedule;
		
		cout << "Initial schedule: " << endl;
		initialSchedule.printSchedule();
		cout << "Initial fitness: " << initialSchedule.getFitness() << endl;

		double T = 15;
		int attempts = 0;
		int changes = 0;

		Schedule currentSchedule = initialSchedule;

		while (attempts < 4000) {
			attempts += 1;

			Schedule nextSchedule = Schedule(currentSchedule.getSchedule());
			int deltaEnergy = nextSchedule.getFitness() - currentSchedule.getFitness();
			float random = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			
			if (deltaEnergy > 0) {
				currentSchedule = nextSchedule;
				changes += 1;
			}

			else if (exp((-deltaEnergy) / T) <= random) {
				currentSchedule = nextSchedule;
				changes += 1;
			}

			solutionSchedule = currentSchedule;

			if (changes >= 400 || (attempts == 4000 && changes != 0)) {
				if (changes >= 400) {
					cout << "400 changes made. Lowering temp." << endl;
				}
				else {
					cout << "4000 attempts and " << changes << " changes made. Lowering temp." << endl;
				}
				cout << "Current best fitness: " << solutionSchedule.getFitness() << endl;

				T *= 0.99;
				changes = 0;
				attempts = 0;
			}
			
		}

		cout << "4000 attempts made without changes. Stopping." << endl;
		solutionSchedule.printSchedule();
		cout << "Score: " << solutionSchedule.getFitness() << endl;
		cout << "Difference: " << solutionSchedule.getFitness() - initialSchedule.getFitness() << endl;
		cout << "Temperature: " << T << endl;
		system("pause");

	} while (true);

	return 0;
}
