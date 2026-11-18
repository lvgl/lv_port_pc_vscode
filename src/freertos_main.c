/**
 * @file    Freertos main file
 * @author  MootSeeker
 * @date    2024-09-02
 * @brief   Main file for FreeRTOS tasks and hooks.
 * @license MIT License
 */

#include "lvgl/lvgl.h"

#include "FreeRTOS.h"
#include "task.h"

#if LV_USE_OS == LV_OS_FREERTOS

#include "hal/hal.h"
#include <stdio.h>

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
#if defined(__APPLE__)

/* macOS/iOS: SDL's video init + event polling MUST run on the main thread
 * (Cocoa constraint). The FreeRTOS POSIX port normally occupies the main
 * thread as the scheduler, so we invert that: put the scheduler on a
 * background pthread and keep the main thread for SDL/LVGL.
 */
#include <pthread.h>
#include <unistd.h>

/* =========================================================================
 * macOS path
 *
 * Main thread  -> lv_init + sdl_hal_init + lv_timer_handler loop
 * Bg pthread   -> vTaskStartScheduler
 * FreeRTOS     -> another_task (proves scheduler runs)
 * ========================================================================= */

/**
 * @brief   Entry point for the background scheduler pthread.
 *
 * Keeps the main thread free for SDL/Cocoa.
 */
static void *scheduler_thread(void *pvParameters)
{
    (void)pvParameters;
    vTaskStartScheduler();
    return NULL;
}

/**
 * @brief   LVGL task placeholder (unused on macOS)
 *
 * On macOS the LVGL rendering loop runs on the main thread directly from
 * main() to satisfy the Cocoa threading constraint. This function exists
 * only to keep the original task naming for cross-platform clarity.
 */
void lvgl_task(void *pvParameters)
{
    (void)pvParameters;
}

/**
 * @brief   Another task — runs under the scheduler.
 *
 * Periodically prints to prove the scheduler is working concurrently with
 * the main-thread LVGL loop.
 */
void another_task(void *pvParameters)
{
    (void)pvParameters;
    while (true) {
        printf("Second Task is running :)\n");
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

/**
 * @brief   Entry point (macOS path)
 */
int main(int argc, char **argv)
{
    LV_UNUSED(argc);
    LV_UNUSED(argv);

    /* Unbuffered so prints from main thread and scheduler threads interleave cleanly. */
    setvbuf(stdout, NULL, _IONBF, 0);

    /* Initialize LVGL (no SDL yet). */
    lv_init();

    /* Create the FreeRTOS task. xTaskCreate triggers the POSIX port's
     * prvSetupSignalsAndSchedulerPolicy(), which sets the caller's signal
     * mask and installs SIGALRM handler for tick generation. Any SDL call
     * must happen *after* this — SDL_Init mutates the process signal mask
     * and would otherwise break the tick mechanism
     */
    if (xTaskCreate(another_task, "Another Task", 1024, NULL, 1, NULL) != pdPASS) {
        printf("Error creating another task\n");
    }

    /* Start the scheduler on a background pthread so the main thread stays
     * available for SDL video init + event polling (Cocoa requires these on
     * the main thread).
     */
    pthread_t sched;
    if (pthread_create(&sched, NULL, scheduler_thread, NULL) != 0) {
        printf("Error creating scheduler thread\n");
        return 1;
    }

    /* SDL init must be on the main thread (Cocoa constraint). Do it AFTER
     * xTaskCreate + pthread_create so it cannot perturb FreeRTOS internals.
     */
    sdl_hal_init(320, 480);
    create_hello_world_screen();

    /* Main thread: LVGL rendering loop + SDL event pump. */
    while (true) {
        uint32_t sleep_time_ms = lv_timer_handler();
        if (sleep_time_ms == LV_NO_TIMER_READY) {
            sleep_time_ms = LV_DEF_REFR_PERIOD;
        }
        usleep(sleep_time_ms * 1000);
    }

    return 0;
}

#else /* !__APPLE__ — Linux / Windows / ESP / other POSIX RTOS backends */

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

    /*Initialize LVGL*/
    lv_init();

    /*Initialize the HAL (display, input devices, tick) for LVGL*/
    sdl_hal_init(320, 480);
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
int main(int argc, char **argv)
{
    LV_UNUSED(argc);
    LV_UNUSED(argv);
    /* Initialize LVGL (Light and Versatile Graphics Library) and other resources */

    /* Create the LVGL task */
    if (xTaskCreate(lvgl_task, "LVGL Task", 4096, NULL, 1, NULL) != pdPASS) {
        printf("Error creating LVGL task\n");
        /* Error handling */
    }

    /* Create another task */
    if (xTaskCreate(another_task, "Another Task", 1024, NULL, 1, NULL) != pdPASS) {
        printf("Error creating another task\n");
        /* Error handling */
    }

    /* Start the scheduler */
    vTaskStartScheduler();
}

#endif /* __APPLE__ */

#endif /* LV_USE_OS == LV_OS_FREERTOS */
