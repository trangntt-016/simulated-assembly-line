#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include "Workstation.h"


Workstation::Workstation(const std::string& record) :Station(record) {
	m_pNextStation = nullptr;
};

void Workstation::runProcess(std::ostream& os) {
	//If the queue is not empty,fills the order in the front at the current Workstation
	if (!m_orders.empty()) {
		m_orders.front().fillItem(*this, os);
	}
}

bool Workstation::moveOrder() {
	bool isMoved = false;
	// check if the queue is not empty and the order at the front of the queue having items that are all filled
	if (!m_orders.empty()&&m_orders.front().isItemFilled(this->getItemName())) {
		//move it to the next station on the assembly line
		*m_pNextStation += std::move(m_orders.front());
		//removing after moving
		m_orders.pop_front();
		isMoved = true;
	}
	return isMoved;
}
void Workstation::setNextStation(Station& station) {
	// remember to set base class to be virtual 
	m_pNextStation = dynamic_cast<Workstation*>(&station);
};

const Workstation* Workstation::getNextStation() const {
	return m_pNextStation;
};

bool Workstation::getIfCompleted(CustomerOrder& order) {
	bool isComplete = false;
	//check if the queue is not empty and if the orders at the front are complete (filled)
	if (!m_orders.empty()&&m_orders.front().isOrderFilled()) {
		//remove the order at the front from the queue and place it in the parameter
		order = std::move(m_orders.front());
		//removing after moving
		m_orders.pop_front();
		isComplete = true;
	}
	return isComplete;
};
void Workstation::display(std::ostream& os)const {
	os << this->getItemName();
	os << (m_pNextStation == nullptr ? " --> END OF LINE" : " --> "+ m_pNextStation->getItemName()) << std::endl;
};

Workstation& Workstation::operator+=(CustomerOrder&& order) {
	m_orders.push_back(std::move(order));
	return *this;
};

Workstation::~Workstation() {
	m_pNextStation = nullptr;
}