// serial port functions (over micro USB on STM32F4 Discovery Board) 
// based on http://vedder.se/2012/07/usb-serial-on-stm32f4 , https://www.das-labor.org/trac/browser/microcontroller/src-stm32f4xx/serialUSB , http://stm32f4-discovery.com/2014/08/library-24-virtual-com-port-vcp-stm32f4xx/
// On Windows, use STM32 Virtual COM Port Driver (http://www.st.com/web/en/catalog/tools/PF257938#) on computer to connect to serial port over USB connection
// On Linux and Mac, you do not need any separate driver -- on Linux, the device will typically appear as /dev/ttyACM0; on Mac, the device will typically appear as /dev/tty.usbmodemfd121


#include "usbd_cdc_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usbd_cdc_vcp.h"

#include "serial_port_usb.h"

__ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;

// initialize serial port via micro USB on STM32F4 Discovery Board
void init_serial_port_usb()
{

  USBD_Init(&USB_OTG_dev,
            USB_OTG_FS_CORE_ID,
            &USR_desc,
            &USBD_CDC_cb,
            &USR_cb);
}


// read a byte from the serial port (via micro USB); byte is returned in *c; function returns 1 if successfully read, 0 if not successfully read
uint8_t read_serial_usb_byte(uint8_t *c) 
{
  return VCP_get_char(c);
}

// writes a sequence of n bytes (i.e., a char array) to the serial port (via micro USB)
// n should be the number of bytes; a should point to an array containing n bytes to be written
void write_serial_usb_bytes(uint8_t *a, int n)
{
  VCP_send_buffer(a, n);
}
