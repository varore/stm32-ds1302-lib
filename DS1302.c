#include "DS1302.h"


// GPIO Pins
static GPIO_TypeDef *DS1302_CLK_PORT;
static GPIO_TypeDef *DS1302_DAT_PORT;
static GPIO_TypeDef *DS1302_RST_PORT;
static uint16_t DS1302_CLK_PIN;
static uint16_t DS1302_DAT_PIN;
static uint16_t DS1302_RST_PIN;

uint8_t bcd_to_dec(uint8_t bcd) {
  return (10 * ((bcd & 0xF0) >> 4) + (bcd & 0x0F));
}

uint8_t decToBcd(uint8_t dec) {
  const uint8_t tens = dec / 10;
  const uint8_t ones = dec % 10;
  return (tens << 4) | ones;
}

uint8_t hourFromRegisterValue(const uint8_t value) {
  uint8_t adj;
  if (value & 128)  // 12-hour mode
    adj = 12 * ((value & 32) >> 5);
  else           // 24-hour mode
    adj = 10 * ((value & (32 + 16)) >> 4);
  return (value & 15) + adj;
}
void dat_pin_write_mode(){
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	DS1302_DAT_PORT->CRH |= 0x3;
	GPIOB->CRH &= ~(0xc);
}
void dat_pin_read_mode(){
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	GPIOB->CRH &= ~(0xf);
	GPIOB->CRH |= 0x8;
}

static void pin_write_mode(GPIO_TypeDef *port, uint16_t pin)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
  GPIO_InitStruct.Pin = pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}
//static void pin_read_mode(uint16_t pin)
//{
//  GPIO_InitTypeDef GPIO_InitStruct;
//  HAL_GPIO_WritePin(GPIOB, pin, GPIO_PIN_RESET);
//  GPIO_InitStruct.Pin = pin;
//  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//
//}



void pulse_clk(void){
	HAL_GPIO_WritePin(DS1302_CLK_PORT, DS1302_CLK_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(DS1302_CLK_PORT, DS1302_CLK_PIN, GPIO_PIN_RESET);
}

void rtc_write(uint8_t value){
//	pin_write_mode(DS_1302_DAT_PIN);
	dat_pin_write_mode();

	for (int i=0; i<8; i++){
		HAL_GPIO_WritePin(DS1302_DAT_PORT, DS1302_DAT_PIN, (value>>i) & 1 ? GPIO_PIN_SET : GPIO_PIN_RESET);
		pulse_clk();
	}
}

uint8_t rtc_read(){
	uint8_t input_value = 0;
	uint8_t bit = 0;
//	pin_read_mode(DS_1302_DAT_PIN);
	dat_pin_read_mode();
	for (int i = 0; i < 8; ++i) {
		bit = HAL_GPIO_ReadPin(DS1302_DAT_PORT, DS1302_DAT_PIN);
		input_value |= (bit << i);
		pulse_clk();
	}
	return input_value;
}

uint8_t rtc_read_reg(uint8_t reg){
	volatile uint8_t cmd_byte = (0x81 | (reg << 1));
	uint8_t result = 0;
	HAL_GPIO_WritePin(DS1302_RST_PORT, DS1302_RST_PIN, GPIO_PIN_SET);
	rtc_write(cmd_byte);
	result = rtc_read();
	HAL_GPIO_WritePin(DS1302_RST_PORT, DS1302_RST_PIN, GPIO_PIN_RESET);

	return result;
}

void rtc_write_reg(uint8_t reg, uint8_t num){
	uint8_t cmd_byte = (0x80 | (reg << 1));
	uint8_t reg_data = decToBcd(num);
	HAL_GPIO_WritePin(DS1302_RST_PORT, DS1302_RST_PIN, GPIO_PIN_SET);
	rtc_write(cmd_byte);
	rtc_write(reg_data);
	HAL_GPIO_WritePin(DS1302_RST_PORT, DS1302_RST_PIN, GPIO_PIN_RESET);

}
void rtc_set_time(uint8_t year, uint8_t month, uint8_t date, uint8_t hour, uint8_t min, uint8_t sec){
	rtc_write_reg(kYearReg, year);
	rtc_write_reg(kMonthReg, month);
	rtc_write_reg(kDateReg, date);
	rtc_write_reg(kHourReg, hour);
	rtc_write_reg(kMinuteReg, min);
	rtc_write_reg(kSecondReg, sec);

}


void rtc_init(void){
	HAL_Delay(1000);
	__HAL_RCC_GPIOB_CLK_ENABLE();
	pin_write_mode(DS1302_CLK_PORT, DS1302_CLK_PIN);
	pin_write_mode(DS1302_RST_PORT, DS1302_RST_PIN);
	HAL_GPIO_WritePin(DS1302_CLK_PORT, DS1302_CLK_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DS1302_RST_PORT, DS1302_RST_PIN, GPIO_PIN_RESET);
}
