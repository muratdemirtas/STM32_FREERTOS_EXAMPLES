/////////////VISUAL GDB STM32F103CT8 FREE RTOS LED BLINK EXAMPLE////////////

//INCLUDE STM32 HAL AND FREERTOS LIBRARY
#include <stm32f1xx_hal.h>
#include <../CMSIS_RTOS/cmsis_os.h>

//DEFINE LED BLINK TIMES
#define LED1_BLINK_TIME 100
#define LED2_BLINK_TIME	 50
#define IN_BUILT_BLINK_TIME 25

//SET THREAD ID'S
osThreadId LED1_BLINK, LED2_BLINK, IN_BUILT_BLINK;

//set thread functions,they are static so you can bind some arguments

static void LED1_Thread(void const *argument);
static void LED2_Thread(void const *argument);
static void	IN_BUILT_Thread(void const *argument);

//USE THIS FOR PRONT CODE ERROR WITH FILE AND LINE
#define  USE_FULL_ASSERT

//MAIN AND SETUP FUNCTION FOR STM32
int main(void)
{
	//INITIALIZE STM32	HAL LIBRARY FOR GPIOS
	HAL_Init();  
	
	//ENABLE WHICH GPIOS YOU WANT TO USE.
	__GPIOB_CLK_ENABLE();
	__GPIOC_CLK_ENABLE();

	//SET STRUCT FOR LED1 AND LED2 B PORT BANK
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin = GPIO_PIN_8 | GPIO_PIN_9;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	
	GPIO_InitTypeDef GPIOC_InitStructure;
	GPIOC_InitStructure.Pin = GPIO_PIN_13;
	GPIOC_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIOC_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIOC_InitStructure.Pull = GPIO_NOPULL;
	
	//PREPARE THIS CONFIGURATIONS
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
	HAL_GPIO_Init(GPIOC, &GPIOC_InitStructure);
	
	//LED1_THREAD  DEFINITION
	osThreadDef(LED1, LED1_Thread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
  
	//LED2_THREAD  DEFINITION
	osThreadDef(LED2, LED2_Thread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
  
	osThreadDef(IN_BUILT_LED, IN_BUILT_Thread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
	
	//CREATE THREAD FOR LED1
	LED1_BLINK = osThreadCreate(osThread(LED1), NULL);
  
	//CREATE THREAD FOR LED2
	LED2_BLINK = osThreadCreate(osThread(LED2), NULL);
  
	//CREATE THREAD FOR INBUILT LED
	IN_BUILT_BLINK = osThreadCreate(osThread(IN_BUILT_LED), NULL);
	
	//START SCHEDULER
	osKernelStart();
	
	//NOW CONTROL ON THE SCHEDULER, WE WILL NOT COME HERE
	
	for (;;)
		;
}

//Handles the tick increment
void SysTick_Handler(void)
{
	osSystickHandler();
}


static void LED1_Thread(void const *argument)
{
	(void) argument;
  
	for (;;)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
		osDelay(LED1_BLINK_TIME);	
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
		osDelay(LED1_BLINK_TIME);
	}
}

static void IN_BUILT_Thread(void const *argument)
{
	(void) argument;
  
	for (;;)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
		osDelay(IN_BUILT_BLINK_TIME);
		
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
		osDelay(IN_BUILT_BLINK_TIME);
		
	}
}


static void LED2_Thread(void const *argument)
{
	uint32_t count;
	(void) argument;
  
	for (;;)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
		osDelay(LED2_BLINK_TIME);
		
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
		osDelay(LED2_BLINK_TIME);

	}
}

//ASSERT ERROR CALLBACK FUNCTION FOR ERROR PRINTING
#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{
 
	while (1)
	{
		printf("Wrong parameters value: file %s on line %d\r\n", file, line);
	}
}
#endif


