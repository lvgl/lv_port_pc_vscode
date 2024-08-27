/**
 * @file Freertos main file
 * @brief Main file for FreeRTOS tasks and hooks.
 */

#if LV_USE_OS == LV_OS_FREERTOS

#include "glob.h"
#include "FreeRTOS.h"
#include "task.h"
#include "lvgl/lvgl.h"
#include "lvgl/examples/lv_examples.h"

// Task identifiers used for managing FreeRTOS tasks
typedef enum {
    TASK_LVGL,          // Task for LVGL rendering and management
    TASK_ANOTHER_TASK,  // Additional task (extend as needed)
    TASK_MAX            // Count of tasks, should always be the last item
} TaskIdentifier_t;

// Structure to hold handles for created tasks
typedef struct {
    TaskHandle_t handles[TASK_MAX];
} TaskHandles_t;

// Global instance of task handles
TaskHandles_t task_handles;

// ........................................................................................................
/**
 * @brief   Malloc failed hook
 *
 * This function is called when a memory allocation (malloc) fails. It logs the available heap size and enters
 * an infinite loop to halt the system.
 *
 * @param   None
 * @return  None
 */
void vApplicationMallocFailedHook(void)
{
    printf("Malloc failed! Available heap: %ld bytes\n", xPortGetFreeHeapSize());
    for( ;; );
}

// ........................................................................................................
/**
 * @brief   Idle hook
 *
 * This function is called when the system is idle. It can be used for low-power mode operations or other
 * maintenance tasks that need to run when the CPU is not busy.
 *
 * @param   None
 * @return  None
 */
void vApplicationIdleHook(void) {}

// ........................................................................................................
/**
 * @brief   Stack overflow hook
 *
 * This function is called when a stack overflow is detected in a task. It logs the task name and enters
 * an infinite loop to halt the system.
 *
 * @param   xTask        Handle of the task that caused the stack overflow
 * @param   pcTaskName   Name of the task that caused the stack overflow
 * @return  None
 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    printf("Stack overflow in task %s\n", pcTaskName);
    for(;;);
}

// ........................................................................................................
/**
 * @brief   Tick hook
 *
 * This function is called on each tick interrupt. It can be used to execute periodic operations
 * that need to occur at a fixed time interval.
 *
 * @param   None
 * @return  None
 */
void vApplicationTickHook(void) {}

// ........................................................................................................
/**
 * @brief   Get idle task memory
 *
 * This function provides the memory required for the idle task. It must be implemented by the user
 * when static allocation is enabled (configSUPPORT_STATIC_ALLOCATION).
 *
 * @param   ppxIdleTaskTCBBuffer   Pointer to the TCB buffer for the idle task
 * @param   ppxIdleTaskStackBuffer Pointer to the stack buffer for the idle task
 * @param   pulIdleTaskStackSize   Pointer to the size of the idle task stack
 * @return  None
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
/**
 * @brief   Get timer task memory
 *
 * This function provides the memory required for the timer task. It must be implemented by the user
 * when static allocation is enabled (configSUPPORT_STATIC_ALLOCATION).
 *
 * @param   ppxTimerTaskTCBBuffer   Pointer to the TCB buffer for the timer task
 * @param   ppxTimerTaskStackBuffer Pointer to the stack buffer for the timer task
 * @param   pulTimerTaskStackSize   Pointer to the size of the timer task stack
 * @return  None
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
/**
 * @brief   Daemon task startup hook
 *
 * This function is called before the daemon task starts running. It can be used to perform
 * initialization tasks that need to be completed before the daemon task executes.
 *
 * @param   None
 * @return  None
 */
void vApplicationDaemonTaskStartupHook(void)
{
    // Initialization code here
}

// ........................................................................................................
/**
 * @brief   Create Hello World screen
 *
 * This function creates a simple LVGL screen with a "Hello, World!" label centered on the screen.
 *
 * @param   None
 * @return  None
 */
void create_hello_world_screen(void)
{
    // Create a new screen
    lv_obj_t *screen = lv_obj_create(NULL);

    // Create a label object on the screen
    lv_obj_t *label = lv_label_create(screen);

    // Set the text of the label to "Hello, World!"
    lv_label_set_text(label, "Hello, World!");

    // Position the label in the center of the screen
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    // Activate the new screen
    lv_scr_load(screen);
}

// ........................................................................................................
/**
 * @brief   LVGL task
 *
 * This task initializes LVGL and runs the main loop, periodically calling the LVGL task handler.
 * It is responsible for managing the LVGL state and rendering updates.
 *
 * @param   pvParameters   Task parameters (not used in this example)
 * @return  None
 */
void lvgl_task(void *pvParameters)
{
    // Show simple hello world screen
    create_hello_world_screen(); 

    while (1)
    {
        lv_timer_handler(); // Handle LVGL tasks
        vTaskDelay(pdMS_TO_TICKS(5)); // Short delay for the RTOS scheduler
    }
}

// ........................................................................................................
/**
 * @brief   Another task
 *
 * This task simulates some background work by periodically printing a message.
 *
 * @param   pvParameters   Task parameters (not used in this example)
 * @return  None
 */
void another_task(void *pvParameters)
{
    // Create some load
    while (1)
    {
        printf("Second Task is running :)\n");
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

// ........................................................................................................
/**
 * @brief   FreeRTOS main function
 *
 * This function sets up and starts the FreeRTOS tasks, including the LVGL task and another demo task.
 *
 * @param   None
 * @return  None
 */
void freertos_main(void)
{
    // Create LVGL task 
    if (xTaskCreate(lvgl_task, "LVGL Task", 16384, NULL, 1, &task_handles.handles[TASK_LVGL]) != pdPASS) {
        printf("Failed to create LVGL Task\n");
        // Error handling, e.g., stop the program or take other measures
    }

    // Create a second demo task
    if (xTaskCreate(another_task, "Another Task", 4096, NULL, 1, &task_handles.handles[TASK_ANOTHER_TASK]) != pdPASS) {
        printf("Failed to create Another Task\n"); 
    }

    // Start Scheduler
    vTaskStartScheduler();
}

#endif
