#include "FreeRTOS.h"
#include "task.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "lvgl/lvgl.h"
#include "lvgl/examples/lv_examples.h"
#include "lvgl/demos/lv_demos.h"

// Malloc failed hook
void vApplicationMallocFailedHook( void )
{
    /* Malloc failed! Log or handle the error here. */
    printf("Malloc failed! Available heap: %ld bytes\n", xPortGetFreeHeapSize());
    for( ;; );
}


// Idle hook
void vApplicationIdleHook(void) {}

// Stack overflow hook
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    // Handle the stack overflow here, perhaps by halting the system
    printf("Stack overflow in task %s\n", pcTaskName);
    for(;;);
}

// Tick hook
void vApplicationTickHook(void) {}

/* Prototypes */
static lv_display_t * hal_init(int32_t w, int32_t h);
void lvgl_task(void *pvParameters);

int main(int argc, char **argv)
{
    (void)argc; /*Unused*/
    (void)argv; /*Unused*/

    /* Create LVGL Task */
    xTaskCreate(lvgl_task, "LVGL Task", 4096, NULL, 1, NULL);

    /* Start the FreeRTOS scheduler */
    vTaskStartScheduler();

    /* Scheduler should never return */
    while (1) {}

    return 0;
}

void lvgl_task(void *pvParameters)
{
    /* Initialize LVGL */
    lv_init();

    /* Initialize the HAL (display, input devices, tick) for LVGL */
    hal_init(480, 272);

    /* Load a demo */
    lv_demo_widgets();

    while (1) {
        /* Periodically call the lv_task handler */
        lv_timer_handler();
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

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
