/**
 * @file main
 *
 */

/*********************
 *      INCLUDES
 *********************/
#define _DEFAULT_SOURCE /* usleep()需要此定义 */
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "lvgl/lvgl.h"
#include "lvgl/examples/lv_examples.h"
#include "lvgl/demos/lv_demos.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  静态函数声明
 **********************/
static lv_display_t * hal_init(int32_t w, int32_t h);

/**********************
 *  静态变量
 **********************/

/**********************
 *      宏定义
 **********************/

/**********************
 *   全局函数
 **********************/

int main(int argc, char **argv)
{
  (void)argc; /* 未使用 */
  (void)argv; /* 未使用 */

  /* 初始化LVGL */
  lv_init();

  /* 初始化LVGL的HAL（显示，输入设备，计时） */
  hal_init(480, 272);

  /* 创建主屏幕 */
  lv_obj_t * scr = lv_scr_act();

  /* 创建一个用于放置方块的网格 */
  static lv_coord_t col_dsc[] = {80, 10, 80, LV_GRID_TEMPLATE_LAST};  // 每个方块80px，间隙10px
  static lv_coord_t row_dsc[] = {80, 10, 80, 10, 80, LV_GRID_TEMPLATE_LAST};  // 每个方块80px，间隙10px
  
  lv_obj_t * grid = lv_obj_create(scr);
  lv_obj_set_size(grid, 240, 320);  // 宽度为240px，高度为320px
  lv_obj_center(grid);
  lv_obj_set_layout(grid, LV_LAYOUT_GRID);
  lv_obj_set_grid_dsc_array(grid, col_dsc, row_dsc);

  /* 创建方块的样式 */
  static lv_style_t style_block;
  lv_style_init(&style_block);
  lv_style_set_bg_color(&style_block, lv_color_hex(0xCCCCCC));  /* 灰色背景 */
  lv_style_set_radius(&style_block, 10);  /* 圆角 */

  /* 创建方块 */
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 2; j++) {
      lv_obj_t * block = lv_btn_create(grid);
      lv_obj_set_size(block, 80, 80);  // 设置大小为80px x 80px
      lv_obj_add_style(block, &style_block, 0);
      lv_obj_set_grid_cell(block, LV_GRID_ALIGN_CENTER, j * 2, 1, LV_GRID_ALIGN_CENTER, i * 2, 1);
    }
  }
  
  while(1) {
    /* 定期调用lv_task处理器。
     * 这也可以在定时器中断或操作系统任务中完成。*/
    lv_timer_handler();
    usleep(5 * 1000);
  }

  return 0;
}

/**********************
 *   静态函数
 **********************/

/**
 * 初始化LVGL图形库的硬件抽象层（HAL）
 */
static lv_display_t * hal_init(int32_t w, int32_t h)
{

  lv_group_set_default(lv_group_create());

  lv_display_t * disp = lv_sdl_window_create(w, h);

  lv_indev_t * mouse = lv_sdl_mouse_create();
  lv_indev_set_group(mouse, lv_group_get_default());
  lv_indev_set_display(mouse, disp);
  lv_display_set_default(disp);

  LV_IMAGE_DECLARE(mouse_cursor_icon); /* 声明图像文件。*/
  lv_obj_t * cursor_obj;
  cursor_obj = lv_image_create(lv_screen_active()); /* 创建一个用于光标的图像对象 */
  lv_image_set_src(cursor_obj, &mouse_cursor_icon); /* 设置图像源 */
  lv_indev_set_cursor(mouse, cursor_obj); /* 将图像对象连接到驱动程序 */

  lv_indev_t * mousewheel = lv_sdl_mousewheel_create();
  lv_indev_set_display(mousewheel, disp);
  lv_indev_set_group(mousewheel, lv_group_get_default());

  lv_indev_t * kb = lv_sdl_keyboard_create();
  lv_indev_set_display(kb, disp);
  lv_indev_set_group(kb, lv_group_get_default());

  return disp;
}
