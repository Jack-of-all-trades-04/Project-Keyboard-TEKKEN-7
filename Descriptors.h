#ifndef _DESCRIPTORS_H_
#define _DESCRIPTORS_H_

#include <LUFA/Drivers/USB/USB.h>

#define INTERFACE_ID_Joystick 0
#define JOYSTICK_EPADDR       (ENDPOINT_DIR_IN | 1)
#define JOYSTICK_EPSIZE       8

typedef struct
{
    uint8_t Button;  // Buttons untuk bitmask
    int8_t  X;       // Sumbu X
    int8_t  Y;       // Sumbu Y
} USB_JoystickReport_Data_t;

extern const USB_Descriptor_HIDReport_Datatype_t PROGMEM JoystickReport[];

extern const USB_Descriptor_Device_t PROGMEM DeviceDescriptor;

// Configuration descriptor
typedef struct
{
    USB_Descriptor_Configuration_Header_t Config;
    USB_Descriptor_Interface_t             HID_Interface;
    USB_HID_Descriptor_HID_t               HID_Joystick;
    USB_Descriptor_Endpoint_t              HID_ReportINEndpoint;
} USB_Descriptor_Configuration_t;

extern const USB_Descriptor_Configuration_t PROGMEM ConfigurationDescriptor;

extern const USB_Descriptor_String_t PROGMEM LanguageString;
extern const USB_Descriptor_String_t PROGMEM ManufacturerString;
extern const USB_Descriptor_String_t PROGMEM ProductString;

uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue, const uint16_t wIndex, const void** const DescriptorAddress) ATTR_WARN_UNUSED_RESULT ATTR_NON_NULL_PTR_ARG(3);

#endif
