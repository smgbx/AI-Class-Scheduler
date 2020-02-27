#include "Schedule.h"
#include <iostream>
#include <time.h>
using namespace std;

int main() {
	srand(time(NULL));

	Schedule test = Schedule();
	test.printSchedule();

	cout << endl;

	cout << "Fitness: ";
	cout << test.getFitness() << endl;

	system("pause");


	return 0;
}