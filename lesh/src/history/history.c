#include "history.h"


LeshHistory* lesh_load_history(char* history_file_name) {
    return NULL;
}

void lesh_free_history(LeshHistory* history) {
    size_t idx = 0;
    do {
        size_t cmd_idx = 0;
        char **cmd_ptr = history->history[idx];

        do {
            free(cmd_ptr[cmd_idx]);
        } while(cmd_ptr[++cmd_idx] != NULL);

        free(cmd_ptr);
    } while(history->history[++idx] != NULL);

    free(history);
}

void lesh_history_write_cmd(LeshHistory* history, char **cmd) { 
    if(history->size < history->buff_size) {
        history->history
    } else {
        // TODO realloc array
    }

}

char **lesh_history_read_cmd(LeshHistory* history, int idx) {

}

