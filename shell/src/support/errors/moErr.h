#ifndef MO_ERR_H
#define MO_ERR_H

typedef enum {
        NONFATAL,
        FATAL
} Fatality;

typedef enum {
        ERR_NONE,
        ERR_INPUT,
        ERR_OUT_OF_MEMORY,
        ERR_FORK
} ErrNo;

ErrNo report_err(Fatality fatal, ErrNo errNo, char* msg,
                        const char* file, int line, const char* func);

#define REPORT_ERR(fatal, errNo, msg) \
        report_err(fatal, errNo, msg, __FILE__, __LINE__, __func__)

#endif