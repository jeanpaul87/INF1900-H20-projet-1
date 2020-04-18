#ifndef INTERRUPT_BUTTON_H
#define INTERRUPT_BUTTON_H

#define F_CPU 8000000L

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
enum Edges {LowLevel, AnyEdge, FallingEdge, RisingEdge};

class InterruptButton{
public:
    InterruptButton();
    ~InterruptButton(){}
    void initInterruptButton0(Edges edge);
    void initInterruptButton1(Edges edge);
    void initInterruptButton2(Edges edge);

    void raiseFlagButton0();
    void raiseFlagButton1();
    void raiseFlagButton2();

    bool debounce();

};

#endif