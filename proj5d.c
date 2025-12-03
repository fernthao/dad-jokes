// Server

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
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
#define ERROR 1
#define QLEN 1
#define PROTOCOL "tcp"
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

void parse_req(char* req, FILE* csp) {
    // request is on 1 single line
    if (fgets(req, BUFLEN, csp) != NULL) {
        // methods not requiring arguments
        if (strcmp(req, "LIST\n") == 0) {
            list(csp);
        }
        else if (strcmp(req, "RANDOM\n") == 0) {
            random_joke(csp);
        }
        else if (strcmp(req, "DADSAY\n") == 0) {
            dadsay(csp);
        }

        // methods requiring arguments (separated with a space)
        else {
            char* method = strtok(req, " ");
            
            if (strcmp(method, "CREATE") == 0) {
                char* title = strtok(NULL, "|");
                char* msg = strtok(NULL, "\n");
                create(title, msg, csp);
            }
            
            else if (strcmp(method, "EDIT") == 0) {
                char* title = strtok(NULL, "|");
                char* msg = strtok(NULL, "\n");
                edit(title, msg, csp);
            }
            else if (strcmp(method, "DELETE") == 0) {
                char* title = strtok(NULL, "\n");
                delete_joke(title, csp);
            }
            else {
                fputs("Error: request not recognized\n", csp);
                fputs("Accepted requests: LIST, RANDOM, CREATE, EDIT, DELETE\n", csp);
            }
        }
        fflush(csp);
    } else {
        fprintf(stderr, "no request or read error\n");
    }
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

        // use stdio to write to and read from client socket pointer
        char req[BUFLEN];

        // parse request and write to socket accordingly
        parse_req(req, csp);

        // close connection
        fclose(csp);
    }
    exit (0);
}
