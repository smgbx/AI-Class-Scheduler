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

};

//Generate neighbor schedule
//Randomly swaps certain elements