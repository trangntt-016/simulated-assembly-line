//==============================================
// Name:           NGUYEN THI THUY TRANG
// Student Number: 131104192
// Email:          tnguyen-thi-thuy@myseneca.ca
// Section:        
// Date:           14 Nov 2020
//==============================================
// Milestone:      1    
//==============================================
// 2019/11 - Cornel
//==============================================
// Name:           NGUYEN THI THUY TRANG
// Student Number: 131104192
// Email:          tnguyen-thi-thuy@myseneca.ca
// Section:        NLL
// Date:           21 Nov 2020
//==============================================
// Milestone:       2    
#ifndef UTILITIES_H
#define UTILITIES_H

class Utilities
{
	size_t m_widthField =1;
	static char m_delimiter;
public:
	void setFieldWidth(size_t newWidth = 1);
	size_t getFieldWidth()const;
	std::string extractToken(const std::string& str, size_t& next_pos, bool& more);
	static void setDelimiter(char newDelimiter);
	static char getDelimiter();
};



#endif