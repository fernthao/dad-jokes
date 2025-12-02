// Server

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "list.h"
#include "random_joke.h"
#include "create.h"
#include "edit.h"
#include "delete_joke.h"

#define REQUIRED_ARGC 2
#define PORT_POS 1
#define ERROR 1
#define QLEN 1
#define PROTOCOL "tcp"
#define BUFLEN 1024

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

int main (int argc, char *argv [])
{
    struct sockaddr_in sin;
    struct sockaddr addr;
    struct protoent *protoinfo;
    unsigned int addrlen;
    int sd, sd2;
    
    if (argc != REQUIRED_ARGC)
        usage (argv [0]);

    /* determine protocol */
    if ((protoinfo = getprotobyname (PROTOCOL)) == NULL)
        errexit ("cannot find protocol information for %s", PROTOCOL);

    /* setup endpoint info */
    memset ((char *)&sin,0x0,sizeof (sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons ((unsigned short int) atoi (argv [PORT_POS]));

    /* allocate a socket */
    /* would be SOCK_DGRAM for UDP */
    sd = socket(PF_INET, SOCK_STREAM, protoinfo->p_proto);
    if (sd < 0)
        errexit("cannot create socket", NULL);

    /* bind the socket */
    if (bind (sd, (struct sockaddr *)&sin, sizeof(sin)) < 0)
        errexit ("cannot bind to port %s", argv [PORT_POS]);

    /* listen for incoming connections */
    if (listen (sd, QLEN) < 0)
        errexit ("cannot listen on port %s\n", argv [PORT_POS]);

    /* accept a connection */
    addrlen = sizeof (addr);
    sd2 = accept (sd,&addr,&addrlen);
    if (sd2 < 0)
        errexit ("error accepting connection", NULL);

    // client socket pointer
    FILE *csp = fdopen(sd2, "r+");
    if (csp == NULL) {
        close(sd2);
        errexit ("error creating client socket pointer\n", NULL);
    }

    // use stdio to write to and read from client socket pointer
    char req[BUFLEN];

    // read first line
    if (fgets(req, sizeof(req), csp) != NULL) {
        // First token: command, second and third: possibly arguments
        if (strcmp(req, "LIST\n") == 0) {
            list(csp);
        }
        else if (strcmp(req, "RANDOM\n") == 0) {
            random_joke(csp);
        }
        else {
            char* method = strtok(req, " ");
            
            if (strcmp(method, "CREATE") == 0) {
                char* title = strtok(NULL, "|");
                char* msg = strtok(NULL, "\n");
                if (create(title, msg) == 0) {
                    fputs("Your joke is added!\n", csp);
                } else {
                    fputs("Error adding joke\n", csp);
                }
            }
            
            else if (strcmp(method, "EDIT") == 0) {
                char* title = strtok(NULL, "|");
                char* msg = strtok(NULL, "\n");
                if (edit(title, msg) == 0) {
                    fputs("Your joke is edited!\n", csp);
                } else {
                    fputs("Error editing joke\n", csp);
                }
            }
            else if (strcmp(method, "DELETE") == 0) {
                char* title = strtok(NULL, "\n");
                delete_joke(title) == 0 ? fputs("Your joke is deleted!\n", csp) : fputs("Error deleting joke\n", csp);
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

    // close connection
    fclose(csp); 
    
    /* close listening socket and exit */
    close (sd);
    exit (0);
}
