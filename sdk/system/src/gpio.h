/*
 * gpio.h
 *
 *  Created on: Mar 22, 2019
 *      Author: adam
 */

#ifndef SRC_GPIO_H_
#define SRC_GPIO_H_

#define PB_CH_0_1_ADDR_GPIO_ID 0
#define PB_ADDR_0_1_GPIO_CH 1
#define PB_STOP_ADDR_0_1 2

#define PB_CH_2_3_ADDR_GPIO_ID 1
#define PB_ADDR_2_3_GPIO_CH 1
#define PB_STOP_ADDR_2_3 2

#define PB_CH_0_1_ADDR_READBACK_HP_GPIO_ID   2
#define PB_CH_ADDR_READBACK_GPIO_CH 1
#define PB_CH_HP_GPIO_CH 2

#define PB_CH_MODE_EN_GPIO_ID  3
#define PB_CH_MODE_GPIO_CH 1
#define PB_CH_EN_GPIO_CH 2

#define PB_CH_DIN_WRITE_STOP_ADDR_GPIO_ID 4
#define PB_CH_DIN_GPIO_CH 1
#define PB_CH_WRITE_STOP_ADDR_CH 2

#define PB_CH_WRITE_ADDR_WRITE_DIN_GPIO_ID 5
#define PB_CH_WRITE_ADDR_GPIO_CH 1
#define PB_CH_WRITE_DIN_GPIO_CH 2

#define PB_CH_CLK_CTRL_GPIO_ID 6
#define PB_CH_0_1_ADDR_READBACK_CH 1
#define LOOP_PB_GPIO 2

#define PB_CH_2_3_ADDR_READBACK_WRITE_HP_GPIO_ID 7
#define PB_CH_2_3_ADDR_READBACK_CH 1
#define WRITE_HP_CH 2

#define GPIO_AS_OUTPUT 0
#define GPIO_AS_INPUT 1


#endif /* SRC_GPIO_H_ */
