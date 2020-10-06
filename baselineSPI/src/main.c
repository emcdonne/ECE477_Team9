/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f0xx.h"
#include "stm32f0308_discovery.h"

void GPIO_Init(void);
void SPI_Init(void);
void ili9341_hspi_init(SPI_TypeDef *SPIx);

int main(void)
{
	// RCC setup
	RCC->AHBENR   |= RCC_AHBENR_GPIOAEN;
	RCC->AHBENR   |= RCC_AHBENR_GPIOBEN;
	RCC->AHBENR   |= RCC_AHBENR_GPIOCEN;

	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

	GPIOC-> MODER |= (1<<16);
	GPIOC-> ODR |= (1<<8);

	int i;

	GPIO_Init();
	SPI_Init();
	ili9341_hspi_init(SPI1);

    // Main loop - empty the screen as a test.
    int tft_iter = 0;
    int tft_on = 0;
    // Set column range.
    hspi_cmd(SPI1, 0x2A);
    hspi_w16(SPI1, 0x0000);
    hspi_w16(SPI1, (uint16_t)(239));
    // Set row range.
    hspi_cmd(SPI1, 0x2B);
    hspi_w16(SPI1, 0x0000);
    hspi_w16(SPI1, (uint16_t)(319));
    // Set 'write to RAM'
    hspi_cmd(SPI1, 0x2C);

	while(1)
	{
		// Write 320 * 240 pixels.
		for (tft_iter = 0; tft_iter < (320*240); ++tft_iter) {
			// Write a 16-bit color.
		    if (tft_on) {
		    	//hspi_w16(SPI1, 0xF800);
		    	hspi_w16(SPI1, 0x0280);
		     }
		     else {
		       //hspi_w16(SPI1, 0x001F);
		    	 hspi_w16(SPI1, 0x0280);
		     }
		}
		tft_on = !tft_on;

	    for (i=0; i< 500000; i++);
	    GPIOC-> ODR |= (1<<8);
	    for (i=0; i< 500000; i++);
	    GPIOC-> ODR &= ~(1<<8);

	}
}

// Simple delay method, with instructions not to optimize.
// It doesn't accurately delay a precise # of cycles,
// it's just a rough scale.
void __attribute__((optimize("O0"))) delay_cycles(uint32_t cyc) {
  uint32_t d_i;
  for (d_i = 0; d_i < cyc; ++d_i) {
    asm("NOP");
  }
}

void GPIO_Init(void){
    // Define GPIOB pin mappings for software '4-wire' SPI.
    #define PB_MOSI (5)
    #define PB_SCK  (3)
    #define PB_DC   (4)
    #define PA_CS   (12)
    #define PA_RST  (15)
    GPIOB->MODER   &= ~((0x3 << (PB_MOSI * 2)) |
                        (0x3 << (PB_SCK  * 2)) |
                        (0x3 << (PB_DC   * 2)));
    // Set the MOSI and SCK pins to alternate function mode 0.
    // Set D/C to normal output.
      GPIOB->AFR[0]  &= ~(GPIO_AFRL_AFSEL3 |
                          GPIO_AFRL_AFSEL5);
    GPIOB->MODER   |=  ((0x2 << (PB_MOSI * 2)) |
                        (0x2 << (PB_SCK  * 2)) |
                        (0x1 << (PB_DC   * 2)));
    // Use pull-down resistors for the SPI peripheral?
    // Or no pulling resistors?
    GPIOB->PUPDR   &= ~((0x3 << (PB_MOSI * 2)) |
                        (0x3 << (PB_SCK  * 2)) |
                        (0x3 << (PB_DC   * 2)));
    GPIOB->PUPDR  |=   ((0x1 << (PB_MOSI * 2)) |
                        (0x1 << (PB_SCK  * 2)));
    // Output type: Push-pull
    GPIOB->OTYPER  &= ~((0x1 << PB_MOSI) |
                        (0x1 << PB_SCK)  |
                        (0x1 << PB_DC));
    // High-speed - 50MHz maximum
    // (Setting all '1's, so no need to clear bits first.)
    GPIOB->OSPEEDR |=  ((0x3 << (PB_MOSI * 2)) |
                        (0x3 << (PB_SCK  * 2)) |
                        (0x3 << (PB_DC   * 2)));
    // Initialize the GPIOA pins; ditto.
    GPIOA->MODER   &= ~((0x3 << (PA_CS   * 2)) |
                        (0x3 << (PA_RST  * 2)));
    GPIOA->MODER   |=  ((0x1 << (PA_CS   * 2)) |
                        (0x1 << (PA_RST  * 2)));
    GPIOA->OTYPER  &= ~((0x1 << PA_CS) |
                        (0x1 << PA_RST));
    GPIOA->PUPDR   &= ~((0x3 << (PA_CS  * 2)) |
                        (0x3 << (PA_RST * 2)));

    // Set initial pin values.
    //   (The 'Chip Select' pin tells the display if it
    //    should be listening. '0' means 'hey, listen!', and
    //    '1' means 'ignore the SCK/MOSI/DC pins'.)
    GPIOA->ODR |=  (1 << PA_CS);
    //   (See the 'sspi_cmd' method for 'DC' pin info.)
    GPIOB->ODR |=  (1 << PB_DC);
    // Set SCK high to start
    GPIOB->ODR |=  (1 << PB_SCK);
    // Reset the display by pulling the reset pin low,
    // delaying a bit, then pulling it high.
    GPIOA->ODR &= ~(1 << PA_RST);
    // Delay at least 100ms; meh, call it 2 million no-ops.
    delay_cycles(2000000);
    GPIOA->ODR |=  (1 << PA_RST);
    delay_cycles(2000000);
}

void SPI_Init(){
    // Make sure that the peripheral is off, and reset it.
    SPI1->CR1 &= ~(SPI_CR1_SPE);
    RCC->APB2RSTR |=  (RCC_APB2RSTR_SPI1RST);
    RCC->APB2RSTR &= ~(RCC_APB2RSTR_SPI1RST);
    // Set clock polarity and phase.
    SPI1->CR1 |=  (SPI_CR1_CPOL |
                   SPI_CR1_CPHA);

    // Set the STM32 to act as a host device.
    SPI1->CR1 |=  (SPI_CR1_MSTR);
    // Set software 'Chip Select' pin.
    SPI1->CR1 |=  (SPI_CR1_SSM);
    // (Set the internal 'Chip Select' signal.)
    SPI1->CR1 |=  (SPI_CR1_SSI);

    // Enable the peripheral.
    SPI1->CR1 |=  (SPI_CR1_SPE);

    SPI1->CR1 |=  (0x7 << SPI_CR1_BR_Pos);
}

void hspi_w8(SPI_TypeDef *SPIx, uint8_t dat) {
  // Wait for TXE.
  while (!(SPIx->SR & SPI_SR_TXE)) {};
  // Send the byte.
  *(uint8_t*)&(SPIx->DR) = dat;
}

void hspi_w16(SPI_TypeDef *SPIx, uint16_t dat) {
  // Wait for TXE.
  while (!(SPIx->SR & SPI_SR_TXE)) {};
  // Send the data.
  // (Flip the bytes for the little-endian ARM core.)
  dat = (((dat & 0x00FF) << 8) | ((dat & 0xFF00) >> 8));
  *(uint16_t*)&(SPIx->DR) = dat;

}

/*
 * Send a 'command' byte over hardware SPI.
 * Pull the 'D/C' pin low, send the byte, then pull the pin high.
 * Wait for the transmission to finish before changing the
 * 'D/C' pin value.
 */
void hspi_cmd(SPI_TypeDef *SPIx, uint8_t cmd) {
  while ((SPIx->SR & SPI_SR_BSY)) {};
  GPIOB->ODR &= ~(1 << PB_DC);
  hspi_w8(SPIx, cmd);
  while ((SPIx->SR & SPI_SR_BSY)) {};
  GPIOB->ODR |=  (1 << PB_DC);
}

void ili9341_hspi_init(SPI_TypeDef *SPIx) {
  // (Display off)
  //hspi_cmd(SPIx, 0x28);
  // Issue a series of initialization commands from the
  // Adafruit library for a simple 'known good' test.
  // (TODO: Add named macro definitions for these hex values.)
  hspi_cmd(SPIx, 0xEF);
  hspi_w8(SPIx, 0x03);
  hspi_w8(SPIx, 0x80);
  hspi_w8(SPIx, 0x02);
  hspi_cmd(SPIx, 0xCF);
  hspi_w8(SPIx, 0x00);
  hspi_w8(SPIx, 0xC1);
  hspi_w8(SPIx, 0x30);
  hspi_cmd(SPIx, 0xED);
  hspi_w8(SPIx, 0x64);
  hspi_w8(SPIx, 0x03);
  hspi_w8(SPIx, 0x12);
  hspi_w8(SPIx, 0x81);
  hspi_cmd(SPIx, 0xE8);
  hspi_w8(SPIx, 0x85);
  hspi_w8(SPIx, 0x00);
  hspi_w8(SPIx, 0x78);
  hspi_cmd(SPIx, 0xCB);
  hspi_w8(SPIx, 0x39);
  hspi_w8(SPIx, 0x2C);
  hspi_w8(SPIx, 0x00);
  hspi_w8(SPIx, 0x34);
  hspi_w8(SPIx, 0x02);
  hspi_cmd(SPIx, 0xF7);
  hspi_w8(SPIx, 0x20);
  hspi_cmd(SPIx, 0xEA);
  hspi_w8(SPIx, 0x00);
  hspi_w8(SPIx, 0x00);
  // PWCTR1
  hspi_cmd(SPIx, 0xC0);
  hspi_w8(SPIx, 0x23);
  // PWCTR2
  hspi_cmd(SPIx, 0xC1);
  hspi_w8(SPIx, 0x10);
  // VMCTR1
  hspi_cmd(SPIx, 0xC5);
  hspi_w8(SPIx, 0x3E);
  hspi_w8(SPIx, 0x28);
  // VMCTR2
  hspi_cmd(SPIx, 0xC7);
  hspi_w8(SPIx, 0x86);
  // MADCTL
  hspi_cmd(SPIx, 0x36);
  hspi_w8(SPIx, 0x48);
  // VSCRSADD
  hspi_cmd(SPIx, 0x37);
  hspi_w8(SPIx, 0x00);
  // PIXFMT
  hspi_cmd(SPIx, 0x3A);
  hspi_w8(SPIx, 0x55);
  // FRMCTR1
  hspi_cmd(SPIx, 0xB1);
  hspi_w8(SPIx, 0x00);
  hspi_w8(SPIx, 0x18);
  // DFUNCTR
  hspi_cmd(SPIx, 0xB6);
  hspi_w8(SPIx, 0x08);
  hspi_w8(SPIx, 0x82);
  hspi_w8(SPIx, 0x27);
  hspi_cmd(SPIx, 0xF2);
  hspi_w8(SPIx, 0x00);
  // GAMMASET
  hspi_cmd(SPIx, 0x26);
  hspi_w8(SPIx, 0x01);
  // (Actual gamma settings)
  hspi_cmd(SPIx, 0xE0);
  hspi_w8(SPIx, 0x0F);
  hspi_w8(SPIx, 0x31);
  hspi_w8(SPIx, 0x2B);
  hspi_w8(SPIx, 0x0C);
  hspi_w8(SPIx, 0x0E);
  hspi_w8(SPIx, 0x08);
  hspi_w8(SPIx, 0x4E);
  hspi_w8(SPIx, 0xF1);
  hspi_w8(SPIx, 0x37);
  hspi_w8(SPIx, 0x07);
  hspi_w8(SPIx, 0x10);
  hspi_w8(SPIx, 0x03);
  hspi_w8(SPIx, 0x0E);
  hspi_w8(SPIx, 0x09);
  hspi_w8(SPIx, 0x00);
  hspi_cmd(SPIx, 0xE1);
  hspi_w8(SPIx, 0x00);
  hspi_w8(SPIx, 0x0E);
  hspi_w8(SPIx, 0x14);
  hspi_w8(SPIx, 0x03);
  hspi_w8(SPIx, 0x11);
  hspi_w8(SPIx, 0x07);
  hspi_w8(SPIx, 0x31);
  hspi_w8(SPIx, 0xC1);
  hspi_w8(SPIx, 0x48);
  hspi_w8(SPIx, 0x08);
  hspi_w8(SPIx, 0x0F);
  hspi_w8(SPIx, 0x0C);
  hspi_w8(SPIx, 0x31);
  hspi_w8(SPIx, 0x36);
  hspi_w8(SPIx, 0x0F);
  // Exit sleep mode.
  hspi_cmd(SPIx, 0x11);
  delay_cycles(2000000);
  // Display on.
  hspi_cmd(SPIx, 0x29);
  delay_cycles(2000000);
  // 'Normal' display mode.
  hspi_cmd(SPIx, 0x13);
}
