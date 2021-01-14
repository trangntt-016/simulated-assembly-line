#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
#include<algorithm>
#include<vector>
#include<iomanip>
#include<exception>

#include"CustomerOrder.h"
#include"Utilities.h"


size_t CustomerOrder::m_widthField = 0u;

CustomerOrder::CustomerOrder() {
	m_name = "";
	m_product = "";
	m_cntItem = 0;
	m_lstItem = nullptr;
}
CustomerOrder::CustomerOrder(const std::string& record) {
	{
		m_name = "";
		m_product = "";
		m_cntItem = 0u;
		m_lstItem = nullptr;
	}
	Utilities utils;
	// set the delimiter in case it isnt set. All Utilities objects share the same delimiter
	if (utils.getDelimiter() == ' ') {
		size_t firstDeli = record.find_first_of("|");
		Utilities::setDelimiter(record[firstDeli]);
	}
	// create a vector to store tokens
	std::vector<std::string> tokens;
	bool more = true;
	size_t next_pos = 0u;

	// get the customer Name	
	m_name = utils.extractToken(record, next_pos, more);
	next_pos += m_name.length() + 1;

	//get the product
	m_product = utils.extractToken(record, next_pos, more);
	next_pos += m_product.length() + 1;

	while (more) {
		std::string token;
		try {
			token = utils.extractToken(record, next_pos, more);
		}
		catch (std::exception& err) {
			token = "";
			throw err;
		}
		next_pos += token.length() + 1;
		tokens.push_back(token);
	}


	//count how many items to allocate memory for them based on the delimiter. (-1) is the product's delimiter 
	m_cntItem = (-1) + std::count_if(record.begin(), record.end(), [](const char c) {
		bool result = false;
		if (c == Utilities::getDelimiter()) {
			result = true;
		}
		return result;
		});
	// allocating memory for the list of items making up the order (at least one item) 
	if ((int)m_cntItem > 0) {
		m_lstItem = new Item * [m_cntItem];
		for (int i = 0; i < (int)m_cntItem; i++) {
			m_lstItem[i] = new Item(tokens[i]);
		}
	}
	//update CustomerOrder::m_widthField if the value stored there is smaller than the value stored in Utilities::m_widthField . 
	m_widthField = std::max(utils.getFieldWidth(), m_widthField);
}

CustomerOrder::CustomerOrder(const CustomerOrder& copy) {
	throw "Error";
}

CustomerOrder::CustomerOrder(CustomerOrder&& mv) noexcept {
	*this = std::move(mv);
}

CustomerOrder& CustomerOrder::operator=(CustomerOrder&& mv)noexcept {
	if (this != &mv) {
		if (mv.m_lstItem) {
			if (m_name != "") {
				for (int i = 0; i < (int)m_cntItem; i++) {
					delete m_lstItem[i];
				}
				delete[] m_lstItem;
			}
			m_lstItem = mv.m_lstItem;
		}
		m_cntItem = mv.m_cntItem;
		m_name = mv.m_name;
		m_product = mv.m_product;

		mv.m_lstItem = nullptr;
		mv.m_cntItem = 0;
		mv.m_name = "";
		mv.m_product = "";

	}
	return *this;
}

void CustomerOrder::display(std::ostream& os) const {
	os << m_name << " - " << m_product << std::endl;
	for (int i = 0; i < (int)m_cntItem; i++) {
		os << "[" << std::setfill('0') << std::setw(6) << std::right << m_lstItem[i]->m_serialNumber << "]" << " ";
		os << std::setfill(' ') << std::setw(m_widthField) << std::left << m_lstItem[i]->m_itemName << " - ";
		os << (m_lstItem[i]->m_isFilled ? "FILLED" : "MISSING") << std::endl;
	}
};

bool CustomerOrder::isOrderFilled() const {
	bool result = false;
	int noOfFilled = 0;
	//returns true if All items are filled
	for (int i = 0; i < (int)m_cntItem; i++) {
		if (m_lstItem[i]->m_isFilled) {
			++noOfFilled;
		}
		if (noOfFilled == ((int)m_cntItem - 1)) {
			result = true;
		}
	}
	return result;
};

bool CustomerOrder::isItemFilled(const std::string& itemName)const {
	bool isFilled = true;
	for (int i = 0; i < (int)m_cntItem; i++) {
		if (m_lstItem[i]->m_itemName.compare(itemName) == 0 && (m_lstItem[i]->m_isFilled) == false) {
			isFilled = false;
			i = m_cntItem;
		}
	}
	return isFilled;
};

void CustomerOrder::fillItem(Station& station, std::ostream& os) {
	for (int i = 0; i < (int)m_cntItem; i++) {
		// check if the itemName in station match with any itemName in the list
		if (m_lstItem[i]->m_itemName == station.getItemName()) {
			//check if the quantity of that itemName is still in stock
			if (station.getQuantity() > 0) {
				//update the quantity in stock
				station.updateQuantity();
				//update the filled state and serial number
				m_lstItem[i]->m_isFilled = true;
				m_lstItem[i]->m_serialNumber = station.getNextSerialNumber();
				// Display the filled state
				os << std::setw(10) << std::right << "Filled";
			}
			else {
				os << "Unable to fill";
			}
			os << " " << m_name << ", " << m_product << " [" << m_lstItem[i]->m_itemName << "]" << std::endl;
		}
	}
}

CustomerOrder::~CustomerOrder() {

	for (int i = 0; i < (int)m_cntItem; i++) {
		delete m_lstItem[i];
		m_lstItem[i] = nullptr;
	}
	delete[]m_lstItem;


}