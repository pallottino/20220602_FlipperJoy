/*******************************************************************************
Copyright 2016 Microchip Technology Inc. (www.microchip.com)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

To request to license the code under the MLA license (www.microchip.com/mla_license), 
please contact mla_licensing@microchip.com
*******************************************************************************/

/********************************************************************
-usb_descriptors.c-
-------------------------------------------------------------------
Filling in the descriptor values in the usb_descriptors.c file:
-------------------------------------------------------------------

[Device Descriptors]
The device descriptor is defined as a USB_CTRLDEVICE_DESCRIPTOR type.  
This type is defined in usb_ch9.h  Each entry into this structure
needs to be the correct length for the data type of the entry.

[Configuration Descriptors]
The configuration descriptor was changed in v2.x from a structure
to a uint8_t array.  Given that the configuration is now a byte array
each byte of multi-byte fields must be listed individually.  This
means that for fields like the total size of the configuration where
the field is a 16-bit value "64,0," is the correct entry for a
configuration that is only 64 bytes long and not "64," which is one
too few bytes.

The configuration attribute must always have the _DEFAULT
definition at the minimum. Additional options can be ORed
to the _DEFAULT attribute. Available options are _SELF and _RWU.
These definitions are defined in the usb_device.h file. The
_SELF tells the USB host that this device is self-powered. The
_RWU tells the USB host that this device supports Remote Wakeup.

[Endpoint Descriptors]
Like the configuration descriptor, the endpoint descriptors were 
changed in v2.x of the stack from a structure to a uint8_t array.  As
endpoint descriptors also has a field that are multi-byte entities,
please be sure to specify both bytes of the field.  For example, for
the endpoint size an endpoint that is 64 bytes needs to have the size
defined as "64,0," instead of "64,"

Take the following example:
  // Endpoint Descriptor //
  0x07,           //the size of this descriptor //
  USB_DESCRIPTOR_ENDPOINT,  //Endpoint Descriptor
  _EP02_IN,         //EndpointAddress
  _INT,           //Attributes
  0x08,0x00,          //size (note: 2 bytes)
  0x02,           //Interval

The first two parameters are self-explanatory. They specify the
length of this endpoint descriptor (7) and the descriptor type.
The next parameter identifies the endpoint, the definitions are
defined in usb_device.h and has the following naming
convention:
_EP<##>_<dir>
where ## is the endpoint number and dir is the direction of
transfer. The dir has the value of either 'OUT' or 'IN'.
The next parameter identifies the type of the endpoint. Available
options are _BULK, _INT, _ISO, and _CTRL. The _CTRL is not
typically used because the default control transfer endpoint is
not defined in the USB descriptors. When _ISO option is used,
addition options can be ORed to _ISO. Example:
_ISO|_AD|_FE
This describes the endpoint as an isochronous pipe with adaptive
and feedback attributes. See usb_device.h and the USB
specification for details. The next parameter defines the size of
the endpoint. The last parameter in the polling interval.

-------------------------------------------------------------------
Adding a USB String
-------------------------------------------------------------------
A string descriptor array should have the following format:

rom struct{byte bLength;byte bDscType;word string[size];}sdxxx={
sizeof(sdxxx),DSC_STR,<text>};

The above structure provides a means for the C compiler to
calculate the length of string descriptor sdxxx, where xxx is the
index number. The first two bytes of the descriptor are descriptor
length and type. The rest <text> are string texts which must be
in the unicode format. The unicode format is achieved by declaring
each character as a word type. The whole text string is declared
as a word array with the number of characters equals to <size>.
<size> has to be manually counted and entered into the array
declaration. Let's study this through an example:
if the string is "USB" , then the string descriptor should be:
(Using index 02)
rom struct{byte bLength;byte bDscType;word string[3];}sd002={
sizeof(sd002),DSC_STR,'U','S','B'};

A USB project may have multiple strings and the firmware supports
the management of multiple strings through a look-up table.
The look-up table is defined as:
rom const uint8_t *rom USB_SD_Ptr[]={&sd000,&sd001,&sd002};

The above declaration has 3 strings, sd000, sd001, and sd002.
Strings can be removed or added. sd000 is a specialized string
descriptor. It defines the language code, usually this is
US English (0x0409). The index of the string must match the index
position of the USB_SD_Ptr array, &sd000 must be in position
USB_SD_Ptr[0], &sd001 must be in position USB_SD_Ptr[1] and so on.
The look-up table USB_SD_Ptr is used by the get string handler
function.

-------------------------------------------------------------------

The look-up table scheme also applies to the configuration
descriptor. A USB device may have multiple configuration
descriptors, i.e. CFG01, CFG02, etc. To add a configuration
descriptor, user must implement a structure similar to CFG01.
The next step is to add the configuration descriptor name, i.e.
cfg01, cfg02,.., to the look-up table USB_CD_Ptr. USB_CD_Ptr[0]
is a dummy place holder since configuration 0 is the un-configured
state according to the definition in the USB specification.

********************************************************************/

/*********************************************************************
 * Descriptor specific type definitions are defined in:
 * usb_device.h
 *
 * Configuration options are defined in:
 * usb_config.h
 ********************************************************************/

/** INCLUDES *******************************************************/
#include "extinc.h"
#include "usb.h"
#include "usb_device_hid.h"

/** CONSTANTS ******************************************************/
/* Device Descriptor */
const USB_CTRLDEVICE_DESCRIPTOR device_dsc=
{
  0x12,           // Size of this descriptor in bytes
  USB_DESCRIPTOR_CTRLDEVICE,    // CTRLDEVICE descriptor type
  0x0110,         // USB Spec Release Number in BCD format = 1.10
  0x00,           // Class Code
  0x00,           // Subclass code
  0x00,           // Protocol code
  USB_EP0_BUFF_SIZE,      // Max packet size for EP0, see usb_config.h
  JEPI_VID,         // Vendor ID, see usb_config.h
  JEPI_PID,         // Product ID, see usb_config.h
  JEPI_BCD,         // Device release number in BCD format
  0x01,           // Manufacturer string index
  0x02,           // Product string index
  0x03,           // Device serial number string index
  0x01            // Number of possible configurations
};

/* Configuration 1 Descriptor */
const uint8_t configDescriptor1[]={
  /* Configuration Descriptor */
  0x09,           // (1 Byte) sizeof(USB_CFG_DSC) Size of this descriptor in bytes
  USB_DESCRIPTOR_CONFIGURATION, // (1 Byte) CONFIGURATION descriptor type
  //DESC_CONFIG_WORD (0x0022),  // (2 Byte) Total length of data for this cfg  con 1 solo endpoint
                                // 34 Byte
  DESC_CONFIG_WORD (0x0029),  // (2 Byte) Total length of data for this cfg 
                                // 41 Byte
  1,              // (1 Byte) Number of interfaces in this cfg
  1,              // (1 Byte) Index value of this configuration
  4,              // (1 Byte) Configuration string index
  _DEFAULT | _SELF, // (1 Byte) Attributes, see usb_device.h
  50,             // (1 Byte) Max power consumption (2X mA)

  /* Interface Descriptor */
  0x09,//sizeof(USB_INTF_DSC), // (1 Byte) Size of this descriptor in bytes
  USB_DESCRIPTOR_INTERFACE, // (1 Byte) INTERFACE descriptor type
  0,              // (1 Byte) Interface Number (From 0)
  0,              // (1 Byte) Alternate Setting Number
  2,              // (1 Byte) Number of endpoints in this intf
  HID_INTF,       // (1 Byte) Class code
  0,              // (1 Byte) Subclass code
  0x00,           // (1 Byte) Protocol code
  5,              // (1 Byte) Interface string index

  /* HID Class-Specific Descriptor */
  0x09,//sizeof(USB_HID_DSC)+3,  // (1 Byte) Size of this descriptor in bytes RRoj hack
  DSC_HID,         // (1 Byte) HID descriptor type
  0x00, 0x02,		   // (2 Byte) HID Spec Release Number in BCD format (2.00)
  0x00,           // (1 Byte) Country Code (0x00 for Not supported)
  HID_NUM_OF_DSC,       // (1 Byte) Number of class descriptors, see usbcfg.h
  DSC_RPT,          // (1 Byte) Report descriptor type
  DESC_CONFIG_WORD (HID_RPT01_SIZE), // (2 Byte) sizeof(hid_rpt01),    // Size of the report descriptor

                    // (1 Byte) Constant name specifing type of optionale descriptor
                    // (1 Byte) Numeric expression that is the total size of the optional descriptor

  /* Endpoint Descriptor 01 */
  0x07,           // (1 Byte) sizeof (USB_EP_DSC)
  USB_DESCRIPTOR_ENDPOINT,  // (1 Byte) Endpoint Descriptor
  HID_EP | _EP_IN,  //(1 Byte) EndpointAddress
  //HID_EP | _EP_IN,  //(1 Byte) EndpointAddress
  _INTERRUPT,         // (1 Byte) Attributes
  DESC_CONFIG_WORD(HID_INT_IN_EP_SIZE), // (2 Byte) size
  0x0a,           // (1 Byte) Interval 10 ms.
            // 34 Byte (0x22)   
  /* Endpoint Descriptor 01 */
  0x07,           // (1 Byte) sizeof (USB_EP_DSC)
  USB_DESCRIPTOR_ENDPOINT,  // (1 Byte) Endpoint Descriptor
  HID_EP | _EP_OUT,   //(1 Byte) EndpointAddress
  _INTERRUPT,         // (1 Byte) Attributes
  DESC_CONFIG_WORD(HID_INT_OUT_EP_SIZE), // (2 Byte) size
  0x06           // (1 Byte) Interval 6 ms.
  };          // 41 Byte (0x29)

//
// 2 x 8 pulsanti
//
// 6 Assi Max spostamento assi 0 - 127 - 255
//
// E:\Utility\Usb\dt2_4\Joy_Jamma_6U_8Bit.h
const uint8_t hid_rpt01 [HID_RPT01_SIZE] =
{
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x05,                    // USAGE (Gaming control)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x09, 0x31,                    //   USAGE (Y)  Asse Y UP-DW Count[1]
    0x09, 0x30,                    //   USAGE (X)  Asse X SX-DX Count[0]
    0x09, 0x32,                    //   USAGE (Z)  Asse Y DX    Count[2]
    0x09, 0x33,                    //   USAGE (Rx) Asse X DX    Count[3]
    0x09, 0x34,                    //   USAGE (Ry) Asse Knob    Count[4]
    0x09, 0x35,                    //   USAGE (Rz)
    0x15, 0x81,                    //   LOGICAL_MINIMUM (-128)
    0x25, 0x7f,                    //   LOGICAL_MAXIMUM (127)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x06,                    //   REPORT_COUNT (6)
    0x65, 0x00,                    //   UNIT (None)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x05, 0x09,                    //   USAGE_PAGE (Button)
    0x19, 0x01,                    //   USAGE_MINIMUM (Button 1)
    0x29, 0x10,                    //   USAGE_MAXIMUM (Button 16)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x10,                    //   REPORT_COUNT (16)
    0x55, 0x00,                    //   UNIT_EXPONENT (0)
    0x65, 0x00,                    //   UNIT (None)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0xc0                           // END_COLLECTION
};            //  51 Byte HID_RPT01_SIZE

/*! \brief Report Descriptor 
const uint8_t hid_rpt01 [HID_RPT01_SIZE] =
{
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x04,                    // USAGE (Joystick)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x09, 0x31,                    //   USAGE (Y)  Asse Y UP-DW Count[1]
    0x09, 0x30,                    //   USAGE (X)  Asse X SX-DX Count[0]
    0x09, 0x32,                    //   USAGE (Z)  Asse Y DX    Count[2]
    0x09, 0x33,                    //   USAGE (Rx) Asse X DX    Count[3]
    0x09, 0x34,                    //   USAGE (Ry) Asse Knob    Count[4]
    0x09, 0x35,                    //   USAGE (Rz)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x35, 0x00,                    //   PHYSICAL_MINIMUM (0)
    0x46, 0xff, 0x00,              //   PHYSICAL_MAXIMUM (255)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x06,                    //   REPORT_COUNT (6)
    0x65, 0x00,                    //   UNIT (None)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x05, 0x09,                    //   USAGE_PAGE (Button)
    0x19, 0x01,                    //   USAGE_MINIMUM (Button 1)
    0x29, 0x10,                    //   USAGE_MAXIMUM (Button 16)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x10,                    //   REPORT_COUNT (16)
    0x55, 0x00,                    //   UNIT_EXPONENT (0)
    0x65, 0x00,                    //   UNIT (None)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0xc0                           // END_COLLECTION
};            //  57 Byte HID_RPT01_SIZE


const uint8_t hid_rpt01 [HID_RPT01_SIZE] =
{
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x05,                    // USAGE (Game Pad)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x09, 0x30,                    //   USAGE (X)
    0x09, 0x31,                    //   USAGE (Y)
    0x09, 0x32,                    //   USAGE (Z)
    0x09, 0x33,                    //   USAGE (Rx)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x46, 0xff, 0x00,              //   PHYSICAL_MAXIMUM (255)
    0x35, 0x00,                    //   PHYSICAL_MINIMUM (0)
    0x46, 0xff, 0x00,              //   PHYSICAL_MAXIMUM (255)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x04,                    //   REPORT_COUNT (4)
    0x65, 0x00,                    //   UNIT (None)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x05, 0x09,                    //   USAGE_PAGE (Button)
    0x19, 0x01,                    //   USAGE_MINIMUM (Button 1)
    0x29, 0x20,                    //   USAGE_MAXIMUM (Button 32)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x20,                    //   REPORT_SIZE (32)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x55, 0x00,                    //   UNIT_EXPONENT (0)
    0x65, 0x00,                    //   UNIT (None)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0xc0                           // END_COLLECTION
};            //  53 Byte HID_RPT01_SIZE
*/

//Language code string descriptor
const struct{uint8_t bLength;uint8_t bDscType;uint16_t string[1];}sd000={
sizeof(sd000),USB_DESCRIPTOR_STRING,{0x0409
}};

//Manufacturer string descriptor
const struct{uint8_t bLength;uint8_t bDscType;uint16_t string[21];}sd001={
sizeof(sd001),USB_DESCRIPTOR_STRING,
{'J','E','.','P','i','.',' ','J','o','y','-','1','0','-','0','4','-','2','0','2','2'}}; // 21 Byte

#if defined JAMMAJOYINREADY
  //Product string descriptor             // 22 Byte
  const struct{uint8_t bLength;uint8_t bDscType;uint16_t string [22];} sd002 = {
  sizeof(sd002),USB_DESCRIPTOR_STRING,
  {'J','E','.','P','i','.',' ','J','a','m','m','a',' ','J','o','y',' ','I','n','.',' ',' '}};
#elif defined JAMMAJOYOUTREADY
  //Product string descriptor             // 22 Byte
  const struct{uint8_t bLength;uint8_t bDscType;uint16_t string [22];} sd002 = {
  sizeof(sd002),USB_DESCRIPTOR_STRING,
  {'J','E','.','P','i','.',' ','J','a','m','m','a',' ','J','o','y',' ','O','u','t','.',' '}};
#elif defined TOMMYJOYREADY || defined JAMMAJOYOUTREADY
  //Product string descriptor             // 22 Byte
  const struct{uint8_t bLength;uint8_t bDscType;uint16_t string [22];} sd002 = {
  sizeof(sd002),USB_DESCRIPTOR_STRING,
  {'J','E','.','P','i','.',' ','T','o','m','m','y',' ','J','o','y','s','t','i','c','k','.'}};
#elif defined ESKYJOYREADY
  //Product string descriptor             // 22 Byte
  const struct{uint8_t bLength;uint8_t bDscType;uint16_t string [22];} sd002 = {
  sizeof(sd002),USB_DESCRIPTOR_STRING,
  {'J','E','.','P','i','.',' ','E','s','k','y',' ',' ','J','o','y','s','t','i','c','k','.'}};
  #elif defined FLIPPERJOYREADY
  //Product string descriptor             // 22 Byte
  const struct{uint8_t bLength;uint8_t bDscType;uint16_t string [22];} sd002 = {
  sizeof(sd002),USB_DESCRIPTOR_STRING,
  {'J','E','.','P','i','.','F','l','i','p','p','e','r','J','o','y','s','t','i','c','k','.'}};
#else
  #error "Definire tipo JOY"
  #endif

//Serial string descriptor                // 17 Byte
const struct{uint8_t bLength;uint8_t bDscType;uint16_t string [17];} sd003 = {
sizeof(sd003),USB_DESCRIPTOR_STRING,
{'S','e','r','.',' ','n','.','-','A','0','0','0','0','0','0','3','2'}};

//Config string descriptor              // 17 Byte
const struct{uint8_t bLength;uint8_t bDscType;uint16_t string [17];} sd004 = {
sizeof(sd004),USB_DESCRIPTOR_STRING,
{'8',' ','b','i','t',' ','0',' ','t','o',' ','2','5','5','.',' ',' '}};

//Interface string descriptor              // 18 Byte
const struct{uint8_t bLength;uint8_t bDscType;uint16_t string [18];} sd005 = {
sizeof(sd005),USB_DESCRIPTOR_STRING,
{'I','n','t','e','r','f','a','c','e',' ','N','u','m','b','e','r','.','1'}};

//Array of configuration descriptors
const uint8_t *const USB_CD_Ptr[]=
{
  (const uint8_t *const) configDescriptor1
};

//Array of string descriptors
const uint8_t *const USB_SD_Ptr[]=
{
  (const uint8_t *const) & sd000,
  (const uint8_t *const) & sd001,
  (const uint8_t *const) & sd002,
  (const uint8_t *const) & sd003,
  (const uint8_t *const) & sd004,
  (const uint8_t *const) & sd005
};

/** EOF usb_descriptors.c ***************************************************/

