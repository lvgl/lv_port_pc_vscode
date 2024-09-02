/**
 * @file Freertos main file
 * @brief Main file for FreeRTOS tasks and hooks.
 */

#include "lvgl/src/osal/lv_os.h"

#if LV_USE_OS == LV_OS_FREERTOS

#include "lvgl/examples/lv_examples.h"
#include <cstdio>  // For printf in C++

// Task identifiers used for managing FreeRTOS tasks
enum TaskIdentifier_t {
    TASK_LVGL,          // Task for LVGL rendering and management
    TASK_ANOTHER_TASK,  // Additional task (extend as needed)
    TASK_MAX            // Count of tasks, should always be the last item
};

// Structure to hold handles for created tasks
struct TaskHandles_t {
    lv_thread_t lvgl_thread;  // LVGL thread handle
    TaskHandle_t another_task_handle;  // Another task handle
};

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
extern "C" void vApplicationMallocFailedHook(void)
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
extern "C" void vApplicationIdleHook(void) {}

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
extern "C" void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
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
extern "C" void vApplicationTickHook(void) {}

// ........................................................................................................
/**
 * @brief   Create Hello World screen
 *
 * This function creates a simple LVGL screen with a "Hello, World!" label centered on the screen.
 *
 * @param   None
 * @return  None
 */
void create_hello_world_screen()
{
    // Create a new screen
    lv_obj_t *screen = lv_obj_create(nullptr);

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

    while (true)
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
    while (true)
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
extern "C" void freertos_main()
{
    // Initialisiere LVGL und andere Ressourcen

    // Erstelle den LVGL-Task
    lv_thread_prio_t prio = static_cast<lv_thread_prio_t>(tskIDLE_PRIORITY + 1);
    if (lv_thread_init(&task_handles.lvgl_thread, prio, lvgl_task, 16384, nullptr) != LV_RESULT_OK) {
        printf("Fehler beim Erstellen des LVGL-Tasks\n");
        // Fehlerbehandlung
    }

    // Erstelle einen weiteren Task
    if (xTaskCreate(another_task, "Another Task", 4096, nullptr, 1, &task_handles.another_task_handle) != pdPASS) {
        printf("Fehler beim Erstellen eines weiteren Tasks\n"); 
        // Fehlerbehandlung
    }

    // Starte den Scheduler
    vTaskStartScheduler();
}


#endif
