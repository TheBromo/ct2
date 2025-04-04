/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zuercher Hochschule Winterthur   -
 * --  _| |_| | | | |____ ____) |  (University of Applied Sciences) -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Project     : CT2 lab - SPI Display
 * -- Description : Contains the implementations of the functions
 * --               to write and read frames from and to the TFT-LCD
 * --               display EAeDIPTFT43-A.
 * --
 * -- $Id: lcd_io.c 4720 2019-03-04 10:11:31Z akdi $
 * ------------------------------------------------------------------
 */
#include "lcd_io.h"
#include "hal_sbuf.h"
#include "hal_spi.h"
#include <stdint.h>

#define ACK_CHAR (uint8_t)0x06
#define DC1_CHAR (uint8_t)0x11
#define DC2_CHAR (uint8_t)0x12
#define ESC_CHAR (uint8_t)0x1B
#define ONE_CHAR (uint8_t)0x01

#define NOTHING_RECEIVED (uint8_t)0
enum { SUCCESS = 0, ERRORCODE = 1 };

/* ------------------------------------------------------------------
 * -- Function prototypes
 * ------------------------------------------------------------------
 */
static uint8_t send_read_display_buffer_request(void);

/* ------------------------------------------------------------------
 * -- Function implementations
 * ------------------------------------------------------------------
 */

/*
 * according to description in header file
 */
void init_display_interface(void) {
  hal_spi_init();
  hal_sbuf_init();
}

/*
 * according to description in header file
 */
uint8_t read_display_buffer(uint8_t *readBuffer) {
  /// STUDENTS: To be programmed
  uint8_t dataAvailable = hal_sbuf_get_state();
  if (dataAvailable == 0)
    return 0; // no data available
  
  uint8_t ack = send_read_display_buffer_request();
  if (ack != ACK_CHAR)
    return 0; // no data available


  // start read
  uint8_t checksum = 0;
  uint8_t dc1 = hal_spi_read_write(0x00);
  checksum += dc1;
  if (dc1 != DC1_CHAR) {
    return 0;
  }

  // read string length
  uint8_t length = hal_spi_read_write(0x00);
  checksum += length;

  // read string data
  for (int i = 0; i < length; i++) {
    uint8_t data = hal_spi_read_write(0x00);
    readBuffer[i] = data;
    checksum += data;
  }

  // check checksum
  uint8_t receivedChecksum = hal_spi_read_write(0x00);
  if (receivedChecksum != (checksum % 256))
    return 0;

  return length;
  /// END: To be programmed
}

/*
 * according to description in header file
 */
uint8_t write_cmd_to_display(const uint8_t *cmdBuffer, uint8_t length) {
  /// STUDENTS: To be programmed

  uint8_t checksum = DC1_CHAR + ESC_CHAR + length + 1;
  hal_spi_read_write(DC1_CHAR);
  hal_spi_read_write(length + 1);
  hal_spi_read_write(ESC_CHAR);

  for (int i = 0; i < length; i++) { // before we did not have -1
    hal_spi_read_write(cmdBuffer[i]);
    checksum += cmdBuffer[i];
  }
  hal_spi_read_write(checksum % 256);

  uint8_t result = hal_spi_read_write(0x00);
  if (result == ACK_CHAR) {
    return SUCCESS;
  }
  return ERRORCODE;

  /// END: To be programmed
}

/*
 * Assemble and send a packet to trigger the reading of the display buffer
 * Uses the sequence "<DC2>, 0x01, 0x53, checksum" according to datasheet
 * Check if the ACK was sent by the lcd and return this
 */
static uint8_t send_read_display_buffer_request(void) {
  /// STUDENTS: To be programmed
  uint8_t result;
  // send read request
  uint8_t checksum = DC2_CHAR + ONE_CHAR + 'S';
  hal_spi_read_write(DC2_CHAR);
  hal_spi_read_write(ONE_CHAR);
  hal_spi_read_write('S');
  hal_spi_read_write(checksum % 256);

  // check if read req is ack
  result = hal_spi_read_write(0x00);

  return result;
  /// END: To be programmed
}
