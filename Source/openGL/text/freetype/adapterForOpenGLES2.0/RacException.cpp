#include "RacException.h"

#include <iostream>
#include <sstream>

RacException::RacException(const char* szErr, int num) 
: m_strErr(szErr) 
{
	ostringstream os;
	os << num; 
	m_strErr += os.str();
    std::cout << m_strErr << endl;
}

RacException::RacException(const ostringstream& os) 
{
	m_strErr = os.str();
}
