#include "glob.h"

#include "FreeRTOS.h"
#include "task.h"

typedef enum {
    TASK_LVGL,
    TASK_ANOTHER_TASK,   // Füge hier weitere Tasks hinzu
    TASK_MAX             // Zählt die Anzahl der Tasks, sollte immer zuletzt stehen
} TaskIdentifier_t;


typedef struct {
    TaskHandle_t handles[TASK_MAX];
} TaskHandles_t;


TaskHandles_t task_handles;


// ........................................................................................................
/*
 * @brief	Malloc failed hook
 *
 * This function is called when a memory allocation (malloc) fails. It logs the available heap size and enters
 * an infinite loop to halt the system.
 *
 * @param	None
 * @return 	None
 */
void vApplicationMallocFailedHook( void )
{
    printf("Malloc failed! Available heap: %ld bytes\n", xPortGetFreeHeapSize());
    for( ;; );
}

// ........................................................................................................
/*
 * @brief	Idle hook
 *
 * This function is called when the system is idle. It can be used for low-power mode operations or other
 * maintenance tasks that need to run when the CPU is not busy.
 *
 * @param	None
 * @return 	None
 */
void vApplicationIdleHook(void) {}

// ........................................................................................................
/*
 * @brief	Stack overflow hook
 *
 * This function is called when a stack overflow is detected in a task. It logs the task name and enters
 * an infinite loop to halt the system.
 *
 * @param	xTask        Handle of the task that caused the stack overflow
 * @param	pcTaskName   Name of the task that caused the stack overflow
 * @return 	None
 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    printf("Stack overflow in task %s\n", pcTaskName);
    for(;;);
}

// ........................................................................................................
/*
 * @brief	Tick hook
 *
 * This function is called on each tick interrupt. It can be used to execute periodic operations
 * that need to occur at a fixed time interval.
 *
 * @param	None
 * @return 	None
 */
void vApplicationTickHook(void) {}

// ........................................................................................................
/*
 * @brief	Get idle task memory
 *
 * This function provides the memory required for the idle task. It must be implemented by the user
 * when static allocation is enabled (configSUPPORT_STATIC_ALLOCATION).
 *
 * @param	ppxIdleTaskTCBBuffer   Pointer to the TCB buffer for the idle task
 * @param	ppxIdleTaskStackBuffer Pointer to the stack buffer for the idle task
 * @param	pulIdleTaskStackSize   Pointer to the size of the idle task stack
 * @return 	None
 */
void vApplicationGetIdleTaskMemory(StaticTask_t** ppxIdleTaskTCBBuffer,
                                   StackType_t** ppxIdleTaskStackBuffer,
                                   uint32_t* pulIdleTaskStackSize)
{
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[128];  // Example size

    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;
    *pulIdleTaskStackSize = 128;  // Example size
}

// ........................................................................................................
/*
 * @brief	Get timer task memory
 *
 * This function provides the memory required for the timer task. It must be implemented by the user
 * when static allocation is enabled (configSUPPORT_STATIC_ALLOCATION).
 *
 * @param	ppxTimerTaskTCBBuffer   Pointer to the TCB buffer for the timer task
 * @param	ppxTimerTaskStackBuffer Pointer to the stack buffer for the timer task
 * @param	pulTimerTaskStackSize   Pointer to the size of the timer task stack
 * @return 	None
 */
void vApplicationGetTimerTaskMemory(StaticTask_t** ppxTimerTaskTCBBuffer,
                                    StackType_t** ppxTimerTaskStackBuffer,
                                    uint32_t* pulTimerTaskStackSize)
{
    static StaticTask_t xTimerTaskTCB;
    static StackType_t uxTimerTaskStack[256];  // Example size

    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;
    *pulTimerTaskStackSize = 256;  // Example size
}

// ........................................................................................................
/*
 * @brief	Daemon task startup hook
 *
 * This function is called before the daemon task starts running. It can be used to perform
 * initialization tasks that need to be completed before the daemon task executes.
 *
 * @param	None
 * @return 	None
 */
void vApplicationDaemonTaskStartupHook(void)
{
    // Initialization code here
}

// ........................................................................................................
/*
 * @brief	LVGL task
 *
 * This task initializes LVGL and runs the main loop, periodically calling the LVGL task handler.
 * It is responsible for managing the LVGL state and rendering updates.
 *
 * @param	pvParameters   Task parameters (not used in this example)
 * @return 	None
 */
void lvgl_task(void *pvParameters)
{
    printf("Start LVGL\n");



    printf("LVGL is running\n");

    while (1)
    {
        lv_timer_handler(); // LVGL-Handling
        vTaskDelay(pdMS_TO_TICKS(5)); // Kurze Pause für den RTOS-Scheduler
    }
}


void another_task(void *pvParameters)
{
    // Task-Code hier
    while (1)
    {
        printf(" Second Task is running in backgroud :)\n");
        vTaskDelay( pdMS_TO_TICKS( 500 ));
    }
}

void freertos_main( void *pvParameters )
{

    printf("Start FreeRTOS\n");

    xTaskCreate( lvgl_task,
                 "LVGL Task",
                 16384,
                 NULL,
                 1,
                 &task_handles.handles[TASK_LVGL]);

    xTaskCreate(
                 another_task,
                 "Another Task",
                 4096,
                 NULL,
                 2,
                 &task_handles.handles[TASK_ANOTHER_TASK]);

    // Start Scheduler
    vTaskStartScheduler();
}
