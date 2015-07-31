#ifndef __PARADIGMADATETIME_H__
#define __PARADIGMADATETIME_H__

#include <string>
#include <cstdio>

using namespace std;

namespace Paradigma {

class ParadigmaDateTime
{
    protected:
        int                 bcdToDec(char n) const { return (((unsigned)n)>>4)*10 + (n&0x0f); }

    private:
        unsigned char       m_Date;
        unsigned char       m_Month;
        unsigned char       m_Minute;
        unsigned char       m_Hour;
        

    public:
        ParadigmaDateTime()  : m_Date(0), m_Month(0), m_Minute(0), m_Hour(0) {}
        
        operator string() const                 { char Buffer[15]; sprintf(Buffer, "%02d.%02d, %02d:%02d", getDate(), getMonth(), getHour(), getMinute()); return Buffer; }
        
        std::string         getDateString()     { char Buffer[15]; sprintf(Buffer, "%02d.%02d", getDate(), getMonth()); return Buffer; }
        std::string         getTimeString()     { char Buffer[15]; sprintf(Buffer, "%02d:%02d", getHour(), getMinute()); return Buffer; }
        
        unsigned char       getDate() const     { return bcdToDec(m_Date); }
        unsigned char       getMonth() const    { return bcdToDec(m_Month); }
        unsigned char       getMinute() const   { return bcdToDec(m_Minute); }
        unsigned char       getHour() const     { return bcdToDec(m_Hour); }
} __attribute__ (( __packed__ ));
}
#endif // __PARADIGMADATETIME_H__
