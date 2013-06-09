#ifndef _RACEXCEPTION_H_INCLUDED_
#define _RACEXCEPTION_H_INCLUDED_

#include <string>
using namespace std;

class RacException 
{
public:
	RacException(const char* szErr) : m_strErr(szErr) {}
	RacException(const char* szErr, const string& strAdd) 
	 : m_strErr(szErr) 
	 {
		 m_strErr += strAdd;
	 }
	RacException(const char* szErr, const char* szAdd) 
	 : m_strErr(szErr) 
	 {
		 if (szAdd != NULL)
			m_strErr += szAdd;
	 }
	RacException(const char* szErr, int num);
	RacException(const ostringstream& os);
	const string& GetStrErr() const
	{
		return m_strErr;
	}
private:
	string m_strErr;
};

#endif // _RACEXCEPTION_H_INCLUDED_
