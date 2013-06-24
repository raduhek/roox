#ifndef ROOX_HTTP_H
#define ROOX_HTTP_H

#include "defines.h"
#include "../endpoints.h"

extern int clients[CONNMAX];
extern int listenfd;

void start_server(char *);
void respond(int, endpoint_t *);

#endif

