/*
 *  DHT Library for  Digital-output Humidity and Temperature sensors
 *
 *  Works with DHT11, DHT22 Nucleo Board tested on F103RB
 *
 *  Copyright (C) Wim De Roeve 
 *                ported to work on Nucleo Board:
 *                                              Moises Marangoni
 *                                              Somlak Mangnimit
 *                based on DHT22 sensor library by HO WING KIT
 *                Arduino DHT11 library 
 */

#include "DHT.h"
#include "mbed.h"

#define DHT_DATA_BIT_COUNT 41

//DigitalInOut data_pin(PA_10);

DHT::DHT(PinName pin, int DHTtype) {
    _pin = pin;
    _DHTtype = DHTtype;
}

DHT::~DHT() {
}

int DHT::readData() {
    Timer tmr;
    DigitalInOut data_pin(_pin);
    // BUFFER TO RECEIVE
    //uint8_t bits[5];
    uint8_t cnt = 7;
    uint8_t idx = 0;
    
    tmr.stop();
    tmr.reset();

    // EMPTY BUFFER
    for(int i=0; i< 5; i++) bits[i] = 0;

    // REQUEST SAMPLE
    data_pin.output();
    data_pin.write(0);
    ThisThread::sleep_for(18ms);
    data_pin.write(1);
    wait_us(40);
    data_pin.input();

    // ACKNOWLEDGE or TIMEOUT
    unsigned int loopCnt = 10000;
    
    while(!data_pin.read())if(!loopCnt--)return DHTLIB_ERROR_TIMEOUT;

    loopCnt = 10000;
    
    while(data_pin.read())if(!loopCnt--)return DHTLIB_ERROR_TIMEOUT;

    // READ OUTPUT - 40 BITS => 5 BYTES or TIMEOUT
    for(int i=0; i<40; i++){
        
        loopCnt = 10000;
        
        while(!data_pin.read())if(loopCnt-- == 0)return DHTLIB_ERROR_TIMEOUT;

        //unsigned long t = micros();
        tmr.start();

        loopCnt = 10000;
        
        while(data_pin.read())if(!loopCnt--)return DHTLIB_ERROR_TIMEOUT;

        if (tmr.elapsed_time().count() > 40) bits[idx] |= (1 << cnt);
        
        tmr.stop();
        tmr.reset();
        
        if(cnt == 0){   // next byte?
        
            cnt = 7;    // restart at MSB
            idx++;      // next byte!
            
        }else cnt--;
        
    }
    // WRITE TO RIGHT VARS
    // as bits[1] and bits[3] are allways zero they are omitted in formulas.
    //humidity    = bits[0]; 
    //temperature = bits[2]; 

    //uint8_t sum = bits[0] + bits[2]; // Esto es para el DHT11 papu, no para el DHT22
    uint8_t sum = bits[0] + bits[1] + bits[2] + bits[3];
  

    if(bits[4] != sum)return DHTLIB_ERROR_CHECKSUM;
    
    return DHTLIB_OK;
}
float DHT::ReadTemperature() {
    //int retornotemp;

    switch (_DHTtype) {
        case DHT11:
            temperature = bits[2];
            return float(temperature);
        case DHT22:
            temperature = bits[2] & 0x7F;
            temperature *= 256;
            temperature += bits[3];
            temperature /= 10;
           if (bits[2] & 0x80)
            {temperature *= -1;}
            return float(temperature);
    }
    return 0;
}

float DHT::ReadHumidity() {
    if (_DHTtype == DHT22) {
        return (bits[0] * 256 + bits[1]) / 10.0f;
    } else if (_DHTtype == DHT11) {
        return float(bits[0]);
    }
    return 0.0f;
}

/* Version original
int DHT::ReadHumidity() {
    //int v;

    switch (_DHTtype) {
        case DHT11:
            humidity = bits[0];
            return float(humidity);
        case DHT22:
            humidity = bits[0];
            humidity *= 256;
            humidity += bits[1];
            humidity /= 10;
            return float(humidity);
    }
    return 0;
}*/