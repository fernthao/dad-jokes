/* Name: Thao Nguyen 
 Case Network ID: ttn60
 The filename: proj5d.c
 Date created: Nov 20, 2025
 Description: Implementation of the server
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include "list.h"
#include "random_joke.h"
#include "create.h"
#include "edit.h"
#include "delete_joke.h"
#include "dadsay.h"
#include "constants.h"

#define REQUIRED_ARGC 2
#define PORT_POS 1
#define QLEN 1
#define BUFLEN 1024

volatile sig_atomic_t keep_running = 1;
int listening_socket = -1; 

void handle_sigint(int sig) {
    (void)sig; // Unused parameter
    keep_running = 0;

    // Close the listening socket
    if (listening_socket >= 0) {
        close(listening_socket);
        listening_socket = -1;
    }
}

int usage (char *progname)
{
    fprintf (stderr,"usage: %s port\n", progname);
    exit (ERROR);
}

int errexit (char *format, char *arg)
{
    fprintf (stderr,format,arg);
    fprintf (stderr,"\n");
    exit (ERROR);
}

void parse_title_msg(char* req, char* title, char* msg, FILE* csp) {
    // first line: title
    memset(req, 0x0, TITLE_LEN);
    if (fgets(req, TITLE_LEN, csp) == NULL) fprintf(stderr, "missing title\n");
    printf("%s", req);
    int lastchar = strlen(req) - 1;
    if (req[lastchar] == '\n') {
        req[lastchar] = '\0';
    }
    strcpy(title, req);

    // lines after that: joke content
    strcpy(msg, "\0");
    char line[JOKE_LINE_LENGTH];
    while (fgets(line, JOKE_LINE_LENGTH, csp) != NULL && strcmp(line,JOKE_END) !=0) {
        printf("%s", line);
        // Add each line to the joke content string
        strcat(msg, line); 

        memset(line, 0x0, sizeof(line));
    }
    printf("%s", line);
    fflush(csp);
}

void parse_req(char* req, FILE* csp) {
    // get first line: should contain method
    if (fgets(req, BUFLEN, csp) == NULL) {
        printf("C -> S: %s\n", req);
        printf("S -> C: ERROR\n");
        fputs("Error: no request or read error\n", csp);
        return;
    }

    printf("C -> S: %s", req);

    // methods not requiring arguments
    if (strcmp(req, "LIST\n") == 0) {
        if(list(csp) == ERROR) {
            printf("S -> C: ERROR\n");
        } else printf("S -> C: SUCCESS\n");
    }
    else if (strcmp(req, "RANDOM\n") == 0) {
        if(random_joke(csp) == ERROR) {
            printf("S -> C: ERROR\n");
        } else printf("S -> C: SUCCESS\n");
    }
    else if (strcmp(req, "DADSAY\n") == 0) {
        if(dadsay(csp) == ERROR) {
            printf("S -> C: ERROR\n");
        } else printf("S -> C: SUCCESS\n");
    }
    // methods requiring arguments 
    else if (strcmp(req, "CREATE\n") == 0) {
        char title[TITLE_LEN];
        char msg[MAX_LINES*JOKE_LINE_LENGTH];

        parse_title_msg(req, title, msg, csp);
        if(create(title, msg, csp) == ERROR) {
            printf("S -> C: ERROR\n");
        } else printf("S -> C: SUCCESS\n");
    }
    else if (strcmp(req, "EDIT\n") == 0) {
        char title[TITLE_LEN];
        char msg[MAX_LINES*JOKE_LINE_LENGTH];

        parse_title_msg(req, title, msg, csp);
        if(edit(title, msg, csp) == ERROR) {
            printf("S -> C: ERROR\n");
        } else printf("S -> C: SUCCESS\n");
    }
    else if (strcmp(req, "DELETE\n") == 0) {
        // next line: title
        memset(req, 0x0, TITLE_LEN);
        if (fgets(req, TITLE_LEN, csp) == NULL) fprintf(stderr, "missing title\n");
        printf("%s", req);
        // remove trailing newline
        int lastchar = strlen(req) - 1;
        if (req[lastchar] == '\n') {
            req[lastchar] = '\0';
        }
        if(delete_joke(req, csp) == ERROR) {
            printf("S -> C: ERROR\n");
        } else printf("S -> C: SUCCESS\n");
    }
    else {
        printf("S -> C: ERROR\n");
        fputs("Error: request not recognized\n", csp);
        fputs("Accepted requests: LIST, RANDOM, CREATE, EDIT, DELETE, DADSAY\n", csp);
        
    }
    fflush(csp);
    return;
}

int main (int argc, char *argv [])
{
    struct sockaddr_in sin;
    struct sockaddr addr;
    struct protoent *protoinfo;
    unsigned int addrlen;
    int sd, sd2;
    
    if (argc != REQUIRED_ARGC) usage (argv [0]);

    // signal handler for SIGINT
    signal(SIGINT, handle_sigint);

    // determine protocol
    if ((protoinfo = getprotobyname (PROTOCOL)) == NULL)
        errexit ("cannot find protocol information for %s", PROTOCOL);

    // setup endpoint info
    memset ((char *)&sin,0x0,sizeof (sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons ((unsigned short int) atoi (argv [PORT_POS]));

    // allocate a socket
    sd = socket(PF_INET, SOCK_STREAM, protoinfo->p_proto);
    if (sd < 0)
        errexit("cannot create socket", NULL);

    // Store socket in global variable for signal handler
    listening_socket = sd;

    // bind the socket 
    if (bind (sd, (struct sockaddr *)&sin, sizeof(sin)) < 0)
        errexit ("cannot bind to port %s", argv [PORT_POS]);

    // listen for incoming connections
    if (listen (sd, QLEN) < 0)
        errexit ("cannot listen on port %s\n", argv [PORT_POS]);

    printf("Server listening on port %s. Press Ctrl-C to stop.\n", argv[PORT_POS]);

    // Loop to accept multiple connections
    while (keep_running) {
        // accept a connection
        addrlen = sizeof (addr);
        sd2 = accept (sd, &addr, &addrlen);
        
        if (sd2 < 0) {
            // check if interrupted by signal
            if (!keep_running) {
                break;
            }
            perror("error accepting connection");
            continue; // serve next connection
        }

        // client socket pointer
        FILE *csp = fdopen(sd2, "r+");
        if (csp == NULL) {
            close(sd2);
            perror("error creating client socket pointer");
            continue; // next connection
        }

        // parse request and write to socket accordingly
        char req[BUFLEN];
        parse_req(req, csp);

        // close connection
        fclose(csp);
    }
    exit (0);
}
