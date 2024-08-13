/*********************
 *      INCLUDES
 *********************/
#ifndef __STARTUP_IMPORT_WORD_H__
#define __STARTUP_IMPORT_WORD_H__


#include <lvgl.h>

typedef struct
{
	lv_obj_t* bg_cont;
	lv_obj_t* label_title;
	lv_obj_t* label_word;

	char word[32];
	uint8_t word_index;
} startup_import_word_t;


void startup_import_word_start(void);
void startup_import_word_stop(void);

#endif /* __STARTUP_IMPORT_WORD_H__ */


