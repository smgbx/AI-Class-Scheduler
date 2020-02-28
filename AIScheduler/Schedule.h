#pragma once
#pragma once
#include <unordered_map>
#include <vector>
using namespace std;

class Schedule {
public:
	Schedule(); //random schedule
	Schedule(vector<tuple<int, int, int>> previousSchedule); //copy of previous schedule
	void printSchedule();
	vector<tuple<int, int, int>> getSchedule();
	int getFitness();

private:
	vector<tuple<int, int, int>> schedule;
	int checkInstructor();
	int checkRoomAndTime();
	int checkRoomCapacity();
	bool atSameTime(int firstClass, int secondClass);
	bool areAdjacent(int firstClass, int secondClass);
	bool sameBuilding(int firstClass, int secondClass);
	int scoreIfInDistantBuildings(int firstClass, int secondClass);
	string getBuilding(int course);
	bool sectionsAtDifferentTimes(int firstClass, int secondClass);
};