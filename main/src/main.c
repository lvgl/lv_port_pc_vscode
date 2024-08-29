/*
 * @file    main.cpp
 * @brief   Main application file for initializing FreeRTOS and LVGL.
 *
 * This file contains the main function and other necessary hooks and tasks
 * to run a FreeRTOS-based application with the LVGL graphics library.
 *
 * @author  MootSeeker
 * @date    2024-08-27
 *
 * @license MIT License
 *
 * Copyright (c) 2024 MootSeeker
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "glob.h"


    #include "lvgl/lvgl.h"
    #include "ui.h"


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
 * @brief	HAL initialization
 *
 * This function initializes the hardware abstraction layer (HAL) for LVGL, including display and input devices.
 *
 * @param	w   Width of the display
 * @param	h   Height of the display
 * @return 	Pointer to the initialized display
 */
static lv_display_t * hal_init(int32_t w, int32_t h)
{
    lv_group_set_default(lv_group_create());

    lv_display_t * disp = lv_sdl_window_create(w, h);

    lv_indev_t * mouse = lv_sdl_mouse_create();
    lv_indev_set_group(mouse, lv_group_get_default());
    lv_indev_set_display(mouse, disp);
    lv_display_set_default(disp);

    LV_IMAGE_DECLARE(mouse_cursor_icon); /*Declare the image file.*/
    lv_obj_t * cursor_obj;
    cursor_obj = lv_image_create(lv_screen_active()); /*Create an image object for the cursor */
    lv_image_set_src(cursor_obj, &mouse_cursor_icon);           /*Set the image source*/
    lv_indev_set_cursor(mouse, cursor_obj);             /*Connect the image object to the driver*/

    lv_indev_t * mousewheel = lv_sdl_mousewheel_create();
    lv_indev_set_display(mousewheel, disp);
    lv_indev_set_group(mousewheel, lv_group_get_default());

    lv_indev_t * kb = lv_sdl_keyboard_create();
    lv_indev_set_display(kb, disp);
    lv_indev_set_group(kb, lv_group_get_default());

    return disp;
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

// ........................................................................................................
/*
 * @brief	Main function
 *
 * The main entry point of the program. It starts the FreeRTOS scheduler and creates the LVGL task.
 * The scheduler is expected to take over, and this function should never return.
 *
 * @param	argc   Argument count (not used)
 * @param	argv   Argument vector (not used)
 * @return 	Always returns 0
 */
int main(int argc, char **argv)
{
    (void)argc; /*Unused*/
    (void)argv; /*Unused*/

    lv_init();
    hal_init(320, 480);
    ui_init();

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

    return 0;
}
