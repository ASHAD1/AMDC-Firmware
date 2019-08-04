#ifdef APP_BETA_LABS

#include "cmd_bemfo.h"
#include "../bemfo.h"
#include "../../../sys/commands.h"
#include "../../../sys/defines.h"
#include <string.h>
#include <stdlib.h>

static command_entry_t cmd_entry;

#define NUM_HELP_ENTRIES (1)
static command_help_t cmd_help[NUM_HELP_ENTRIES] = {
    {"reset", "Reset filter state"}
};

void cmd_bemfo_register(void)
{
    // Populate the command entry block
    commands_cmd_init(&cmd_entry,
            "bemfo", "BEMFO commands",
            cmd_help, NUM_HELP_ENTRIES,
            cmd_bemfo
    );

    // Register the command
    commands_cmd_register(&cmd_entry);
}

int cmd_bemfo(int argc, char **argv)
{
    // Handle 'reset' sub-command
    if (argc == 2 && strcmp("reset", argv[1]) == 0) {
        bemfo_reset();
        return SUCCESS;
    }

    return INVALID_ARGUMENTS;
}

#endif // APP_BETA_LABS
