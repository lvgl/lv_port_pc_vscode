#ifndef FEEDBACK_LOGIC_H
#define FEEDBACK_LOGIC_H

#include <stddef.h>
#include <stdint.h>

void feedback_format_message(char * buffer, size_t buffer_size, uint32_t feedback_count);

#endif