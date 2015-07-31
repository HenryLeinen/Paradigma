#ifndef __PARADIGMA_DATA_H__
#define __PARADIGMA_DATA_H__

#include "ParadigmaBase.h"
#include "ParadigmaTemperature.h"
#include "ParadigmaDateTime.h"

#include <string>
#include <cstring>

using namespace std;

namespace Paradigma
{

/** ParadigmaBlockType_t enumerates valid values for the block type. The block type is always the first
  * data byte in a response from paradigma. There are currently two known values.
  */
typedef enum {
    ParadigmaVariables = 0xFC,
    ParadigmaParameters = 0xFD,
} __attribute__((__packed__)) ParadigmaBlockType_t;

/** ParadigmaMessageID_t specifies the content of the transmitted object. Currently only one valid value
  * is known which indicates a valuid variable or parameter block is being transmitted */
typedef enum {
    ParadigmaMessage = 0x0C,
} __attribute__ (( __packed__ )) ParadigmaMessageID_t;

/** ParadigmaDatasetType_t specifies which type of information is contained in the datablock. Currently
  * known values are : Dataset1 and Dataset2 for Variables, and Parameters for different settings which
  * can be changed and quried by the user.
  */
typedef enum {
    ParadigmaMonitorDataset1 = 0x01,
    ParadigmaMonitorDataset2 = 0x02,
    ParadigmaParameterset = 0x03,
    ParadigmaUnknown1 = 0x14,
    Invalid = 0xFF
} __attribute__((__packed__)) ParadigmaDatasetType_t;

typedef char ParadigmaParameterAddress_t[3];

const ParadigmaParameterAddress_t ParadigmaAdresses[] = {
    {0x00, 0x02, 0x2A},      //  Heizkreis 1
    {0x01, 0x88, 0x2A},      //  Heizkreis 2
    {0x03, 0x0E, 0x09},      //  Warmwasser
    {0x03, 0xF7, 0x0F},      //  Anlagendaten Kessel/Puffer und Zirkulation
    {0x03, 0xE6, 0x12},      //  Wartung Telefonnummer
    {0x03, 0x17, 0x70},      //  Warmwasserprogramm 1
    {0x03, 0x87, 0x70},      //  Warmwasserprogramm 2
    {0x04, 0x06, 0x70},      //  Zirkulationszeitprogramm 1
    {0x04, 0x76, 0x70},      //  Zirkulationszeitprogramm 2
    {0x00, 0x2C, 0x70},      //  Heizzeitprogramm 1 HK1
    {0x00, 0x9C, 0x70},      //    Heizzeitprogramm 2 HK1
    {0x01, 0x0C, 0x70},     // Heizzeitprogramm 3 HK1
    {0x01, 0xB2, 0x70},     // Heizzeitprogramm 1 HK2
    {0x02, 0x22, 0x70},     //  Heizzeitprogramm 2 HK2
    {0x02, 0x92, 0x70},     //  Heizzeitprogramm 3 HK2
    {0x05, 0x08, 0x03}      //  Anlagendaten Kessel/Puffer 2
};

/** ParadigmaBlockHeader_t represents the header of any sent block by paradigma. */
typedef struct {
    ParadigmaBlockType_t        block_type;         // 0xFC for normal observable variables, 0xFD for parameter blocks
    unsigned char               block_length;
    ParadigmaMessageID_t        message_id;         // always 0x0C
    ParadigmaDatasetType_t      dataset_type;
} __attribute__((__packed__)) ParadigmaBlockHeader_t;

typedef struct {
    ParadigmaDateTime           DateTime;
    ParadigmaTemperature        Aussentemp;             //  (in 0,1 Grad Schritten)
    ParadigmaTemperature        Warmwassertemp;         //  (in 0,1 Grad Schritten)
    ParadigmaTemperature        Kesselvorlauf;          //  (in 0,1 Grad Schritten)
    ParadigmaTemperature        Kesselruecklauf;        //  (in 0,1 Grad Schritten)

    ParadigmaTemperature        RaumtemperaturHK1;      //  (in 0,1 Grad Schritten)
    ParadigmaTemperature        RaumtemperaturHK2;      //  (in 0,1 Grad Schritten)
    ParadigmaTemperature        VorlauftemperaturHK1;   //  (in 0,1 Grad Schritten)
    ParadigmaTemperature        VorlauftemperaturHK2;   //  (in 0,1 Grad Schritten)
    ParadigmaTemperature        RuecklauftemperaturHK1; //  (in 0,1 Grad Schritten)
    ParadigmaTemperature        RuecklauftemperaturHK2; //  (in 0,1 Grad Schritten)
    ParadigmaTemperature        PuffertemperaturOben;   //  (in 0,1 Grad Schritten)
    ParadigmaTemperature        PuffertemperaturUnten;  //  (in 0,1 Grad Schritten)
    ParadigmaTemperature        Zirkulationstemperatur; //  (in 0,1 Grad Schritten)

    byte                        Checksumme;
} __attribute__((__packed__)) MonDta1_t;

typedef struct {
    ParadigmaTemperature        RaumsollHK1;
    ParadigmaTemperature        RaumsollHK2;
    ParadigmaTemperature        VorlaufsollHK1;
    ParadigmaTemperature        VorlaufsollHK2;
    ParadigmaTemperature        Warmwassersolltemp;
    ParadigmaTemperature        Puffersolltemp;
    struct {
        word    PHK1:1;
        word    PHK2:1;
        word    PK:1;
        word    Mischer1Auf:1;
        word    Mischer1Zu:1;
        word    Mischer2Auf:1;
        word    Mischer2Zu:1;
        word    ULV:1;
        word    PZ:1;
        word    B1:1;
        word    Taster:1;
        word    LONModul:1;
        word    OTModul:1;
        word    reserver:3;
    } __attribute__((__packed__))           ZustandAusgaenge;
    ParadigmaDword              BetriebsstundenKessel;
    ParadigmaDword              AnzahlKesselstarts;
    ParadigmaWord               StoercodeKessel;
    byte                        StoercodeFuehler;
    byte                        BetriebsartHK1;
    byte                        NiveauHK1;
    byte                        BetriebsartHK2;
    byte                        NiveauHK2;
    byte                        LeistungPHK1;
    byte                        LeistungPHK2;
    byte                        LeistungPK;

    byte                        Checksumme;
} __attribute__((__packed__)) MonDta2_t;


/** Class ParadigmaMonitorData encapsulates the data representation and parsing of the
  * data blocks that are being sent by the paradigma heater. The class also provides
  * a stream type functionality to detect and parse the data from a stream of chars.
  */
class ParadigmaMonitorData
{
	void (*m_CallbackData1)(void);
	void (*m_CallbackData2)(void);

    MonDta1_t               m_Data1;
    MonDta2_t               m_Data2;

    ParadigmaBlockHeader_t  m_Header;
    ParadigmaDatasetType_t  m_activeDataBlock;
    int                     m_actualPos;
    char                    m_checksum;

    void                    invalidateHeader();
    void                    callBack1()  {
        if (m_CallbackData1)
		m_CallbackData1();
    }
    void                    callBack2()  {
        if (m_CallbackData2)
		m_CallbackData2();
    }
    word                    swapWord(word d) {
        return ((d&0xFF)<<8) | ((d>>8)&0xFF);
    }
    ulong                  swapDWord(ulong d) {
        return ((d>>24)&0xFF) | ((d>>8)&0x0000FF00) | ((d<<8)&0x00FF0000) | ((d<<24)&0xFF000000);
    }
public:
    /** Public constructor. Will initialize all internal data and variables. */
    ParadigmaMonitorData(): m_checksum(0)  {
        memset(&m_Data1, 0, sizeof(m_Data1));
        memset(&m_Data2, 0, sizeof(m_Data2));
        invalidateHeader();
    }
    /** Access function to get the transmitted date time object.
      * @returns : the date and time objects of the transmitted data.
      */
    ParadigmaDateTime getDateTime()                     {
        return m_Data1.DateTime;
    }
    /** Access function to the Outside air temperature (AUSSENTEMP).
      * @returns : the outside air temperature.
      */
    ParadigmaTemperature getAussentemp()                {
        return m_Data1.Aussentemp;
    }
    /** Access function to the warm water temperature (WARMWASSERTEMP).
      * @returns : the warm water temperature.
      */
    ParadigmaTemperature getWarmwassertemp()            {
        return m_Data1.Warmwassertemp;
    }
    /** Access function to the 'kessel vorlauf' temperature.
      * @returns : the temperature of the 'kessel vorlauf'.
      */
    ParadigmaTemperature getKesselvorlauf()             {
        return m_Data1.Kesselvorlauf;
    }
    /** Access function to the 'kessel ruecklauf' temperature.
      * @returns : the temperature of the 'kessel ruecklauf'.
      */
    ParadigmaTemperature getKesselruecklauf()           {
        return m_Data1.Kesselruecklauf;
    }
    /** Access function to the room temperature of heating circuit 1.
      * @returns : the room temperature of heating circuit 1.
      */
    ParadigmaTemperature getRaumtemperaturHK1()         {
        return m_Data1.RaumtemperaturHK1;
    }
    /** Access function to the room temperature of heating circuit 2.
      * @returns : the room temperature of heating circuit 2.
      */
    ParadigmaTemperature getRaumtemperaturHK2()         {
        return m_Data1.RaumtemperaturHK2;
    }
    ParadigmaTemperature getVorlauftemperaturHK1()      {
        return m_Data1.VorlauftemperaturHK1;
    }
    ParadigmaTemperature getVorlauftemperaturHK2()      {
        return m_Data1.VorlauftemperaturHK2;
    }
    ParadigmaTemperature getRuecklauftemperaturHK1()    {
        return m_Data1.RuecklauftemperaturHK1;
    }
    ParadigmaTemperature getRuecklauftemperaturHK2()    {
        return m_Data1.RuecklauftemperaturHK2;
    }
    ParadigmaTemperature getPuffertemperaturOben()      {
        return m_Data1.PuffertemperaturOben;
    }
    ParadigmaTemperature getPuffertemperaturUnten()     {
        return m_Data1.PuffertemperaturUnten;
    }
    ParadigmaTemperature getZirkulationstemperatur()    {
        return m_Data1.Zirkulationstemperatur;
    }

    ParadigmaTemperature getRaumsollHK1()               {
        return m_Data2.RaumsollHK1;
    }
    ParadigmaTemperature getRaumsollHK2()               {
        return m_Data2.RaumsollHK2;
    }
    ParadigmaTemperature getVorlaufsollHK1()            {
        return m_Data2.VorlaufsollHK1;
    }
    ParadigmaTemperature getVorlaufsollHK2()            {
        return m_Data2.VorlaufsollHK2;
    }
    ParadigmaTemperature getWarmwassersolltemp()        {
        return m_Data2.Warmwassersolltemp;
    }
    ParadigmaTemperature getPuffersolltemp()            {
        return m_Data2.Puffersolltemp;
    }

    /** Function will let the user select the temperature to be returned.
      * @param sel : Will specify which temperature value to return.
      * @returns : the temperature object as selected by param sel.
      */
    ParadigmaTemperature getTemperature(ParadigmaTemperatureSelector_t sel);

    ulong               getBetriebsstundenKessel()      {
        return (ulong)m_Data2.BetriebsstundenKessel;
    }
    ulong               getAnzahlKesselstarts()         {
        return (ulong)m_Data2.AnzahlKesselstarts;
    }
    word                getStoercodeKessel()            {
        return (word)m_Data2.StoercodeKessel;
    }
    byte               getStoercodeFuehler()           {
        return (byte)m_Data2.StoercodeFuehler;
    }


    ParadigmaMonitorData& operator<<( char c );
    ParadigmaMonitorData& operator<<( char Buffer[] );

    void attach1( void (*fct)(void) )    {
        m_CallbackData1 = fct;
    }
    void attach2( void (*fct)(void) )   {
        m_CallbackData2 = fct;
    }
    
    
    MonDta1_t *  getData1()      { return &m_Data1; }
    MonDta2_t *  getData2()      { return &m_Data2; }
};
}

#endif

