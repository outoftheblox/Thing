#ifndef _BlinkPattern_H_
#define _BlinkPattern_H_

#include <Arduino.h>

namespace g3rb3n
{

  class BlinkPattern 
  {
    public:
      template <int N>
      struct Pattern
      {
        uint8_t pattern[N];
        uint16_t duration;
        static const uint8_t size = N;
      };

    private:
      uint8_t* pattern;
      uint16_t duration;
      uint8_t size = 0;
      uint8_t pin;
      uint32_t nextChange;
      uint8_t index;
      
    public:
      BlinkPattern(uint8_t pin);
      ~BlinkPattern();
      template <int N>
      void setPattern(const Pattern<N>& p)
      {
        pattern = (uint8_t*)p.pattern;
        size = p.size;
        duration = p.duration;
        index = 0;
        nextChange = millis();
      }
      void begin();
      void handle();
      
  };
}

#endif
