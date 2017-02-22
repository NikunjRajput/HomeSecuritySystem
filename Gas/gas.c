#include "stm32f4xx_adc.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

void init_LED_pins()
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;  // enable clock to GPIOD
    
    GPIOD->MODER &= ~(0xFF << (2*12)); // clear the 2 bits corresponding to pins 12, 13, 14, 15
    GPIOD->MODER |= (0x55 << (2*12));    // set pin 12,13,14,15 to be general purpose output
    
}


void init_button()
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;  // enable clock to GPIOA
    
    GPIOA->MODER &= ~(0x3 << (2*0)); // clear the 2 bits corresponding to pin 0
    // if the 2 bits corresponding to pin 0 are 00, then it is in input mode
}





void LED_On(uint32_t i)
{
    
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;  // enable clock to GPIOA
    

            
            GPIOD->BSRRL |= (0x1 << (1*12));
        
            GPIOD->BSRRL |= (0x1 << (1*13));
       
            GPIOD->BSRRL |= (0x1 << (1*14));
        
            GPIOD->BSRRL |= (0x1 << (1*15));
            
        
    
}


void LED_Off(uint32_t i)
{
    
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;  // enable clock to GPIOA
    
    
    switch(i)
    {
            
        case 0:
        { GPIOD->BSRRH |= (0x1 << (1*12));
            break;
        }
        case 1:
        { GPIOD->BSRRH |= (0x1 << (1*13));
            break;
        }
        case 2:
        { GPIOD->BSRRH |= (0x1 << (1*14));
            break;
        }
        case 3:
        { GPIOD->BSRRH |= (0x1 << (1*15));
            break;
        }
    }
}



// initialize the temperature sensor
float init_gas_sensor(){
    ADC_InitTypeDef ADC_InitStruct;
    ADC_CommonInitTypeDef ADC_CommonInitStruct;
    
    // RCC Initialization
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    
    // Common ADC Initialization
    ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div8;
    ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInit(&ADC_CommonInitStruct);
    
    ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStruct.ADC_ScanConvMode = DISABLE;
    ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStruct.ADC_NbrOfConversion = 1;
    ADC_Init(ADC1, &ADC_InitStruct);
    
    // ADC Channel 1 Configuration
    ADC_RegularChannelConfig(ADC1, ADC_Channel_Gas, 1, ADC_SampleTime_144Cycles);
    
    // Enable Temperature Sensor
    //ADC_TempSensorVrefintCmd(ENABLE);
    
    // Enable ADC
    ADC_Cmd(ADC1, ENABLE);
}

// get one gas reading (in ppm)
float read_gas_sensor(){
    float gas; // gas reading
    
    ADC_SoftwareStartConv(ADC1); //Start the conversion
    
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET); //Wait for ADC conversion to finish
    temp = (float)ADC_GetConversionValue(ADC1); //Get one ADC reading -- as a number from 0 to 4095 (12 bits)
    
    // Convert ADC reading to voltage
    gas /= 4095.0f;
    gas *= 3.3f;
    LED_GAS_ON(gas);
    
    return gas;
}

void LED_GAS_ON(gas)
        {
    
    if (gas >= 000011001000)
    {
        i=1;
        LED_On(i);
        
    }
        
        
        }




