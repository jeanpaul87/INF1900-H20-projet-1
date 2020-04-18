#include "InterruptButton.h"

InterruptButton::InterruptButton() {}

void InterruptButton::initInterruptButton0(Edges edge){
    // do not allow interrupt during init
    cli();
    // interrupt mask
    EIMSK |= (1 << INT0);
    switch (edge){
        // by default, the EICRA register is 0, 0
        case LowLevel:
            break;
        // ISR activated by both a rising and a falling edge
        case AnyEdge:
            EICRA |= (1 << ISC00);
            break;
        // ISR activated by falling edge
        case FallingEdge:
            EICRA |= (1 << ISC01);
            break;
        // ISR activated by rising edge
        case RisingEdge:
            EICRA |= (1 << ISC01) | (1 << ISC00);
            break;
        default:
            break;
    }
    // allow interruptions again
    sei();
}

void InterruptButton::initInterruptButton1(Edges edge){
    // do not allow interrupt during init
    cli();
    // interrupt mask
    EIMSK |= (1 << INT1);
    switch (edge){
        // by default, the EICRA register is 0, 0
        case LowLevel:
            break;
        // ISR activated by both a rising and a falling edge
        case AnyEdge:
            EICRA |= (1 << ISC10);
            break;
        // ISR activated by falling edge
        case FallingEdge:
            EICRA |= (1 << ISC11);
            break;
        // ISR activated by rising edge
        case RisingEdge:
            EICRA |= (1 << ISC11) | (1 << ISC10);
            break;
        default:
            break;
    }
    // allow interruptions again
    sei();
}

void InterruptButton::initInterruptButton2(Edges edge){
    // do not allow interrupt during init
    cli();
    // interrupt mask
    EIMSK |= (1 << INT2);
    switch (edge){
        // by default, the EICRA register is 0, 0
        case LowLevel:
            break;
        // ISR activated by both a rising and a falling edge
        case AnyEdge:
            EICRA |= (1 << ISC20);
            break;
        // ISR activated by falling edge
        case FallingEdge:
            EICRA |= (1 << ISC21);
            break;
        // ISR activated by rising edge
        case RisingEdge:
            EICRA |= (1 << ISC21) | (1 << ISC20);
            break;
        default:
            break;
    }
    // allow interruptions again
    sei();
}

// these should be used at the end of ISR
void InterruptButton::raiseFlagButton0(){
    EIFR |= (1 << INTF0); 
}

void InterruptButton::raiseFlagButton1(){
    EIFR |= (1 << INTF1); 
}

void InterruptButton::raiseFlagButton2(){
    EIFR |= (1 << INTF2); 
}

bool InterruptButton::debounce(){
    bool isPushed = (PIND & 0x04);
    _delay_ms(10);
    bool isStillPushed = (PIND & 0x04);
    return isPushed && isStillPushed;
}