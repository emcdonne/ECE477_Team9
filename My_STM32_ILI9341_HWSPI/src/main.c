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
#include "stm32f0xx_hal.h"
//#include "ILI9341_Driver.h"

void GPIO_Init(void);
void SPI_Init(void);
void hspi_w8(SPI_TypeDef *SPIx, uint8_t dat);
void hspi_w16(SPI_TypeDef *SPIx, uint16_t dat);
void hspi_cmd(SPI_TypeDef *SPIx, uint8_t cmd);
void ili9341_hspi_init(SPI_TypeDef *SPIx);

SPI_HandleTypeDef hspi1;

int main(void)
{

	RCC->AHBENR   |= RCC_AHBENR_GPIOAEN;
	RCC->AHBENR   |= RCC_AHBENR_GPIOBEN;
	RCC->AHBENR   |= RCC_AHBENR_GPIOCEN;

	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;


	GPIOC-> MODER |= (1<<16);
	GPIOC-> ODR |= (1<<8);

	GPIO_Init();
	SPI_Init();
	//MX_SPI1_Init();
	//my_ILI9341_Init(SPI1);
	ili9341_hspi_init(SPI1);
	//ili9341_init();


	//hspi_cmd(SPI1, 0x28);

	int i;


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

    while (1) {
      //ili9341_hspi_init(SPI1);
      //ili9341_init();

      // Write 320 * 240 pixels.
      for (tft_iter = 0; tft_iter < (320*240); ++tft_iter) {
        // Write a 16-bit color.
        if (tft_on) {
          hspi_w16(SPI1, 0xF080);	// 0xF800
        }
        else {
          hspi_w16(SPI1, 0x001F);
        }
      }
      tft_on = !tft_on;

      // blink
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
#define PB_MOSI (5)
#define PB_SCK  (3)
#define PB_DC   (4)
#define PA_CS   (12)
#define PA_RST  (15)
void GPIO_Init(void) {

    // Define GPIOB pin mappings for software '4-wire' SPI.


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

	return;
}

void MX_SPI1_Init(void)
{

  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_HARD_OUTPUT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  HAL_SPI_Init(&hspi1);

}

void SPI_Init(void){
    // Make sure that the peripheral is off, and reset it.
    SPI1->CR1 &= ~(SPI_CR1_SPE);
    RCC->APB2RSTR |=  (RCC_APB2RSTR_SPI1RST);
    RCC->APB2RSTR &= ~(RCC_APB2RSTR_SPI1RST);
    // Set clock polarity and phase.
    SPI1->CR1 |=  (SPI_CR1_CPOL |
                   SPI_CR1_CPHA);

    // Datasize set
    // SPI1->CR2 |= (SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2 | SPI_CR2_DS_3); // 16 bits
    SPI1->CR2 &= ~(SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2 | SPI_CR2_DS_3); // 8 bits

    // Set the STM32 to act as a host device.
    SPI1->CR1 |=  (SPI_CR1_MSTR);
    // Set software 'Chip Select' pin.
    SPI1->CR1 |=  (SPI_CR1_SSM);
    // (Set the internal 'Chip Select' signal.)
    SPI1->CR1 |=  (SPI_CR1_SSI);

    // FOR OSCILLOSCOPE DEBUGGING: slow this down
    //SPI1->CR1 |=  (0x3 << SPI_CR1_BR_Pos);

    // Enable the peripheral.
    SPI1->CR1 |=  (SPI_CR1_SPE);

	return;
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

void my_ILI9341_Init(SPI_TypeDef *SPIx) {
	//ILI9341_SPI_Init();

	/* Reset The Screen */
	//ILI9341_Reset();
	HAL_GPIO_WritePin(GPIOA,PA_RST,GPIO_PIN_RESET);
	HAL_Delay(50);
	HAL_GPIO_WritePin(GPIOA,PA_RST,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA,PA_CS,GPIO_PIN_RESET);
	HAL_Delay(50);
	hspi_cmd(SPIx, 0x01);
	HAL_GPIO_WritePin(GPIOA,PA_CS,GPIO_PIN_SET);
	hspi_cmd(SPIx, 0x01);

	/* Power Control A */
	hspi_cmd(SPIx, 0xCB);
	hspi_w8(SPIx, 0x39);
	hspi_w8(SPIx, 0x2C);
	hspi_w8(SPIx, 0x00);
	hspi_w8(SPIx, 0x34);
	hspi_w8(SPIx, 0x02);

	/* Power Control B */
	hspi_cmd(SPIx, 0xCF);
	hspi_w8(SPIx, 0x00);
	hspi_w8(SPIx, 0xC1);
	hspi_w8(SPIx, 0x30);

	/* Driver timing control A */
	hspi_cmd(SPIx, 0xE8);
	hspi_w8(SPIx, 0x85);
	hspi_w8(SPIx, 0x00);
	hspi_w8(SPIx, 0x78);

	/* Driver timing control B */
	hspi_cmd(SPIx, 0xEA);
	hspi_w8(SPIx, 0x00);
	hspi_w8(SPIx, 0x00);

	/* Power on Sequence control */
	hspi_cmd(SPIx, 0xED);
	hspi_w8(SPIx, 0x64);
	hspi_w8(SPIx, 0x03);
	hspi_w8(SPIx, 0x12);
	hspi_w8(SPIx, 0x81);

	/* Pump ratio control */
	hspi_cmd(SPIx, 0xF7);
	hspi_w8(SPIx, 0x20);

	/* Power Control 1 */
	hspi_cmd(SPIx, 0xC0);
	hspi_w8(SPIx, 0x10);

	/* Power Control 2 */
	hspi_cmd(SPIx, 0xC1);
	hspi_w8(SPIx, 0x10);

	/* VCOM Control 1 */
	hspi_cmd(SPIx, 0xC5);
	hspi_w8(SPIx, 0x3E);
	hspi_w8(SPIx, 0x28);

	/* VCOM Control 2 */
	hspi_cmd(SPIx, 0xC7);
	hspi_w8(SPIx, 0x86);

	/* VCOM Control 2 */
	hspi_cmd(SPIx, 0x36);
	hspi_w8(SPIx, 0x48);

	/* Pixel Format Set */
	hspi_cmd(SPIx, 0x3A);
	hspi_w8(SPIx, 0x55);    //16bit

	hspi_cmd(SPIx, 0xB1);
	hspi_w8(SPIx, 0x00);
	hspi_w8(SPIx, 0x18);

	/* Display Function Control */
	hspi_cmd(SPIx, 0xB6);
	hspi_w8(SPIx, 0x08);
	hspi_w8(SPIx, 0x82);
	hspi_w8(SPIx, 0x27);

	/* 3GAMMA FUNCTION DISABLE */
	hspi_cmd(SPIx, 0xF2);
	hspi_w8(SPIx, 0x00);

	/* GAMMA CURVE SELECTED */
	hspi_cmd(SPIx, 0x26); //Gamma set
	hspi_w8(SPIx, 0x01); 	//Gamma Curve (G2.2)

	//Positive Gamma  Correction
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

	//Negative Gamma  Correction
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

	//EXIT SLEEP
	hspi_cmd(SPIx, 0x11);

	//TURN ON DISPLAY
	hspi_cmd(SPIx, 0x29);
	hspi_w8(SPIx, 0x2C);
}
void ili9341_writecmd(uint8_t cmd)
{
HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);	//DC to 0
HAL_SPI_Transmit(&hspi1,&cmd,1,1000);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);	//DC to 1
}

void ili9341_writedata(uint8_t data)
{
HAL_SPI_Transmit(&hspi1,&data,1,1000);
}

void ili9341_init(void)
{
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);	//RESET to 0
HAL_Delay(100);
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);	//RESET to 1

ili9341_writecmd(0xCB);
ili9341_writedata(0x39);
ili9341_writedata(0x2C);
ili9341_writedata(0x00);
ili9341_writedata(0x34);
ili9341_writedata(0x02);

ili9341_writecmd(0xCF);
ili9341_writedata(0x00);
ili9341_writedata(0XC1);
ili9341_writedata(0X30);

 HAL_Delay(10);

ili9341_writecmd(0xE8);
ili9341_writedata(0x85);
ili9341_writedata(0x00);
ili9341_writedata(0x78);

ili9341_writecmd(0xEA);
ili9341_writedata(0x00);
ili9341_writedata(0x00);

ili9341_writecmd(0xED);
ili9341_writedata(0x64);
ili9341_writedata(0x03);
ili9341_writedata(0X12);
ili9341_writedata(0X81);

ili9341_writecmd(0xF7);
ili9341_writedata(0x20);

ili9341_writecmd(0xC0);    //Power control
ili9341_writedata(0x23);   //VRH[5:0]

ili9341_writecmd(0xC1);    //Power control
ili9341_writedata(0x10);   //SAP[2:0];BT[3:0]

ili9341_writecmd(0xC5);    //VCM control
ili9341_writedata(0x3e);   //Contrast
ili9341_writedata(0x28);

ili9341_writecmd(0xC7);    //VCM control2
ili9341_writedata(0x86);   //--

ili9341_writecmd(0x36);    // Memory Access Control
ili9341_writedata(0x28);   //RGB565 Horizontal orientation
//ili9341_writedata(0x08);   //RGB565 Vertical orientation

ili9341_writecmd(0x3A);
ili9341_writedata(0x55);

ili9341_writecmd(0xB1);
ili9341_writedata(0x00);
ili9341_writedata(0x18);

ili9341_writecmd(0xB6);    // Display Function Control
ili9341_writedata(0x08);
ili9341_writedata(0x82);
ili9341_writedata(0x27);

ili9341_writecmd(0x11);    //Exit Sleep
HAL_Delay(100);
ili9341_writecmd(0x29);    //Display on
ili9341_writecmd(0x2c);

}

void ili9341_setaddr(uint8_t x1,uint8_t y1,uint16_t x2,uint16_t y2)
{
ili9341_writecmd(0x2A);
ili9341_writedata(x1>>8);
ili9341_writedata(x1);
ili9341_writedata(x2>>8);
ili9341_writedata(x2);

ili9341_writecmd(0x2B);
ili9341_writedata(y1>>8);
ili9341_writedata(y1);
ili9341_writedata(y2>>8);
ili9341_writedata(y2);

ili9341_writecmd(0x2C);
}

