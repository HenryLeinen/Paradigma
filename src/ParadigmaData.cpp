#include "ParadigmaData.h"



using namespace Paradigma;
static char m_Buffer[50];

ParadigmaMonitorData& ParadigmaMonitorData::operator<<(char c)
{
    int nLen = 0;
    if (m_activeDataBlock != Invalid) {
        //  The actual data block had been detected already, so write into the data block
        m_Buffer[m_actualPos++] = c;
        //  Build checksum on the fly
        m_checksum += c;
        //  Now check if all bytes have been received already
        if (m_activeDataBlock == ParadigmaMonitorDataset1) {
            // for datablock 1
            nLen = sizeof(m_Data1);
            if (nLen == m_actualPos) {
                //   all bytes received, so reset counter and check if checksum is matching
                if (m_checksum != 0) {
#ifdef DEBUG
                    printf("Data corruption error. Received invalid data ! (checksum is %02x)", m_checksum);
#endif
                } else {
                   memcpy(&m_Data1, m_Buffer, nLen);
                    m_Data1.Aussentemp.adjustEndiness();
                    m_Data1.Warmwassertemp.adjustEndiness();
                    m_Data1.Kesselvorlauf.adjustEndiness();
                    m_Data1.Kesselruecklauf.adjustEndiness();
                    m_Data1.RaumtemperaturHK1.adjustEndiness();
                    m_Data1.RaumtemperaturHK2.adjustEndiness();
                    m_Data1.VorlauftemperaturHK1.adjustEndiness();
                    m_Data1.VorlauftemperaturHK2.adjustEndiness();
                    m_Data1.RuecklauftemperaturHK1.adjustEndiness();
                    m_Data1.RuecklauftemperaturHK2.adjustEndiness();
                    m_Data1.PuffertemperaturOben.adjustEndiness();
                    m_Data1.PuffertemperaturUnten.adjustEndiness();
                    m_Data1.Zirkulationstemperatur.adjustEndiness();
#ifdef DEBUG
                    printf("*********************** received new data1 !");
#endif
                    callBack1();
                }
               invalidateHeader();
            }
        } else {
            //  for datablock 2
            nLen = sizeof(m_Data2);
            if (nLen == m_actualPos) {
                //   all bytes received, so reset counter and check if checksum is matching
                if (m_checksum != 0) {
                    printf("Data corruption error. Received invalid data ! (checksum is %02x)", m_checksum);
                } else {
                    memcpy(&m_Data2, m_Buffer, nLen);
                    m_Data2.RaumsollHK1.adjustEndiness();
                    m_Data2.RaumsollHK2.adjustEndiness();
                    m_Data2.VorlaufsollHK1.adjustEndiness();
                    m_Data2.VorlaufsollHK2.adjustEndiness();
                    m_Data2.Warmwassersolltemp.adjustEndiness();
                    m_Data2.Puffersolltemp.adjustEndiness();
                    m_Data2.BetriebsstundenKessel.adjustEndiness();
                    m_Data2.AnzahlKesselstarts.adjustEndiness();
                    m_Data2.StoercodeKessel.adjustEndiness();
#ifdef DEBUG
                    printf("*********************** received new data2 !");
#endif
                    callBack2();
                }
                invalidateHeader();
            }
        }
    } else {
        //  Still trying to detect a valid data block, check to see if a valid address-part is here
        switch (m_actualPos) {
            case 0 :        // block type
                if ( (c== ParadigmaParameters) || (c== ParadigmaVariables)) {
                    m_Header.block_type = (ParadigmaBlockType_t)c;
                    m_actualPos++;
                    m_checksum = c;
#ifdef DEBUG
                    printf("Block start !\n");
#endif
                }
                break;
                
            case 1 :        //  length
                m_Header.block_length = c;
                m_actualPos++;
                m_checksum += c;
#ifdef DEBUG
                printf("Block len !\n");
#endif
                break;
                
            case 2 :        //  Message ID
                if ( c == ParadigmaMessage ) {                    m_Header.message_id = (ParadigmaMessageID_t)c;
                    m_actualPos++;
                    m_checksum += c;
                } else {
                    invalidateHeader();
#ifdef DEBUG
                    printf("Rejected due to incorrect Message ID %d\n", c);
#endif
                }
                break;
                
            case 3 :        //
                if ( (c == ParadigmaMonitorDataset1) || (c == ParadigmaMonitorDataset2)) {
                    m_Header.dataset_type = (ParadigmaDatasetType_t)c;
                    m_actualPos = 0;
                    m_checksum += c;
                    m_activeDataBlock = (ParadigmaDatasetType_t)c;
#ifdef DEBUG
                    printf("Dataset Number %d!\n", c);
#endif
                } else {
                    invalidateHeader();
#ifdef DEBUG
                    printf("Rejected due to incorrect Dataset number %d\n", c);
#endif
                }
                break;
            
            default:
                invalidateHeader();
                break;
        }
    }
}

/** Function will reset all header and actual position information so that search for valid header
 *  will be reset and starting from beginning with receival of next character.
 *
 *  Will have to clearup the header itsself and the checksum. Also the actual data block information
 *  needs to be invalidated. Actual position will also be reset
 */
void ParadigmaMonitorData::invalidateHeader()
{
    //  Invalidate Header information
    memset(&m_Header, 0, sizeof(m_Header));     
    //  Set active data block information to invalid
    m_activeDataBlock = (ParadigmaDatasetType_t)Invalid; 
    //  Reset actual position with in Buffer so that next char starts from beginning
    m_actualPos = 0;
    //  Reset the checksum, because it will be calculated on the fly.
    m_checksum = 0;
}

/** Function will let user select which temperature to retrieve */
ParadigmaTemperature ParadigmaMonitorData::getTemperature(ParadigmaTemperatureSelector_t sel)
{
    ParadigmaTemperature    temp;
    switch(sel) {
        //  Temperatures from Monitor Data 1
        case T_aussen :             temp = m_Data1.Aussentemp; break;
        case T_warm_wasser :        temp = m_Data1.Warmwassertemp; break;
        case T_kessel_vorlauf :     temp = m_Data1.Kesselvorlauf; break;
        case T_kessel_ruecklauf :   temp = m_Data1.Kesselruecklauf; break;
        case T_HK1_raum :           temp = m_Data1.RaumtemperaturHK1; break;
        case T_HK2_raum :           temp = m_Data1.RaumtemperaturHK2; break;
        case T_HK1_vorlauf :        temp = m_Data1.VorlauftemperaturHK1; break;
        case T_HK2_vorlauf :        temp = m_Data1.VorlauftemperaturHK2; break;
        case T_HK1_ruecklauf :      temp = m_Data1.RuecklauftemperaturHK1; break;
        case T_HK2_ruecklauf :      temp = m_Data1.RuecklauftemperaturHK2; break;
        case T_puffer_oben :        temp = m_Data1.PuffertemperaturOben; break;
        case T_puffer_unten :      temp = m_Data1.PuffertemperaturUnten; break;
        case T_zirkulation :        temp = m_Data1.Zirkulationstemperatur; break;
        
        //  Temperatures form Monitor Data 2
        case T_HK1_raum_soll :      temp = m_Data2.RaumsollHK1; break;
        case T_HK2_raum_soll :      temp = m_Data2.RaumsollHK2; break;
        case T_HK1_vorlauf_soll :   temp = m_Data2.VorlaufsollHK1; break;
        case T_HK2_vorlauf_soll :   temp = m_Data2.VorlaufsollHK2; break;
        case T_warm_wasser_soll :   temp = m_Data2.Warmwassersolltemp; break;
        case T_puffer_soll :        temp = m_Data2.Puffersolltemp; break;
        
        default:    // Invalid selecion !
#ifdef DEBUG
            printf("**** An unknown selection for the temperature was made !");
#endif
            break;
    }
	return temp;
}
