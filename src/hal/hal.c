#include "hal.h"

#if LV_USE_SDL
lv_display_t * sdl_hal_init(int32_t w, int32_t h)
{

  lv_group_set_default(lv_group_create());

  lv_display_t * disp = lv_sdl_window_create(w, h);

  lv_indev_t * mouse = lv_sdl_mouse_create();
  lv_indev_set_group(mouse, lv_group_get_default());
  lv_indev_set_display(mouse, disp);
  lv_display_set_default(disp);
  /*Declare the image file.*/
  LV_IMAGE_DECLARE(mouse_cursor_icon); 
  lv_obj_t * cursor_obj;
  /*Create an image object for the cursor */
  cursor_obj = lv_image_create(lv_screen_active()); 
  /*Set the image source*/
  lv_image_set_src(cursor_obj, &mouse_cursor_icon);           
  /*Connect the image  object to the driver*/
  lv_indev_set_cursor(mouse, cursor_obj);             

  lv_indev_t * mousewheel = lv_sdl_mousewheel_create();
  lv_indev_set_display(mousewheel, disp);
  lv_indev_set_group(mousewheel, lv_group_get_default());

  lv_indev_t * kb = lv_sdl_keyboard_create();
  lv_indev_set_display(kb, disp);
  lv_indev_set_group(kb, lv_group_get_default());

  return disp;
}
#endif

#if LV_USE_WINDOWS
lv_display_t* win_hal_init(
    const wchar_t* title,
    int32_t hor_res,
    int32_t ver_res,
    int32_t zoom_level,
    bool allow_dpi_override,
    bool simulator_mode)
{
    lv_display_t* display = lv_windows_create_display(
        title,
        hor_res,
        ver_res,
        zoom_level,
        allow_dpi_override,
        simulator_mode);

    lv_windows_acquire_pointer_indev(display);
    lv_windows_acquire_keypad_indev(display);
    lv_windows_acquire_encoder_indev(display);

	return display;
}
#endif
