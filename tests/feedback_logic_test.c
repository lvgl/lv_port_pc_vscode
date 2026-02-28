#include "feedback_logic.h"

#include <assert.h>
#include <string.h>

int main(void)
{
    char buffer[96];

    feedback_format_message(buffer, sizeof(buffer), 1);
    assert(strcmp(buffer, "Thanks for your feedback! (1)") == 0);

    feedback_format_message(buffer, sizeof(buffer), 42);
    assert(strcmp(buffer, "Thanks for your feedback! (42)") == 0);

    buffer[0] = '\0';
    feedback_format_message(buffer, sizeof(buffer), 0);
    assert(strcmp(buffer, "Thanks for your feedback! (0)") == 0);

    return 0;
}