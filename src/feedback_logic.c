#include "feedback_logic.h"

#include <stdio.h>

void feedback_format_message(char * buffer, size_t buffer_size, uint32_t feedback_count)
{
    if(buffer == NULL || buffer_size == 0) {
        return;
    }

    snprintf(buffer,
             buffer_size,
             "Thanks for your feedback! (%lu)",
             (unsigned long)feedback_count);
}