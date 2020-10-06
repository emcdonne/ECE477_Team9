#include "stm32f0xx_hal.h"
#include "icons_config.h"

const unsigned int image_icon_40x40[] = {
		W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,
		W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,
		W,W,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,W,W,
		W,W,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,W,W,
		W,W,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,W,W,
		W,W,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,W,W,
		W,W,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,W,W,
		W,W,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,W,W,
		W,W,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,W,W,
		W,W,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,W,W,
		W,W,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,W,W,
		W,W,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,W,W,
		W,W,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,W,W,
		W,W,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,W,W,
		W,W,B,B,B,B,B,B,B,B,B,B,B,W,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,W,W,
		W,W,B,B,B,B,B,B,B,B,B,B,W,B,W,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,W,W,
		W,W,B,B,B,B,B,B,B,B,B,W,B,B,B,W,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,W,W,
		W,W,B,B,B,B,B,B,B,B,W,B,B,B,B,B,W,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,W,B,B,B,W,W,
		W,W,B,B,B,B,B,B,B,W,B,B,B,B,B,B,B,W,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,W,B,W,B,B,W,W,
		W,W,B,B,B,B,B,B,W,B,B,B,B,B,B,B,B,B,W,B,B,B,B,B,B,B,B,B,B,B,B,B,W,B,B,B,W,B,W,W,
		W,W,B,B,B,B,B,W,B,B,B,B,B,B,B,B,B,B,B,W,B,B,B,B,B,B,B,B,B,B,B,W,B,B,B,B,B,W,W,W,
		W,W,B,B,B,B,W,B,B,B,B,B,B,B,B,B,B,B,B,B,W,B,B,B,B,B,B,B,B,B,W,B,B,B,B,B,B,B,W,W,
		W,W,B,B,B,W,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,W,B,B,B,B,B,B,B,W,B,B,B,B,B,B,B,B,W,W,
		W,W,B,B,W,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,W,B,B,B,B,B,W,B,B,B,B,B,B,B,B,B,W,W,
		W,W,B,W,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,W,B,B,B,W,B,B,B,B,B,B,B,B,B,B,W,W,
		W,W,W,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,W,B,W,B,B,B,B,B,B,B,B,B,B,B,W,W,
		W,W,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,W,B,B,B,B,B,B,B,B,B,B,B,B,W,W,
		W,W,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,W,B,B,B,B,B,B,B,B,B,B,B,W,W,
		W,W,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,W,B,B,B,B,B,B,B,B,B,B,W,W,
		W,W,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,W,B,B,B,B,B,B,B,B,B,W,W,
		W,W,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,W,B,B,B,B,B,B,B,B,W,W,
		W,W,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,W,B,B,B,B,B,B,B,W,W,
		W,W,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,W,B,B,B,B,B,B,W,W,
		W,W,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,W,B,B,B,B,B,W,W,
		W,W,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,W,B,B,B,B,W,W,
		W,W,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,W,B,B,B,W,W,
		W,W,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,W,B,B,W,W,
		W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,B,W,W,
		W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,
		W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,
};