#define _CRT_SECURE_NO_WARNINGS  
#include<iostream>
#include<fstream>
#include<string>
#include<exception>
#include"LineManager.h"
#include"Utilities.h"

LineManager::LineManager(const std::string& filename, std::vector<Workstation*>& workstation, std::vector<CustomerOrder>& custOrder) {
	std::ifstream file(filename);
	if (!file)
		throw std::string("Unable to open [") + filename + "] file.";

	std::string record;
	// read from the file
	while (!file.eof())
	{
		// get a line from the record
		std::getline(file, record);

		// get the tokens from the record to set up an assembly line for that record
		Utilities utils;
		bool more = true;
		size_t next_pos = 0u;
		// each record has two tokens, they are used for identifying the station and nextStation to set up the assembly line 
		std::string firstStt = "", nextStt = "";
		try {
			firstStt = utils.extractToken(record, next_pos, more);
			next_pos += firstStt.length() + 1;
		}
		catch (std::exception& err) {
			std::cout << err.what();
		}
		catch (const char* msg) {
			std::cout << msg;
		}
		// in case the second token is "", this block will not be executed
		if (more) {
			try {
				nextStt = utils.extractToken(record, next_pos, more);
			}
			catch (std::exception& err) {
				std::cout << err.what();
			}
			catch (const char* msg) {
				std::cout << msg;
			}
		}

		//set up the assembly line
		// loop through all stations to find if the station that has name matching with the first token
		for (auto station:workstation) {
			if (station->getItemName().compare(firstStt) == 0) {
				// if it's matched, then loop through the station to find the station that has name matching with the second token, this station will be set as the next station
				for (auto nextStation:workstation) {
					if (nextStation->getItemName().compare(nextStt) == 0) {
						// setting the next station
						station->setNextStation(*nextStation);
						// stop the loop
						//break;
					}
				}
				//break;
			}
		}		
	}

	//Move all the CustomerOrder objects onto the back of the ToBeFilled queue - It needs std::move here because CustomerOrder prohibits copy
	for (int i = 0; i < (int)custOrder.size(); i++) {
		ToBeFilled.push_back(std::move(custOrder[i]));
	}

	// The number of CustomerOrder objects the assembly line started with, will need this for display stations (ordered)
	m_cntCustomerOrder = custOrder.size();

	// Copy all the Workstation objects into the AssemblyLine container
	for (int i = 0; i < (int)workstation.size(); i++) {
		AssemblyLine.push_back(workstation[i]);
	}

}

void LineManager::displayStations() const {
	for (int i = 0; i < (int)AssemblyLine.size(); i++) {
		AssemblyLine[i]->display(std::cout);
	}
}

void LineManager::displayStationsSorted() const {
	//the starting point is the first station appearing in the assembly and has no other station's m_nextStation points to
	int startingPoint = getStartingPoint();
	const Workstation* start = AssemblyLine[startingPoint];
	while (start != nullptr) {
		start->display(std::cout);
		start = start->getNextStation();
	}
};

bool LineManager::run(std::ostream& os) {
	static int count = 0;
	bool isAllFilled = false;
	os << "Line Manager Iteration: "<<++count<<std::endl;
	//If there are any CustomerOrder objects in the ToBeFilled queue, move the one at the front of the queue onto the starting point of the AssemblyLine
	if (!ToBeFilled.empty()) {
		*AssemblyLine[this->getStartingPoint()] += std::move(ToBeFilled.front());
		//remove the object at the front of the queue after moving
		ToBeFilled.pop_front();
	}

	// Loop through all stations on the assembly line and run one cycle of the station's process
	for (int i = 0; i < (int)AssemblyLine.size(); i++) {
		AssemblyLine[i]->runProcess(os);
	}

	//Loop through all stations on the assembly line
	for (int i = 0; i < (int)AssemblyLine.size(); i++) {
		if (AssemblyLine[i]->getNextStation() != nullptr) {
			//move the CustomerOrder objects down the line.
			AssemblyLine[i]->moveOrder();
		}
		else {
			//create a temporary obj to get the newly moved from getIfCompleted()
			CustomerOrder tmp;
			if (AssemblyLine[i]->getIfCompleted(tmp)) {
				Completed.push_back(std::move(tmp));
			};
			
		}
	}

	if (Completed.size() == m_cntCustomerOrder) {
		isAllFilled = true;
	}

	return isAllFilled;
};

void LineManager::displayCompletedOrders(std::ostream& os) const {
	for (int i = 0; i < (int)Completed.size(); i++) {
		Completed[i].display(os);
	}
};

int LineManager::getStartingPoint()const {
	int start = 0, i = 0;
	for (i = 0; i < (int)AssemblyLine.size(); i++) {
		int counter_notDup = 0;
		for (int j = 0; j < (int)AssemblyLine.size(); j++) {
			// check if the next Station is nullptr - EOF 
			// check if the current station appears in the nextStation of other current station in the assembly. If it doesnt, so it's the first one appears in the assembly and has no previous pointer points to
			if (AssemblyLine[j]->getNextStation()!=nullptr && (AssemblyLine[i]->getItemName().compare(AssemblyLine[j]->getNextStation()->getItemName()) != 0)) {
				counter_notDup++;
			}
		}
		// not counting the nullptr aka WIFI - EOF so AssemblyLine.size()-1
		if (counter_notDup == (int)AssemblyLine.size()-1) {
			start = i;
		}
	}
	return start;
}