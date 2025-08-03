#ifndef _ACTUATOR_H_
#define _ACTUATOR_H_

class Actuator {
public:
    virtual void begin() = 0;
    virtual void on() = 0;
    virtual void off() = 0;
    virtual bool isOn() = 0;
    virtual ~Actuator() = default;
};

#endif