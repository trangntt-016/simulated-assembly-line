#ifndef LINEMANAGER_H
#define LINEMANAGER_H
#include<vector>
#include<deque>
#include"Workstation.h"

class LineManager
{
	std::vector<Workstation*> AssemblyLine;
	std::deque<CustomerOrder> ToBeFilled;
	std::deque<CustomerOrder> Completed;
	unsigned int m_cntCustomerOrder;
	int getStartingPoint()const;
public:
	LineManager(const std::string&, std::vector<Workstation*>&, std::vector<CustomerOrder>&);
	void displayStations() const;
	void displayStationsSorted() const;
	bool run(std::ostream& os);
	void displayCompletedOrders(std::ostream& os) const;
};


#endif