#define _CRT_SECURE_NO_WARNINGS_
#include<iostream>
#include<string>
#include<exception>
#include<iomanip>
#include"Utilities.h"



void Utilities::setFieldWidth(size_t newWidth) {
	if (newWidth > m_widthField) {
		m_widthField = newWidth;
	}
}

size_t Utilities::getFieldWidth()const {
	return m_widthField;
}


std::string Utilities::extractToken(const std::string& str, size_t& next_pos, bool& more) {
	std::string tokenFound, tmp_str;
	try {
		tmp_str = str.substr(next_pos);
	}
	catch (std::exception& err) {
		throw "Unable to extract the temp string";
	}
		
	int deliPos = tmp_str.find(Utilities::getDelimiter());
	if (deliPos > 0) {
		try {
			tokenFound = str.substr(next_pos, deliPos);
		}
		catch (std::exception& err) {
			//const char* msg = "Unable to extract the token";
			throw err;
		}
		if (tokenFound.empty()) {
			const char* msg = "This token is empty";
			throw msg;
		}
	}
	else {
		tokenFound = str.substr(next_pos);
		more = false;
	}
	
	//updates the current object's m_widthField data member if its current value is less than the size of the token extracted.
	setFieldWidth(tokenFound.length());
	return tokenFound;
}

char Utilities::m_delimiter = ' ';
void Utilities::setDelimiter(char newDelimiter) {
	m_delimiter = newDelimiter;
};

char Utilities::getDelimiter() {
	return m_delimiter;
};
