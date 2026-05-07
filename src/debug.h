#ifndef DEBUG_H
#define DEBUG_H

#ifdef DEBUG
#define DEBUG_PRINT(format, ...)                       \
    fprintf(stdout, "DEBUG: %s:%d:%s(): " format "\n", \
            __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define PARENT_PRINT(fmt, ...) printf(FG_BRIGHT_GREEN "[PARENT]" COLOR_RESET " " fmt "\n", ##__VA_ARGS__)
#define CHILD_PRINT(fmt, ...) printf(FG_BRIGHT_CYAN "[CHILD]" COLOR_RESET " " fmt "\n", ##__VA_ARGS__)
#else
#define DEBUG_PRINT(...) ((void)0)
#define CHILD_PRINT(...) ((void)0)
#define PARENT_PRINT(...) ((void)0)

#endif
#endif