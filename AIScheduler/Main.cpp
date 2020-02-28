#include "Schedule.h"
#include <iostream>
#include <time.h>
using namespace std;

int main() {
	srand(time(NULL));

	do {
		Schedule test = Schedule();
		test.printSchedule();
		cout << test.getFitness() << endl;
		system("pause");
	} while (true);

	return 0;
}