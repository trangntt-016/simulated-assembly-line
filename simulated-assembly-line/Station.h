#ifndef STATION_H
#define STATION_H

#include<iostream>


	class Station
	{
		std::string m_itemName;
		unsigned int m_serialNo;
		unsigned int m_qtyInStock;
		std::string m_desc;
		int m_stationID;
	public:
		Station() {};
		static size_t m_widthField;
		static unsigned id_generator;
		Station(const std::string&);
		const std::string& getItemName()const;
		unsigned int getNextSerialNumber();
		unsigned int getQuantity()const;
		void updateQuantity();
		void display(std::ostream& os, bool full) const;
		//virtual destructor  so that Station can be polymorphic and can use dynamic_cast for the derived class
		virtual ~Station() {};
	};

#endif