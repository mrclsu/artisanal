#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>


#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PROMPT "▲ > "
#define LESH_PARSE_INITIAL_BUFF_SIZE 64 
#define LESH_PARSE_INITIAL_TOK_BUFF_SIZE 128

#define DEBUG 0
#define DEBUG_PRINT(...) do { if (DEBUG) printf(__VA_ARGS__); } while (0)


int lesh_exec(char **cmd) { 
    if(cmd[0] == NULL) {
        return 0;
    }

    // Check for builtins
    if(strcmp(cmd[0], "exit") == 0) {
        return -69;
    } else if (strcmp(cmd[0], "help") == 0) { 
        printf("Welcome to lesh, the only shell that truly sucks!\n"
                "lesh supports the following bult in commands: help, exit.\n");
        return 0;
    } else if (strcmp(cmd[0], "cd") == 0) {
        if(cmd[1] == NULL) {
            fprintf(stderr, "lesh: cd requires one argument\n");
            return 1;
        };

        if(chdir(cmd[1]) == -1) { 
            perror("lesh");
            fprintf(stderr, "Couldn't change directory to %s\n", cmd[1]);
            return 1;
        }
        return 0;
    }

    pid_t pid = fork();

    switch (pid) {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
    case 0:
        DEBUG_PRINT("\nChild started\n");
        
        if(execvp(cmd[0], cmd) == -1) {
            perror("exec");
            fprintf(stderr, "'%s' command not found\n", cmd[0]);
        }
        _exit(EXIT_FAILURE);
    default:
        DEBUG_PRINT("Child is PID: %jd\n", (intmax_t) pid);
        int status;

        do {
            waitpid(pid, &status, WUNTRACED);
        } while(!WIFEXITED(status) && !WIFSIGNALED(status));

        return status;
    }


    return 1;
}

/*
 * Parses a line into a NULL terminated array of strings
 * Each whitespace delimited string is considered an argument
 */
char **lesh_parse_line(char *line) {
    size_t buff_size = LESH_PARSE_INITIAL_BUFF_SIZE, position = 0;
    char **tokens = malloc(buff_size * sizeof(char*));

    if(!tokens) {
        fprintf(stderr, "lesh: alloc err\n");
        exit(1);
    }
    
    int idx = 0;
    size_t line_len = strlen(line);
    while(idx < line_len && line[idx] != '\n') { 
        DEBUG_PRINT("outer while, idx: %d\n", idx);
        
        if(buff_size == position) {
           buff_size *= 2;
           DEBUG_PRINT("Resizing cmd buffer, old size: %zu, new size: %zu\n", buff_size / 2, buff_size);
           char **new_tokens = realloc(tokens, buff_size * sizeof(char*));
           if (new_tokens == NULL) {
               perror("lesh: realloc");
               exit(EXIT_FAILURE);
           }
           tokens = new_tokens;
        }

        // alloc initial token buffer
        size_t token_size = LESH_PARSE_INITIAL_TOK_BUFF_SIZE;
        size_t token_pos = 0;
        char *token = malloc(token_size * sizeof(char));

        while(!isspace(line[idx]) && idx < line_len) {
            DEBUG_PRINT("\tinner while, idx: %d, token_pos: %zu, line_len: %zu\n", idx, token_pos, line_len);

            if (token_size == token_pos) {
                token_size *= 2;
                DEBUG_PRINT("Resizing token buffer, old size: %zu, new size: %zu\n", token_size / 2, token_size);
                char *new_token = realloc(token, token_size * sizeof(char));
                if (new_token == NULL) {
                    perror("lesh: realloc");
                    exit(EXIT_FAILURE);
                }
                token = new_token;
            }

            token[token_pos++] = line[idx++];
        }

        token[token_pos] = '\0';

        // Skip whitespaces
        do {
            DEBUG_PRINT("skipping whitespace\n");
        } while(isspace(line[++idx]) && idx < line_len);

        tokens[position++] = token;
    }

    tokens[position] = NULL;
    return tokens;
}

char* lesh_read_line() { 
    char *line = NULL;
    size_t len;

    if(getline(&line, &len, stdin) == -1) { 
        if(feof(stdin)) { 
            free(line);
            exit(0); 
        } else { 
            perror("getline");
            exit(1);
        }
    }

    return line;
}


void lesh_main_loop() { 
    char *line = NULL;


    while(1) {
        printf(PROMPT);

        line = lesh_read_line();
        char **cmd = lesh_parse_line(line);
        int status = lesh_exec(cmd);

        // Memory cleanup
        for(int idx = 0; cmd[idx] != NULL; ++idx) {
            free(cmd[idx]);
        }
        free(cmd);
        free(line);

        if(status == -69) {
            exit(0);
        };
    }
}


int main(int argc, char **argv) { 
    // TODO: add support for -c and -h/--help
    lesh_main_loop();
    return 0;
}
