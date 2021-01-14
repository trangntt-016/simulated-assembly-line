#ifndef CUSTOMERORDER_H
#define CUSTOMERORDER_H

#include "Station.h"

struct Item
{
	std::string m_itemName;
	unsigned int m_serialNumber = 0;
	bool m_isFilled = false;
	Item(const std::string& src) : m_itemName(src) {};
};

class CustomerOrder
{
	std::string m_name;
	std::string m_product;
	unsigned int m_cntItem;
	Item** m_lstItem;
public:
	static size_t m_widthField;
	CustomerOrder();
	CustomerOrder(const std::string&);
	CustomerOrder(const CustomerOrder&);
	CustomerOrder& operator=(const CustomerOrder&) = delete;
	CustomerOrder(CustomerOrder&&)noexcept;
	CustomerOrder& operator=(CustomerOrder&&)noexcept;
	bool isOrderFilled() const;
	bool isItemFilled(const std::string& itemName) const;
	void fillItem(Station& station, std::ostream& os);
	void display(std::ostream& os) const;
	~CustomerOrder();
};
#endif