// balance a partire de 3 jauges 
// by Jerome Favrou ( compagny SYSJF )
// all right reserd as client Tibault Cour 
// janvier 2021

//for I2C

#include <RPS40ST.h>

#define DEBUG 

#define VERSION 1.00 //1er release
//#define VERSION 0.01 // (no test version)

//definition type of pin
typedef  const uint8_t Pin_t;

#ifdef DEBUG
    #define SPEED_UART0 19200  // bauds rates of usb port, serial port 
#endif


//pin use for read cpt
Pin_t anlogPins= A0  ;

//value of resitance for cpt (ohm)
float r0Value = 5200.0f;

//creat instance of cpt 
RPS40ST * cpt;

float balanceValue;

void setup()
{

    #ifdef DEBUG
        Serial.begin(SPEED_UART0) ;
        while(!Serial){;}
        Serial.print(F("Programme Version: "));
        Serial.println(VERSION);
    #endif

    //cpt initialistion

    cpt = new RPS40ST();
    cpt->SetOffset(0.0f);  // offset corrector 
    cpt->SetCalibrate(r0Value  );  // resitace value R0 and factor corrector
    cpt->Init(anlogPins );

    #ifdef DEBUG
        Serial.println(F("setup end"));
    #endif
}

void loop()
{
    delay(1000);

    float tmpWeightValue =.0f;
    float tmpTensionValue =.0f;

    int nbPoint = 20;

    for(int i =0 ; i < nbPoint; i++)
    {
        tmpWeightValue += cpt->ReadWeight();
        tmpTensionValue += cpt->ReadTension();
        delay(50);
    }

    tmpWeightValue /= nbPoint;
    tmpTensionValue /= nbPoint;

    
    #ifdef DEBUG
        Serial.print(tmpTensionValue);
        Serial.print( F(" Volts => ") );
    
        Serial.print( tmpVatmpWeightValuelue );
        Serial.println(" Grammes");
    #endif
    }
}
