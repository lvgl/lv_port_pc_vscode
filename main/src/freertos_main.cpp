/**
 * @file    Freertos main file
 * @author  MootSeeker
 * @date    2024-09-02
 * @brief   Main file for FreeRTOS tasks and hooks.
 * @license MIT License
 */

#include "lvgl/src/osal/lv_os.h"

#if LV_USE_OS == LV_OS_FREERTOS

#include "lvgl.h"
#include <cstdio>  // For printf in C++

/**
 * Initialize the Hardware Abstraction Layer (HAL) for the LVGL graphics
 * library
 */
static lv_display_t * hal_init(int32_t w, int32_t h);

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
    /* Create a new screen object */
    lv_obj_t *screen = lv_obj_create(NULL);
    if (screen == NULL){
        printf("Error: Failed to create screen object\n");
        /* Return if screen creation fails */
        return;
    }

    /* Create a new label object on the screen */
    lv_obj_t *label = lv_label_create(screen);
    if (label == NULL){
        printf("Error: Failed to create label object\n");
        /* Return if label creation fails */
        return;
    }

    /* Set the text of the label to "Hello, World!" */
    lv_label_set_text(label, "Hello, World!");

    /* Align the label to the center of the screen */
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    /* Load the created screen and make it visible */
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
    /* Initialize LVGL*/
    /* When USE_FREERTOS_TASK_NOTIFY rtos param is configured
     * we shall initialize lvgl in a task context o
     */
    lv_init();

    /*Initialize the HAL (display, input devices, tick) for LVGL*/
    hal_init(320, 480);
    /* Show simple hello world screen */
    create_hello_world_screen();

    while (true){
        lv_timer_handler(); /* Handle LVGL tasks */
        vTaskDelay(pdMS_TO_TICKS(5)); /* Short delay for the RTOS scheduler */
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
    /* Create some load in an infinite loop */
    while (true){
        printf("Second Task is running :)\n");
        /* Delay the task for 500 milliseconds */
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
    /* Initialize LVGL (Light and Versatile Graphics Library) and other resources */

    /* Create the LVGL task */
    if (xTaskCreate(lvgl_task, "LVGL Task", 4096, nullptr, 3, nullptr) != pdPASS) {
        printf("Error creating LVGL task\n");
        /* Error handling */
    }

    /* Create another task */
    if (xTaskCreate(another_task, "Another Task", 1024, nullptr, 3, nullptr) != pdPASS) {
        printf("Error creating another task\n");
        /* Error handling */
    }

    /* Start the scheduler */
    vTaskStartScheduler();
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Initialize the Hardware Abstraction Layer (HAL) for the LVGL graphics
 * library
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
  lv_indev_set_cursor(mouse, cursor_obj);             /*Connect the image  object to the driver*/

  lv_indev_t * mousewheel = lv_sdl_mousewheel_create();
  lv_indev_set_display(mousewheel, disp);
  lv_indev_set_group(mousewheel, lv_group_get_default());

  lv_indev_t * kb = lv_sdl_keyboard_create();
  lv_indev_set_display(kb, disp);
  lv_indev_set_group(kb, lv_group_get_default());

  return disp;
}
#endif
