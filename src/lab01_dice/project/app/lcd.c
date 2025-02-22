/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zurich University of             -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                 -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Description:  Implementation of module lcd
 * --               Performs all the interactions with the lcd
 * --
 * -- $Id: lcd.c 5144 2020-09-01 06:17:21Z ruan $
 * ------------------------------------------------------------------
 */

/* standard includes */
#include <stddef.h>
#include <stdio.h>

/* user includes */
#include "lcd.h"
#include "reg_ctboard.h"

/* macros */
#define LCD_ADDR_LINE1 0u
#define LCD_ADDR_LINE2 20u

#define NR_OF_CHAR_PER_LINE 20u

#define LCD_CLEAR "                    "

/// STUDENTS: To be programmed

/*
 * \brief  Writes 'value' to the indicated 'position' on the lcd
 *
 * \param  slot_nr: A number between 1 and 6 indicating at which one of the
 *         six available slots on the lcd the 'value' shall be printed. The
 *         values at the other 5 slots remain unchanged.
 *
 * \param  value: The value to be printed
 */
void lcd_write_value(uint8_t slot_nr, uint8_t value) {
  if (slot_nr > 6 || slot_nr < 1)
    return;
  size_t index = (slot_nr - 1) * 3 + LCD_ADDR_LINE1;
  snprintf(&CT_LCD->ASCII[index], 3, "%2d", value);
}

/*
 * \brief  Writes an explanatory string followed by 'total_value' on the lcd.
 *
 * \param  total_value: The value to be printed
 */
void lcd_write_total(uint8_t total_value) {
    snprintf(&CT_LCD->ASCII[LCD_ADDR_LINE2], NR_OF_CHAR_PER_LINE,
           "total throws: %d", total_value);
}

/*
 * \brief  Clears the lcd and switches it to light green.
 */
void hal_ct_lcd_clear(void) {
  CT_LCD->BG.RED = 0x0000;
  CT_LCD->BG.GREEN = 0xffff;
  CT_LCD->BG.BLUE = 0x0000;

  snprintf(&CT_LCD->ASCII[LCD_ADDR_LINE1], NR_OF_CHAR_PER_LINE, LCD_CLEAR);
  snprintf(&CT_LCD->ASCII[LCD_ADDR_LINE2], NR_OF_CHAR_PER_LINE, LCD_CLEAR);
}

/// END: To be programmed
