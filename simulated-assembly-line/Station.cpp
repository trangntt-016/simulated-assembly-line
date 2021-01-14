#define _CRT_SECURE_NO_WARNINGS_
#include<iostream>
#include<iomanip>
#include<string>
#include<exception>
#include<algorithm>
#include"Station.h"
#include "Utilities.h"


size_t Station::m_widthField = 0u;
unsigned Station::id_generator = 0;

Station::Station(const std::string& str) {
	Utilities utils;
	// set the delimiter. All Utilities objects share the same delimiter
	size_t firstDeli = str.find_first_of(",|");
	Utilities::setDelimiter(str[firstDeli]);
	//extract the information
	bool more = true;	
	size_t next_pos = 0u;
	while (more) {
		try {
			// get the itemName
			m_itemName = utils.extractToken(str, next_pos, more);
			next_pos += m_itemName.length() + 1;
		}
		catch (std::exception& msg) {
			std::cout << msg.what();
		}
		catch (const char* msg) {
			std::cout << msg;
		}
			// get the serial number
		try {
			std::string serialNo = utils.extractToken(str, next_pos, more);
			next_pos += serialNo.length() + 1;
			m_serialNo = std::stoi(serialNo);
		}
		catch (std::exception& msg) {
			std::cout << msg.what();
		}
		catch (const char* msg) {
			std::cout << msg;
		}

		try {
			//get quantity in stock
			std::string qtyInStock = utils.extractToken(str, next_pos, more);
			next_pos += qtyInStock.length() + 1;
			m_qtyInStock = std::stoi(qtyInStock);
		}
		catch (std::exception& msg) {
			std::cout << msg.what();
		}
		catch (const char* msg) {
			std::cout << msg;
		}

		// updates Station::m_widthField to the maximum value of Station::m_widthFieldand Utilities::m_widthField
		m_widthField = std::max(utils.getFieldWidth(), m_widthField);

		try {
			// get the description
			m_desc = utils.extractToken(str, next_pos, more);
			next_pos += m_desc.length();
		}
		catch (std::exception& msg) {
			std::cout << msg.what();
		}

		catch (const char* msg) {
			std::cout << msg;
		}		
	}

	++id_generator;
	m_stationID = id_generator;
};

void Station::display(std::ostream& os, bool full) const {
	os << "["<< std::right<<std::setw(3)<<std::setfill('0')<< m_stationID <<"]"<<std::left;
	os << " Item: " << std::setw(m_widthField) << std::setfill(' ') << m_itemName << std::right;
	os<< " [" << std::setw(6) << std::setfill('0') << m_serialNo << "]" << std::left;
	if (full) {
		os << " Quantity: " << std::setw(m_widthField) << std::setfill(' ') << m_qtyInStock;
		os << " Description: " << m_desc;
	}
	os << std::endl;
};

const std::string& Station::getItemName()const {
	return m_itemName;
};

unsigned int Station::getNextSerialNumber() {
	return m_serialNo++;
};

unsigned int Station::getQuantity()const {
	return m_qtyInStock;
};

void Station::updateQuantity() {
	--m_qtyInStock;
	if (m_qtyInStock <= 0) {
		m_qtyInStock = 0;
	}
};
