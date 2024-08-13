/*********************
 *      INCLUDES
 *********************/

#include "gui_comm.h"
#include "gui_data_comm.h"

static gui_data_t *p_gui_data = NULL;


uint8_t gui_data_get_bg_src_id(void)
{
	return p_gui_data->bg_src_id;
}

void *gui_data_get_bg_src(void)
{
	return p_gui_data->bg_src;
}

void gui_data_set_bg_src_id(uint8_t id)
{
    p_gui_data->bg_src_id = id;

	if(id = 1)
		p_gui_data->bg_src = (void *)&img_bg1;
	else if(id = 2)
		p_gui_data->bg_src = (void *)&img_bg2;
	else if(id = 3)
		p_gui_data->bg_src = (void *)&img_bg3;
	else if(id = 4)
		p_gui_data->bg_src = (void *)&img_bg4;
	else if(id = 5)
		p_gui_data->bg_src = (void *)&img_bg5;
	else if(id = 6)
		p_gui_data->bg_src = (void *)&img_bg6;
	else if(id = 7)
		p_gui_data->bg_src = (void *)&img_bg7;
}

char *gui_data_get_language(void)
{
	printf("%s %s\n", __func__, p_gui_data->language);
	return p_gui_data->language;
}

void gui_data_set_language(char *language)
{
	printf("%s %s\n", __func__, language);
	lv_strcpy(p_gui_data->language, language);
}

char *gui_data_get_pin(void)
{
	printf("%s %s\n", __func__, p_gui_data->pin);
	return p_gui_data->pin;
}

void gui_data_set_pin(char *pin)
{
	printf("%s %s\n", __func__, pin);
	lv_strcpy(p_gui_data->pin, pin);
}

uint8_t gui_data_get_word_num(void)
{
	printf("%s %d\n", __func__, p_gui_data->word_num);
	return p_gui_data->word_num;
}

void gui_data_set_word_num(uint8_t word_num)
{
	printf("%s %d\n", __func__, word_num);
	p_gui_data->word_num = word_num;
}

char *gui_data_get_word(uint8_t word_index)
{
	printf("%s index:%d %s\n", __func__, word_index, p_gui_data->word[word_index]);
	return p_gui_data->word[word_index];
}

void gui_data_set_word(uint8_t word_index, char *word)
{
	printf("%s index:%d %s\n", __func__, word_index, word);
	lv_strcpy(p_gui_data->word[word_index], word);
}

int gui_data_get_shutdown_time(void)
{
	printf("%s %d\n", __func__, p_gui_data->shutdown_time);
	return p_gui_data->shutdown_time;
}

void gui_data_set_shutdown_time(int shutdown_time)
{
	printf("%s %d\n", __func__, shutdown_time);
	p_gui_data->shutdown_time = shutdown_time;
}

int gui_data_get_lock_time(void)
{
	printf("%s %d\n", __func__, p_gui_data->lock_time);
	return p_gui_data->lock_time;
}

void gui_data_set_lock_time(int lock_time)
{
	printf("%s %d\n", __func__, lock_time);
	p_gui_data->lock_time = lock_time;
}

void gui_data_init(void)
{
    p_gui_data = (gui_data_t*)lv_malloc(sizeof(gui_data_t));
    lv_memset(p_gui_data, 0, sizeof(gui_data_t));

    gui_data_set_bg_src_id(1);
    gui_data_set_language("English");
    gui_data_set_pin("666666");
    gui_data_set_word_num(12);
    for(uint8_t i = 0; i < gui_data_get_word_num(); i++)
    {
	    gui_data_set_word(i, "right_word");
    }
    gui_data_set_shutdown_time(10);
    gui_data_set_lock_time(10);
}

void gui_data_deinit(void)
{
    lv_free(p_gui_data);
    p_gui_data = NULL;
}


