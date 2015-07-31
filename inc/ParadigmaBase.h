#ifndef __PARADIGMABASE_H__
#define __PARADIGMABASE_H__

namespace Paradigma
{

typedef unsigned char byte;
typedef short word;
typedef unsigned short uword;
typedef unsigned long ulong;


class ParadigmaDword
{
protected:
    unsigned long   m_data;

public:
    ParadigmaDword()    :   m_data(0)   {}

    /** On the MBED we need to swap the high and lowbytes after reading from stream.
      * this function will be called from reading routine.
      */
    void    adjustEndiness()        {
        m_data = ((m_data>>24)&0xFF) | ((m_data>>8)&0x0000FF00) | ((m_data<<8)&0x00FF0000) | ((m_data<<24)&0xFF000000);
    }

    ParadigmaDword&         operator=(ulong d)              {
        m_data = d;
        return *this;
    }
    ParadigmaDword&         operator=(ParadigmaDword d)     {
        m_data = d.m_data;
        return *this;
    }

    operator ulong() const                                  {
        return m_data;
    }
} __attribute__ (( __packed__ ));

class ParadigmaWord
{
protected:
    unsigned short  m_data;
public:
    ParadigmaWord() :   m_data(0)   {}

    /** On the MBED we need to swap the high and lowbytes after reading from stream.
      * this function will be called from reading routine.
      */
    void    adjustEndiness()        {
        m_data = ((m_data&0xFF)<<8) | ((m_data>>8)&0xFF);
    }

    ParadigmaWord&          operator=(word d)               {
        m_data = d;
        return *this;
    }
    ParadigmaWord&          operator=(ParadigmaWord d)      {
        m_data = d.m_data;
        return *this;
    }

    operator word() const                                   {
        return m_data;
    }
} __attribute__ (( __packed__ ));
}
#endif // __PARADIGMABASE_H__

