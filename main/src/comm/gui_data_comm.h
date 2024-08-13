/*********************
 *      INCLUDES
 *********************/
#ifndef __GUI_DATA_COMM_H__
#define __GUI_DATA_COMM_H__


#include <lvgl.h>
#include <string.h>
#include <stdio.h>

typedef struct
{
	uint8_t bg_src_id;		//保存的背景图序号
	void *bg_src;			//背景图资源
	
	char pin[8];			//保存的密码
	char language[32];		//保存的多语言类型
	
	uint8_t word_num;		//设置的助记词数量
	char word[24][16];		//保存的助记词内容

	int lock_time;
	int shutdown_time;
	
} gui_data_t;

uint8_t gui_data_get_bg_src_id(void);
void *gui_data_get_bg_src(void);
void gui_data_set_bg_src_id(uint8_t id);
char *gui_data_get_language(void);
void gui_data_set_language(char *language);
char *gui_data_get_pin(void);
void gui_data_set_pin(char *pin);
uint8_t gui_data_get_word_num(void);;
void gui_data_set_word_num(uint8_t word_num);
char *gui_data_get_word(uint8_t word_index);
void gui_data_set_word(uint8_t word_index, char *word);
int gui_data_get_shutdown_time(void);
void gui_data_set_shutdown_time(int shutdown_time);
int gui_data_get_lock_time(void);
void gui_data_set_lock_time(int lock_time);



void gui_data_init(void);
void gui_data_deinit(void);

#endif /* __GUI_DATA_COMM_H__ */


