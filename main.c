#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <LUFA/Drivers/USB/USB.h>
#include "Descriptors.h"

#define CENTER 512
#define THRESHOLD 200

// pin untuk button (sudah dicoba dicocokkan dengan codenya JackofAllTrades tapi gatau)
#define BUTTON_U        PB0
#define BUTTON_F        PB1
#define BUTTON_D        PB2
#define BUTTON_B        PB3
#define BUTTON_1        PB4
#define BUTTON_2        PB5
#define BUTTON_3        PB6
#define BUTTON_4        PB7
#define ONOFF           PD0
#define ONOFF_JOYSTICK  PD1

USB_ClassInfo_HID_Device_t Joystick_HID_Interface = {
    .Config = {
        .InterfaceNumber = INTERFACE_ID_Joystick,
        .ReportINEndpoint = {
            .Address = JOYSTICK_EPADDR,
            .Size = JOYSTICK_EPSIZE,
            .Banks = 1,
        },
        .PrevReportINBuffer = NULL,
        .PrevReportINBufferSize = sizeof(USB_JoystickReport_Data_t),
    },
};

void SetupHardware(void) {
    MCUSR &= ~(1 << WDRF);
    USB_Init();

    DDRB = 0x00;
    PORTB = 0xFF;

    DDRD &= ~((1 << ONOFF) | (1 << ONOFF_JOYSTICK));
    PORTD |= (1 << ONOFF) | (1 << ONOFF_JOYSTICK);

    ADMUX = (1 << REFS0);
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);
}

uint16_t ReadADC(uint8_t channel) {
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return ADC;
}

int main(void) {
    SetupHardware();
    GlobalInterruptEnable();

    while (1) {
        HID_Device_USBTask(&Joystick_HID_Interface);
        USB_USBTask();
    }
}

bool CALLBACK_HID_Device_CreateHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo, uint8_t* const ReportID, const uint8_t ReportType, void* ReportData, uint16_t* const ReportSize) {

    USB_JoystickReport_Data_t* report = (USB_JoystickReport_Data_t*)ReportData;
    *ReportSize = sizeof(USB_JoystickReport_Data_t);

    bool enabled = !(PIND & (1 << ONOFF));
    bool useJoystick = !(PIND & (1 << ONOFF_JOYSTICK));

    if (!enabled) {
        report->Button = 0;
        report->X = 0;
        report->Y = 0;
        return true;
    }

    if (useJoystick) {
        uint16_t x = ReadADC(0); // PF0
        uint16_t y = ReadADC(1); // PF1

        report->X = 0;
        report->Y = 0;

        if (x > CENTER + THRESHOLD) report->X = 100;
        if (x < CENTER - THRESHOLD) report->X = -100;
        if (y > CENTER + THRESHOLD) report->Y = 100;
        if (y < CENTER - THRESHOLD) report->Y = -100;
    } else {
        report->X = 0;
        report->Y = 0;
        if (!(PINB & (1 << BUTTON_U))) report->Y = -100;
        if (!(PINB & (1 << BUTTON_D))) report->Y = 100;
        if (!(PINB & (1 << BUTTON_F))) report->X = 100;
        if (!(PINB & (1 << BUTTON_B))) report->X = -100;
    }

    // Action buttons
    report->Button = 0;
    if (!(PINB & (1 << BUTTON_1))) report->Button |= (1 << 0);
    if (!(PINB & (1 << BUTTON_2))) report->Button |= (1 << 1);
    if (!(PINB & (1 << BUTTON_3))) report->Button |= (1 << 2);
    if (!(PINB & (1 << BUTTON_4))) report->Button |= (1 << 3);

    return true;
}

void CALLBACK_HID_Device_ProcessHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo, const uint8_t ReportID, const uint8_t ReportType, const void* ReportData, const uint16_t ReportSize) {}
