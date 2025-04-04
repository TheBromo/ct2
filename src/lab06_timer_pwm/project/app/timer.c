/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 * --
 * -- Description:  Implementation of module timer.
 * --
 * -- $Id: timer.c 5605 2023-01-05 15:52:42Z frtt $
 * ------------------------------------------------------------------------- */

/* standard includes */
#include <reg_stm32f4xx.h>
#include <stdint.h>

/* user includes */
#include "timer.h"

/* -- Macros
 * ------------------------------------------------------------------------- */

#define PERIPH_GPIOB_ENABLE (0x00000002)
#define PERIPH_TIM3_ENABLE (0x00000002)
#define PERIPH_TIM4_ENABLE (0x00000004)

/* Configuring pins used for PWM: PA.0, PA.4, PA.5 */
#define GPIOB_MODE_AF (0x00000a02)
#define GPIOB_MODE_CLEAR (0xfffff0f0)

#define GPIOB_SPEED_100MHZ (0x00000f03)
#define GPIOB_SPEED_CLEAR (0xfffff0f0)

#define GPIOB_AF_TIM3 (0x00220002)
#define GPIOB_AF_CLEAR (0xff00fff0)

/* -- Macros for timer configuration
 * ------------------------------------------------------------------------- */
#define NVIC_IRQ_TIM4 (0x40000000) // NVIC mask for interrupt
#define TIM4_IRQ_UIF (0x00000001)  // TIM4 mask for update irq flag

/* Add additional macros as used by your code */
/// STUDENTS: To be programmed

/// END: To be programmed

/* Public function definitions
 * ------------------------------------------------------------------------- */

/*
 * See header file
 */
void tim3_init(void) {
  /* Enable peripheral clocks */

  RCC->APB1ENR |= PERIPH_TIM3_ENABLE;
  RCC->AHB1ENR |= PERIPH_GPIOB_ENABLE;

  /* Initialize GPIO pins */
  GPIOB->MODER &= GPIOB_MODE_CLEAR;
  GPIOB->MODER |= GPIOB_MODE_AF;

  GPIOB->OSPEEDR &= GPIOB_SPEED_CLEAR;
  GPIOB->OSPEEDR |= GPIOB_SPEED_100MHZ;

  GPIOB->AFRL &= GPIOB_AF_CLEAR;
  GPIOB->AFRL |= GPIOB_AF_TIM3;

  /* initialize timer registers to default values; the values are suitable
     for the basic timer modes used in this lab */

  TIM3->CR2 = 0x0000;
  TIM3->SMCR = 0x0000; // selects CK_INT as source
  TIM3->DIER = 0x0000; // disables all interrupts

  /* add specific configuration for timer3 */
  /// STUDENTS: To be programmed
  TIM3->CR1 = 0x0000; // up counting
  TIM3->PSC = 6;      // prescaler 7 - 1
  TIM3->ARR = 60000;  // auto reload register

  TIM3->DIER = 0x1; // enable interrupt
  TIM3->EGR = 0x1;  // enable reload prescaler

  TIM3->CCMR1 |= 0x6060; // set mode OC2M = 1, OC1M = 1
  TIM3->CCMR2 |= 0x0060; // set mode OC4M = 0, OC3M = 1

  TIM3->CCER |= 0x0111;

  TIM3->CR1 |= 0x1; // enable

  /// END: To be programmed
}

/*
 * See header file
 */
void tim4_init(void) {
  /* Enable peripheral clock */
  RCC->APB1ENR |= PERIPH_TIM4_ENABLE;

  /* initialize timer registers to default values; the values are suitable
     for the basic timer modes used in this lab */
  TIM4->CR2 = 0x0000;
  TIM4->SMCR = 0x0000; // selects CK_INT as source

  /* add specific configuration for timer4 */
  /// STUDENTS: To be programmed

  TIM4->ARR = 0x1000;   // each sesond
  TIM4->PSC = 8400 - 1; // 8399
  TIM4->CR1 = 0x0010;   // down counting
  TIM4->DIER = 0x1;     // enable interrupt
  TIM4->EGR = 0x1;      // enable reload prescaler
  TIM4->CR1 |= 0x1;     // enable

  /// END: To be programmed

  /* Enable TIM4 interrupt in NVIC */
  NVIC->ISER0 |= NVIC_IRQ_TIM4;
}

/*
 * See header file
 */
void tim4_reset_uif(void) { TIM4->SR &= (uint32_t)~TIM4_IRQ_UIF; }

/*
 * See header file
 */
void tim3_set_compare_register(pwm_channel_t channel, uint16_t value) {
  /// STUDENTS: To be programmed

  switch (channel) {
  case PWM_CH1:
    TIM3->CCR1 = value;
    break;
  case PWM_CH2:
    TIM3->CCR2 = value;
    break;
  case PWM_CH3:
    TIM3->CCR3 = value;
    break;
  case PWM_CH4:
    TIM3->CCR4 = value;
    break;
  }
  /// END: To be programmed
}
