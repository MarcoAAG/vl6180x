#include <stm32l4xx_hal.h>
#include <stm32l4xx_hal_gpio.h>
#include <stm32l4xx_hal_rcc.h>
#include <stm32l4xx_hal_i2c.h>
#include <vl6180x_example.h>
#include <vl6180x_lib.h>

static void SystemClock_Config(void);

I2C_HandleTypeDef I2cHandle;

/* I2C TIMING Register define when I2C clock source is SYSCLK */
/* I2C TIMING is calculated in case of the I2C Clock source is the SYSCLK = 80 MHz */
/* This example use TIMING to 0x00D00E28 to reach 1 MHz speed (Rise time = 120ns, Fall time = 25ns) */
#define I2C_TIMING      0x00D00E28

/* Definition for I2Cx Pins */
#define I2Cx_SCL_PIN                    GPIO_PIN_9
#define I2Cx_SCL_GPIO_PORT              GPIOA
#define I2Cx_SDA_PIN                    GPIO_PIN_10
#define I2Cx_SDA_GPIO_PORT              GPIOA
#define I2Cx_SCL_SDA_AF                 GPIO_AF4_I2C1

uint8_t aTxBuffer[2] = {0x0,0x1};
uint8_t aRxBuffer[1];


int main()
{
    HAL_Init();

    /* Configure the system clock to 80 MHz */
    SystemClock_Config();

     __HAL_RCC_GPIOA_CLK_ENABLE();
     __HAL_RCC_GPIOB_CLK_ENABLE();
     __HAL_RCC_I2C1_CLK_ENABLE();

    GPIO_InitTypeDef  GPIO_InitStruct;
    GPIO_InitTypeDef myGpio;
    GPIO_InitTypeDef t_mco;

    // I2C TX GPIO pin configuration
    GPIO_InitStruct.Pin       = I2Cx_SCL_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = I2Cx_SCL_SDA_AF;
    HAL_GPIO_Init(I2Cx_SCL_GPIO_PORT, &GPIO_InitStruct);

    /* I2C RX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = I2Cx_SDA_PIN;
    GPIO_InitStruct.Alternate = I2Cx_SCL_SDA_AF;
    HAL_GPIO_Init(I2Cx_SDA_GPIO_PORT, &GPIO_InitStruct);
    
    myGpio.Pin = GPIO_PIN_3;
    myGpio.Mode = GPIO_MODE_OUTPUT_PP;
    myGpio.Pull = GPIO_NOPULL;
    myGpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOB,&myGpio);

    t_mco.Pin = GPIO_PIN_8;
    t_mco.Mode = GPIO_MODE_AF_PP;
    t_mco.Pull = GPIO_NOPULL;
    t_mco.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    t_mco.Alternate = GPIO_AF0_MCO;
    HAL_GPIO_Init(GPIOA,&t_mco);

    // Configure the I2C clock source. The clock is derived from the SYSCLK
    RCC_PeriphCLKInitTypeDef  RCC_PeriphCLKInitStruct;
    RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
    RCC_PeriphCLKInitStruct.I2c1ClockSelection = RCC_I2C1CLKSOURCE_SYSCLK;
    HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);

    // Configure the I2C peripheral 
    I2cHandle.Instance             = I2C1;
    I2cHandle.Init.Timing          = I2C_TIMING;
    I2cHandle.Init.OwnAddress1     = VL6180_ADDR;
    I2cHandle.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
    I2cHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    I2cHandle.Init.OwnAddress2     = 0xFF;
    I2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    I2cHandle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;  

    volatile HAL_StatusTypeDef t_stat = HAL_I2C_Init(&I2cHandle);

    if(t_stat != HAL_OK)
    {
    /* Initialization Error */
    }


    HAL_RCC_MCOConfig(RCC_MCO1,RCC_MCO1SOURCE_SYSCLK,RCC_CFGR_MCOPRE_DIV16);

    // Set Ranging Mode Single-Shot
    vl6180_v_WriteData(&I2cHandle, RANGE_START_ADDR, &aTxBuffer[0], 1u);
    HAL_Delay(10);
    //Start reading
    vl6180_v_WriteData(&I2cHandle, RANGE_START_ADDR, &aTxBuffer[1], 1u);
    // HAL_Delay(10);
    //Get Data
    vl6180_v_ReadData(&I2cHandle, RANGE_VAL_ADDR, (uint8_t*)aRxBuffer, 1);
    HAL_Delay(10);
    

    while(1)
    {
        HAL_GPIO_TogglePin(GPIOB,myGpio.Pin);
        vl6180_v_ReadData(&I2cHandle, RANGE_VAL_ADDR, (uint8_t*)aRxBuffer, 1);
        //Insert delay 100 ms
        HAL_Delay(1000);
    }
}



/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows :
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 80000000
  *            HCLK(Hz)                       = 80000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 4000000
  *            PLL_M                          = 1
  *            PLL_N                          = 40
  *            PLL_R                          = 2
  *            PLL_P                          = 7
  *            PLL_Q                          = 4
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* MSI is enabled after System reset, activate PLL with MSI as source */
//   RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
//   RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
//   RCC_OscInitStruct.MSIClockRange = RCC_CR_MSIRANGE_6;
//   RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
//   RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLR = 8;
  RCC_OscInitStruct.PLL.PLLP = 7;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
}