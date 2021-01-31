
#ifndef RPS40ST_h
#define RPS40ST_h

//instance for use cpt type RPS40ST
class RPS40ST
{
    //constructor
    public:
        RPS40ST(void):m_anaPin(0) , m_r0(1.0f), m_offset(.0f) , m_aCalib(-1.44928f) , m_bCalib(3.525f) 
        {
        }

    //setter
    public:
        void SetOffset(float _off)
        {
            this->m_offset = _off;
        }

        //set parameter for calbiration 
        void SetCalibrate(float _r0 , float _factor =-1.44928f, float _add =3.525f)
        {
            this->m_r0 = _r0;
            this->m_aCalib =_factor;
            this->m_bCalib = _add;
        }

        void Init(const uint8_t _analog )
        {
            this->m_anaPin = _analog;

        }

        bool Tare(void)
        {
          short nbPoint = 20;
          float value = 0.0;
          
          for(int i = 0 ; i< nbPoint ; i++)
          {
            value =+ this->ReadWeight();
            delay(25);
          }

          value /= nbPoint;

          // if tare exssessive value , dont apply
          if(value > 10000 or isnan(value))
            return false;

          this->m_offset = -value;

          return true;
          
        }
    //getteur
    public: 
        // return value in gramme
        float ReadWeight(void)
        {
            float value = this->ReadTension();

            float Rs = (this->m_r0 /1000.0) *((  5.0f/ value )- 1)  ;

            //convert to gramme , use data sheet
            //(10^((-B) / A) / Rs(x))^(-A)
            value = pow(pow( 10 , ( (-this->m_bCalib)/ this->m_aCalib) )  / Rs ,(-this->m_aCalib)) ; 

            value+= this->m_offset;

            return value;
        }

        float ReadAverageWeight(short _nbPoint , int _delay)
        {
          short nbPoint = _nbPoint;
          float value = 0.0;
          
          for(int i = 0 ; i< _nbPoint ; i++)
          {
            value =+ this->ReadWeight();
            delay(_delay);
          }

          value /= _nbPoint;
          return value;
        }

        float ReadTension(void)
        {
            //read adc
            float value = analogRead(this->m_anaPin);

            //convert in Volt
            return 5.0  * value / 1024.0 ;
        }

    private :
        uint8_t m_anaPin;
        float m_r0 ; 
        float m_offset;
        float m_aCalib;
        float m_bCalib;

};

#endif
