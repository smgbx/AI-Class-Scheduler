#pragma once
#pragma once
#include <unordered_map>
#include <vector>
using namespace std;

class Schedule {
public:
	Schedule(); //random schedule
	Schedule(vector<tuple<int, int, int>> previousSchedule); //copy of previous schedule with one element changed
	void printSchedule();
	vector <tuple<int, int, int>> getSchedule();
	int getFitness();

private:
	vector<tuple<int, int, int>> schedule;

	int checkInstructor();
	int checkRoomAndTime();
	int checkRoomCapacity();
	int checkPairs();

	string getBuilding(int course);

	bool atSameTime(int firstClass, int secondClass);
	bool areAdjacent(int firstClass, int secondClass);
	bool sameBuilding(int firstClass, int secondClass);
	bool sectionsAtDifferentTimes(int firstClass, int secondClass);
	bool onlyOneInKatz(int firstClass, int secondClass);
	bool onlyOneInBloch(int firstClass, int secondClass);
};