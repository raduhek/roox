#ifndef ROOX_HTTP_H
#define ROOX_HTTP_H

#include "defines.h"

extern int clients[CONNMAX];
extern int listenfd;

void start_server(char *);
void respond(int);

#endif

