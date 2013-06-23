/*
 * AUTHOR: Abhijeet Rastogi (http://www.google.com/profiles/abhijeet.1989)
 *
 * This is a very simple HTTP server. Default port is 10000 and ROOT for the server is your current working directory..
 *
 * You can provide command line arguments like:- $./a.aout -p [port] -r [path]
 *
 * for ex. 
 * $./a.out -p 50000 -r /home/
 * to start a server at port 50000 with root directory as "/home"
 *
 * $./a.out -r /home/shadyabhi
 * starts the server at port 10000 with ROOT as /home/shadyabhi
 *
 *
 **************************************************************************
 *
 * Ep0
 *
 * I did NOT write the code for this server
 *
 * Modifications:
 *  - Headers are parsed
 *  - Only one connection at a time
 *
 * It is tied up with Roox
 * 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <fcntl.h>

#include "defines.h"
#include "http.h"
#include "../list/list.h"
#include "../trie/trie_node.h"
#include "../trie/trie.h"
#include "../stack/stack.h"
#include "../pair/pair.h"
#include "../parse_tree/utils.h"
#include "../parse_tree/parse_tree.h"
#include "../sanitizer/sanitizer.h"

int listenfd, clients[CONNMAX];

void respond(int);
int parse_headers(char *header, int *cl);

//start server
void start_server(char *port)
{
    struct addrinfo hints, *res, *p;

    // getaddrinfo for host
    memset (&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if (getaddrinfo( NULL, port, &hints, &res) != 0) {
        perror ("getaddrinfo() error");
        exit(1);
    }

    // socket and bind
    for (p = res; p != NULL; p = p->ai_next) {
        listenfd = socket (p->ai_family, p->ai_socktype, 0);
        if (listenfd == -1) continue;
        if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0) break;
    }

    if (p==NULL) {
        perror ("socket() or bind()");
        exit(1);
    }

    freeaddrinfo(res);

    // listen for incoming connections
    if ( listen (listenfd, 1000000) != 0 ) {
        perror("listen() error");
        exit(1);
    }
    printf ("SERVER STARTED\n\n");
}

void close_stuff(int n) {
    shutdown (clients[n], SHUT_RDWR);
    close(clients[n]);
    clients[n]=-1;
}

/*
 * This is the shit right here
 * 
 * This is quite a big function, that needs to be broken
 *
 */
void respond(int n) {
    char request_method[10];
    char request_path[256];
    char request_http_version[10];

    char *mesg; 
    int rcvd, fd, bytes_read;
    int sscanf_ret;
    int content_length = -1;
    int data_start_index, i;

    mesg = (char*) malloc(99999 * sizeof(char));
    memset( (void*)mesg, (int)'\0', 99999 );

    rcvd = recv(clients[n], mesg, 99999, 0);

    if (rcvd < 0) {
        fprintf(stderr,("recv() error\n"));
        close_stuff(n); return;
    } 
    // Socket closed ??
    if (rcvd==0) {
        fprintf(stderr,"Client disconnected upexpectedly.\n");
        close_stuff(n); return;
    }
    
    /*
     * Let the magic commence
     */
    printf("%s", mesg);

    sscanf_ret = sscanf(mesg, "%s %s %s \r\n", request_method, request_path, request_http_version);

    if (EOF == sscanf_ret) {
        // Don't even bother to respond
        close_stuff(n);
        return;
    }

    // Do we have POST ??
    if (0 != strcmp(request_method, "POST")) {
        write(clients[n], METHOD_NOT_SUPPORTED, METHOD_NOT_SUPPORTED_L);
        close_stuff(n);
        return;
    }

    // For HTTP's sake... obey
    if ((0 != strcmp(request_http_version, "HTTP/1.0")) && (0 != strcmp(request_http_version, "HTTP/1.1"))) {
        write(clients[n], VERSION_NOT_SUPPORTED, VERSION_NOT_SUPPORTED_L);

        close_stuff(n);
        return;
    }

    // We got method, path and http version: change mesg
    // That 4 comes from the three spaces and \r and \n
    mesg += strlen(request_method) + strlen(request_path) + strlen(request_http_version) + 4;
    printf("\n\n\nMESG:%s\n\n\n\n", mesg);

    data_start_index =  parse_headers(mesg, &content_length);

    // If time found, return a message to let the user know what went wrong
    if (0 > data_start_index) {
        write(clients[n], HTTP_BR, HTTP_BR_L);
    }

    printf("DSI: %d\n", data_start_index);
    printf("\n\nMESG:%s", mesg + data_start_index);
    write(clients[n], HTTP_OK, HTTP_OK_L);

    close_stuff(n);

}

/*
 * This is quite a pretty ugly function that tries to parse headers
 */
int parse_headers(char *headers, int *cl) {
    int colon_position = -1;
    int i = 0, j, do_process = 0;
    char *header_key, *header_value;

    // found Content-Length, found Content-Type
    int found_cl = 0, found_ct = 0;

    // data start index
    int dsi = 1;

    *cl = 0;

    while (1) {
        dsi++;
        if ('\0' == headers[i]) {
            // Let's hope we won't get here
            return 0;
        }
        if ('\r' == headers[i]) {
            // Where's a CR, there should be LF
            if (('\0' == headers[i + 1]) || ('\n' != headers[i + 1])) {
                return -2;
            }
            i --;
            do_process = 1;
        }
        if (('\n' == headers[i]) && ('\r' != headers[i - 1])) {
            // Why LF without CR ??
            return -1;
        }

        if ((':' == headers[i]) && (-1 == colon_position)) {
            colon_position = i;
        }

        if (0 == do_process) {
            i++;
            continue;
        }

        if (-1 == colon_position) {
            // We got to the data :D
            free(header_key);
            free(header_value);
            if ((0 == found_cl) || (0 == found_ct)) {
                return -4;
            }
            return dsi;
        }
        header_key = (char *) malloc((colon_position) * sizeof(char));

        header_key[colon_position] = '\0';
        j = 0;
        while (j < colon_position) {
            header_key[j] = headers[j];
            j++;
        }
        headers += colon_position + 2;

        header_value = (char *) malloc((i - colon_position) * sizeof(char));
        header_value[i - colon_position] = '\0';
        j = 0;
        while (j < (i - colon_position - 1)) {
            header_value[j] = headers[j];
            j++;
        }
        headers += (i - colon_position + 1);

        // We have headers
        if (0 == strcmp(header_key, "Content-Type")) {
            found_ct = 1;
            if (0 != strcmp(header_value, "application/javascript")) {
                return -5;
            }
        }

        if (0 == strcmp(header_key, "Content-Length")) {
            found_cl = 1;
            *cl = atoi(header_value);
        }

        colon_position = -1;
        do_process = 0;
        i = 0;
        dsi++;
    } 
    return -3;
}
