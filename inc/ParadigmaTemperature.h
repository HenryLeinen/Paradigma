#ifndef __PARADIGMATEMPERATURE_H__
#define __PARADIGMATEMPERATURE_H__

#include <string>
#include "ParadigmaBase.h"

using namespace std;

namespace Paradigma {

typedef enum {
    //  Temperatures from Monitor Data 1
    T_aussen,
    T_warm_wasser,
    T_kessel_vorlauf,
    T_kessel_ruecklauf,
    T_HK1_raum,
    T_HK2_raum,
    T_HK1_vorlauf,
    T_HK2_vorlauf,
    T_HK1_ruecklauf,
    T_HK2_ruecklauf,
    T_puffer_oben,
    T_puffer_unten,
    T_zirkulation,
    //  Temperatures form Monitor Data 2
    T_HK1_raum_soll,
    T_HK2_raum_soll,
    T_HK1_vorlauf_soll,
    T_HK2_vorlauf_soll,
    T_warm_wasser_soll,
    T_puffer_soll
} ParadigmaTemperatureSelector_t;


class ParadigmaTemperature 
{
    short m_temp;
    
    public:
        ParadigmaTemperature() : m_temp(0) {}
        
        operator string() const ;
        operator float() const ;
        
        ParadigmaTemperature& operator=(float f);
        ParadigmaTemperature& operator=(ParadigmaTemperature &p);
        
    public:
        
        /* On the MBED we need to swap the high and lowbytes after reading from stream. 
          * this function will be called from reading routine.
          */
        void    adjustEndiness()        { m_temp = ((m_temp&0xFF)<<8) | ((m_temp>>8)&0xFF); }
} __attribute__ (( __packed__ ));
}
#endif
