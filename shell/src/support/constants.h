#ifndef CONSTANTS_H
#define CONSTANTS_H

// Matches most valid file paths
#define PATH_REGEX "^[\\w@%+=:,./~\\-]+\\.(txt|sh)?$"

// Initial size for automatically-resizing buffers
#define BUFF_INIT 32

// Boolean values
typedef enum {
    FALSE,
    TRUE
} Bool;

#endif