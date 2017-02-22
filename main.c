#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "arm_math.h"
#include "stm32f4xx_adc.h"


#include <stdio.h>
#include <math.h>

#include "accelerometers/accelerometers.h"

static void init_systick();
static void delay_ms(uint32_t n);
static volatile uint32_t msTicks,msTicks1; // counts 1 ms timeTicks
void init_gas_sensor(void);
float read_gas_sensor(void);
void init_temperature23_sensor(void);
float read_temperature23_sensor(void);
int v;
int n;
int Flag;
float msTicks_var, msTicks_var1, msTicks_var2, msTicks_var3, msTicks_var4, msTicks_var5, msTicks_var6;


// SysTick Handler (Interrupt Service Routine for the System Tick interrupt)
void SysTick_Handler(void){
    msTicks++;
}

// initialize the system tick
void init_systick(void){
    SystemCoreClockUpdate();                      /* Get Core Clock Frequency   */
    if (SysTick_Config(SystemCoreClock / 1000)) { /* SysTick 1 msec interrupts  */
        while (1);                                  /* Capture error              */
    }
}

// pause for a specified number (n) of milliseconds
void delay_ms(uint32_t n) {
    uint32_t msTicks2 = msTicks + n;
    while(msTicks < msTicks2) ;
}


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


uint32_t read_button(void)
{
    unsigned int q;
    
    if(GPIOA->IDR & (0x01))
    {
        q=1;
    }
    else
    {
        q=0;
    }
    return q;
}




// initialize the gas sensor
void init_gas_sensor(){
    ADC_InitTypeDef ADC_InitStruct;
    ADC_CommonInitTypeDef ADC_CommonInitStruct;
    
    // RCC Initialization
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    
    // Common ADC Initialization
    ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;//ADC works in Independent mode
    ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div8;//Set the prescaler
    ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInit(&ADC_CommonInitStruct);
    
    ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStruct.ADC_ScanConvMode = DISABLE;
    ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;//ADC Works in continuous mode
    ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//No trigger
    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStruct.ADC_NbrOfConversion = 3; //Number of convergence
    ADC_Init(ADC1, &ADC_InitStruct);
    
    
    // Enable ADC
    ADC_Cmd(ADC1, ENABLE);
    
    // ADC Channel 1 Configuration
    ADC_RegularChannelConfig(ADC1, ADC_Channel_gas, 1, ADC_SampleTime_144Cycles);//Channel number and configuration
    
    
    
    
}

//The above code shows the initialization of gas sensor.

// get one gas reading (in ppm)
float read_gas_sensor(){
    float gas; // gas reading
    
    ADC_SoftwareStartConv(ADC1); //Start the conversion
    
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET); //Wait for ADC conversion to finish
    gas = (float)ADC_GetConversionValue(ADC1); //Get one ADC reading -- as a number from 0 to 4095 (12 bits)
    
    // Convert ADC reading to voltage
    gas /= 4095.0f;
    
    
    
    return gas;
}

void init_temperature23_sensor(){
    ADC_InitTypeDef ADC_InitStruct;
    ADC_CommonInitTypeDef ADC_CommonInitStruct;
    
    // RCC Initialization
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    
    // Common ADC Initialization
    ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;//ADC works in Independent mode
    ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div8;//Set the prescaler
    ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInit(&ADC_CommonInitStruct);
    
    ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStruct.ADC_ScanConvMode = DISABLE;
    ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;//ADC Works in continuous mode
    ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//No trigger
    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStruct.ADC_NbrOfConversion = 3;//Number of convergence
    ADC_Init(ADC1, &ADC_InitStruct);
    
    
    
    // Enable ADC
    ADC_Cmd(ADC1, ENABLE);
    
    // ADC Channel 1 Configuration
    ADC_RegularChannelConfig(ADC1, ADC_Channel_tempSensor23, 2, ADC_SampleTime_144Cycles);//Channel number and configuration
    
    
    
    
}

//The above code shows the initialization of temperature sensor.
// get one temperature reading (in degree)
float read_temperature23_sensor(){
    float temperature; // gas reading
    
    ADC_SoftwareStartConv(ADC1); //Start the conversion
    
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET); //Wait for ADC conversion to finish
    temperature = (float)ADC_GetConversionValue(ADC1); //Get one ADC reading -- as a number from 0 to 4095 (12 bits)
    
    // Convert ADC reading to voltage
    temperature /= 4095.0f;
   	
    return temperature;
    
}

void init_proximity_sensor(){
    ADC_InitTypeDef ADC_InitStruct;
    ADC_CommonInitTypeDef ADC_CommonInitStruct;
    
    // RCC Initialization
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    
    // Common ADC Initialization
    ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;//ADC works in Independent mode
    ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div8;//Set the prescaler
    ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInit(&ADC_CommonInitStruct);
    
    ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStruct.ADC_ScanConvMode = DISABLE;
    ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;//ADC Works in continuous mode
    ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//No trigger
    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStruct.ADC_NbrOfConversion = 3;//Number of convergence
    ADC_Init(ADC1, &ADC_InitStruct);
    
    
    
    // Enable ADC
    ADC_Cmd(ADC1, ENABLE);
    
    // ADC Channel 1 Configuration
    ADC_RegularChannelConfig(ADC1, ADC_Channel_proximity, 3, ADC_SampleTime_144Cycles);//Channel number and configuration
    
    
    
    
}

//The above code shows the initialization of proximity sensor.
// get one distance reading
float read_proximity_sensor(){
    float distance1; // gas reading
    
    ADC_SoftwareStartConv(ADC1); //Start the conversion
    
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET); //Wait for ADC conversion to finish
    distance1 = (float)ADC_GetConversionValue(ADC1); //Get one ADC reading -- as a number from 0 to 4095 (12 bits)
    
    // Convert ADC reading to voltage
    distance1 /= 4095.0f;
   	
    
    return distance1;
    
}




void initialise_monitor_handles();

int main(void)
{
    // initialize
    SystemInit();
    initialise_monitor_handles();
    init_systick();
    init_gas_sensor();
    init_LED_pins();
    msTicks_var = msTicks;
    msTicks_var1 = msTicks;
    msTicks_var2 = msTicks;
    msTicks_var3 = msTicks;
    msTicks_var4 = msTicks;
    
    
    //Similarly we have two more blocks for temperature sensor and proximity sensor with their respective parameters.
    while (1)
    {
        msTicks_var = msTicks;
        SystemInit();
        float gas1 = read_gas_sensor();// Reads the value from gas sensor ADC channel
        
        SystemInit();
        
        if (gas1 > 0.0350)//checks with reference value for gas
        {
            
            
            if(msTicks_var - msTicks_var1>=8000)//checks for initial delay
            {
                msTicks_var1 = msTicks;
                msTicks_var2 = 0;
                GPIOD->BSRRL |= (0x1 << (1*12));//turns on the alarm
                
                
            }
            
            
        }
        
        msTicks_var = msTicks;
        SystemInit();
        
        
        float temperature1 = read_temperature23_sensor();//Reads the value from temperature sensor ADC channel

        
        SystemInit();
        if (temperature1 > 0.2560)//checks with reference value for temperature
        {
            
            
            
            if(msTicks_var - msTicks_var3>=2000)//checks for initial delay
            {
                msTicks_var3 = msTicks;
                msTicks_var4 = 0;
                
                
                GPIOD->BSRRL |= (0x1 << (1*13));//turns on the alarm

                GPIOD->BSRRL |= (0x1 << (1*14));//turns on the alarm
            }
            
            
        }
        
        msTicks_var5 = msTicks;
        SystemInit();
        
        
        float distance = read_proximity_sensor();//Reads the value from Proximity sensor ADC channel

        
        SystemInit();
        if (distance > 0.3000)//checks with reference value for temperature
        {
            
            
            
            if(msTicks_var - msTicks_var5>=2000)//checks for initial delay
            {
                msTicks_var5 = msTicks;
                msTicks_var6 = 0;
                
                
                GPIOD->BSRRL |= (0x1 << (1*15));//turns on the alarm
                
                
                
            }
            
        }
        
    }
}



