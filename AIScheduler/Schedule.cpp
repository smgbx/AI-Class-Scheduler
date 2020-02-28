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
	score += checkInstructor();
	//score += checkRoomAndTime();
	//score += checkRoomCapacity();
	return score;
}

int Schedule::checkInstructor() {
	/* 
	* If each course can be taught by its assigned instructor, other than Staff +3
	* If a course is taught by Staff +1
	* If an instructor is not assigned to teach another class at the same time, +5
	* If a graduate instructor is teaching more courses than a non-graduate instructor -10
	*/

	int score = 0;
	vector<int> classes;
	vector<pair<int, int>> timesAndInstructors;
	vector<int> coursesPerInstructors = { 0, 0, 0, 0, 0 };

	for (int course = 0; course < 12; course++) {
		int instructor = get<2>(this->schedule[course]);

		if (instructor == 5) { //Checking if class is taught by staff
			score += 1;
		}
		else {
			switch (instructor) {
			case 0:
				classes = { 0, 1, 2, 3, 6, 7, 8, 10, 11 };
				coursesPerInstructors[0] += 1;
				break;
			case 1:
				classes = { 0, 1, 2, 3, 4, 5, 6, 7, 10 };
				coursesPerInstructors[1] += 1; 
				break;
			case 2:
				classes = { 8, 9 };
				coursesPerInstructors[2] += 1;
				break;
			case 3:
				classes = { 4, 5, 8, 9 };
				coursesPerInstructors[3] += 1;
				break;
			case 4:
				classes = { 6, 7, 8, 9, 11 };
				coursesPerInstructors[4] += 1;
				break;
			default:
				exit(-1);
			}
			//Checking if instructor can teach assigned course
			vector<int>::iterator iter = find(classes.begin(), classes.end(), course);
			if (iter != classes.end()) {
				score += 3;
			}
		}
		//Checking if an instructor is assigned to teach another class at the same time
		int time = get<0>(this->schedule[course]);
		pair<int, int> timeAndInstructor = make_pair(time, instructor);
		
		vector<pair<int, int>>::iterator iter = find(timesAndInstructors.begin(), timesAndInstructors.end(), timeAndInstructor); 
		if (iter == timesAndInstructors.end()) {
			timesAndInstructors.push_back(timeAndInstructor);
			score += 5;
		}
	}
	//Checking if each instructor is teaching more than four courses
	for (int& numCourses: coursesPerInstructors) {
		if (numCourses > 4) {
			score -= 5;
		}
	}

	//Checking if Rao is teaching more courses than Hare or Bingham
	if ((coursesPerInstructors[4] > coursesPerInstructors[0]) || (coursesPerInstructors[4] > coursesPerInstructors[1])) {
		score -= 10;
	}
	//Checking if Mitchell is teaching more courses than Hare or Bingham
	else if ((coursesPerInstructors[3] > coursesPerInstructors[0]) || (coursesPerInstructors[3] > coursesPerInstructors[1])) {
		score -= 10;
	}

	return score;
}

int Schedule::checkRoomAndTime() {
	/*
	* If a course is the only course scheduled in that room at that time +5
	* Pairs (CS101 and CS 191) (CS201 and CS291) are compared:
	* If classes within pairs are at the same time -15
	* If classes within pairs are at adjacent times + 5
	* Given that two classes within a pair are at adjacent times:
	*		Are also in the same building +5
	*		One is in Katz and the other isn't -3
	*		One is in Bloch and the other isn't -3
	* If different sections of the same class (CS101, CS191) are 3 hours apart or more +5
	*/
	vector<pair<int, int>> timesAndRooms;
	vector<int> times(12, 0);
	pair<int, int> timeAndRoom;
	vector<pair<int, int>>::iterator iter;
	int score = 0;

	for (int course = 0; course < 12; course++) {
		int time = get<0>(this->schedule[course]);
		int room = get<1>(this->schedule[course]);

		timeAndRoom = make_pair(time, room); //get timeAndRoom pair
		iter = find(timesAndRooms.begin(), timesAndRooms.end(), timeAndRoom);
		if (iter == timesAndRooms.end()) { //if timeAndRoom combo is not in the seen list
			timesAndRooms.push_back(timeAndRoom);
			score += 5;
		}

		switch (course) {
		case 0:
			if (sectionsAtDifferentTimes(course, 1)) {
				score += 5;
			}
			break;
		case 1:
			if (atSameTime(course, 4) || atSameTime(course, 5)) {
				score -= 15;
			}
			else if (areAdjacent(course, 4) || areAdjacent(course, 5)) {
				score += 5;
				if (sameBuilding(course, 4) || sameBuilding(course, 5)) {
					score += 5;
				}
				score += scoreIfInDistantBuildings(course, 4);
				score += scoreIfInDistantBuildings(course, 5);
			}
			break;
		case 2:
		case 3:
			if (atSameTime(course, 6) || atSameTime(course, 7)) {
				score -= 15;
			}
			else if (areAdjacent(course, 6) || areAdjacent(course, 7)) {
				score += 5;
				if (sameBuilding(course, 6) || sameBuilding(course, 7)) {
					score += 5;
				}
				score += scoreIfInDistantBuildings(course, 6);
				score += scoreIfInDistantBuildings(course, 7);
			}
			break;
		case 4:
			if (sectionsAtDifferentTimes(course, 5)) {
				score += 5;
			}
			break;
		default:
			break;
		}
	}

	return score;
}

int Schedule::checkRoomCapacity() {
	/*
	* If a room size is bigger than the expected course enrollment +5
	* If a room size is no larger than twice the expected course enrollment +2
	*/


	int room, capacity, expectedEnrollment;
	vector<tuple<int, int, int>> currentClass;

	int score = 0;

	for (int course = 0; course < 12; course++) {
		room = get<1>(this->schedule[course]);

		switch (room) {
		case 0:
			capacity = 70;
			break;
		case 1:
			capacity = 30;
			break;
		case 2:
			capacity = 70;
			break;
		case 3:
			capacity = 50;
			break;
		case 4:
			capacity = 80;
			break;
		case 5:
			capacity = 25;
			break;
		case 6:
			capacity = 30;
			break;
		default:
			exit(-1);
		}

		switch (course) {
		case 0:
			expectedEnrollment = 40;
			break;
		case 1:
			expectedEnrollment = 25;
			break;
		case 2:
			expectedEnrollment = 30;
			break;
		case 3:
			expectedEnrollment = 30;
			break;
		case 4:
			expectedEnrollment = 60;
			break;
		case 5:
			expectedEnrollment = 20;
			break;
		case 6:
			expectedEnrollment = 20;
			break;
		case 7:
			expectedEnrollment = 40;
			break;
		case 8:
			expectedEnrollment = 50;
			break;
		case 9:
			expectedEnrollment = 40;
			break;
		case 10:
			expectedEnrollment = 55;
			break;
		case 11:
			expectedEnrollment = 40;
			break;
		default:
			exit(-1);
		}

		if (capacity > expectedEnrollment) {
			score += 5;
			if (capacity <= (2 * expectedEnrollment)) {
				score += 2;
			}
		}
	}
	
	return score;
}

bool Schedule::atSameTime(int firstClass, int secondClass) {
	int timeFirstClass = get<0>(this->schedule[firstClass]);
	int timeSecondClass = get<0>(this->schedule[secondClass]);

	if (timeFirstClass == timeSecondClass) {
		return true;
	}
	else {
		return false;
	}
}

bool Schedule::areAdjacent(int firstClass, int secondClass) {
	int timeFirstClass = get<0>(this->schedule[firstClass]);
	int timeSecondClass = get<0>(this->schedule[secondClass]);

	if (abs(timeFirstClass - timeSecondClass) == 1) {
		return true;
	}
	else {
		return false;
	}
}

bool Schedule::sameBuilding(int firstClass, int secondClass) {
	if (getBuilding(firstClass) == getBuilding(secondClass)) {
		return true;
	}
	else {
		return false;
	}
}

int Schedule::scoreIfInDistantBuildings(int firstClass, int secondClass) {
	string firstLocation = getBuilding(firstClass);
	string secondLocation = getBuilding(secondClass);

	int score = 0;
	
	if ((firstLocation == "Katz" && secondLocation != "Katz") || (firstLocation != "Katz" && secondLocation == "Katz")) {
		score -= 3;
	}
	if ((firstLocation == "Bloch" && secondLocation != "Bloch") || (firstLocation != "Bloch" && secondLocation == "Bloch")) {
		score -= 3;
	}

	return score;
}

bool Schedule::sectionsAtDifferentTimes(int firstClass, int secondClass) {
	int timeFirstClass = get<0>(this->schedule[firstClass]);
	int timeSecondClass = get<0>(this->schedule[secondClass]);

	if (abs(timeFirstClass - timeSecondClass) >= 3) {
		return true;
	}
	else {
		return false;
	}
}

string Schedule::getBuilding(int course) {
	switch (course) {
	case 0:
	case 1:
		return "Haag";
	case 2:
		return "Royall";
	case 3:
		return "Katz";
	case 4: 
	case 5:
		return "Flarsheim";
	case 6:
		return "Bloch";
	default:
		exit(-1);
	}
}