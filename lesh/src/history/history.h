#ifndef LESH_HISTORY_H
#define LESH_HISTORY_H

typedef struct {
    size_t size;
    size_t buff_size;
    char ***history;
    char *history_file_name;
} LeshHistory;

LeshHistory* lesh_load_history(char* history_file_name);

void lesh_free_history(LeshHistory* history);

void lesh_history_write_cmd(LeshHistory* history, char **cmd);

char **lesh_history_read_cmd(LeshHistory* history, int idx);

#endif
