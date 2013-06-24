#ifndef ROOX_ENDPOINTS_H
#define ROOX_ENDPOINTS_H

typedef struct endpoint_struct {
    char *uri;
    void (*callback) (int, ...);
} endpoint_t;

#endif

