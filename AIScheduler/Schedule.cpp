#include "Schedule.h"
#include <stdlib.h>
#include <time.h>
#include <string>
#include <iostream>
#include <algorithm>
using namespace std;

Schedule::Schedule() {
	int time, room, instructor;
	tuple<int, int, int> temp;

	for (int course = 0; course < 12; course++) {
		do {
			time = rand() % 7;
			room = rand() % 7;
			instructor = rand() % 6;
			temp = make_tuple(time, room, instructor);
		} while (find(this->schedule.begin(), this->schedule.end(), temp) != this->schedule.end());

		this->schedule.push_back(temp);
	}
}

Schedule::Schedule(vector<tuple<int, int, int>> previousSchedule) {
	int randomCourseToChange = rand() % 12;
	int randomElementToChange = rand() % 3;
	tuple<int, int, int> newClass = previousSchedule[randomCourseToChange];
	int newItemValue;

	switch (randomElementToChange) {
	case 0:
		newItemValue = rand() % 7;
		get<0>(newClass) = newItemValue;
		previousSchedule[randomCourseToChange] = newClass;
		break;
	case 1:
		newItemValue = rand() % 7;
		get<1>(newClass) = newItemValue;
		previousSchedule[randomCourseToChange] = newClass;
		break;
	case 2:
		newItemValue = rand() % 6;
		get<2>(newClass) = newItemValue;
		previousSchedule[randomCourseToChange] = newClass;
		break;
	default:
		exit(-1);
	}

	cout << "Changed class: " << randomCourseToChange << " element: " << randomElementToChange << endl;
	this->schedule = previousSchedule;
}

void Schedule::printSchedule() {
	vector<tuple<int, int, int>>::iterator iter;

	for (iter = this->schedule.begin(); iter < this->schedule.end(); iter++) {
		cout << get<0>(*iter) << " " << get<1>(*iter) << " " << get<2>(*iter) << endl;
	}
}

vector<tuple<int, int, int>> Schedule::getSchedule() {
	return this->schedule;
}

int Schedule::getFitness() {
	int score = 0;
	//score += checkInstructor();
	score += checkRoomAndTime();
	return score;
}

int Schedule::checkInstructor() {
	int score = 0;
	vector<int> classes;
	
	for (int course = 0; course < 12; course++) {
		int instructor = get<2>(this->schedule[course]);

		if (instructor == 5) { //class is taught by staff
			score += 1;
		}
		else {
			switch (instructor) {
			case 0:
				classes = { 0, 1, 2, 3, 6, 7, 8, 10, 11 };
				break;
			case 1:
				classes = { 0, 1, 2, 3, 4, 5, 6, 7, 10 };
				break;
			case 2:
				classes = { 8, 9 };
				break;
			case 3:
				classes = { 4, 5, 8, 9 };
				break;
			case 4:
				classes = { 6, 7, 8, 9, 11 };
				break;
			default:
				exit(-1);
			}

			vector<int>::iterator iter = find(classes.begin(), classes.end(), course);
			if (iter != classes.end()) {
				score += 3;
			}
		}
	}

	return score;
}

int Schedule::checkRoomAndTime() {
	vector<pair<int, int>> timesAndRooms;
	pair<int, int> timeAndRoom;
	vector<pair<int, int>>::iterator iter;
	int score = 0;

	for (int course = 0; course < 12; course++) {
		timeAndRoom = make_pair(get<0>(this->schedule[course]), get<1>(this->schedule[course])); //get timeAndRoom pair
		iter = find(timesAndRooms.begin(), timesAndRooms.end(), timeAndRoom);
		if (iter == timesAndRooms.end()) { //if timeAndRoom combo is not in the seen list
			timesAndRooms.push_back(timeAndRoom);
			score += 5;
		}
	}

	return score;
}