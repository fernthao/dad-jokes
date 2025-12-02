/* Name: Thao Nguyen 
 Case Network ID: ttn60
 The filename: sockets.c
 Date created: Nov 20, 2025
 Description: Implementation of the client
*/

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <getopt.h>

#define ERROR 1
#define HOST_POS 1
#define PORT_POS 2
#define PROTOCOL "tcp"
#define BUFLEN 1024
#define ARG_LIST    0x1
#define ARG_CREATE  0x2
#define ARG_GET     0x4
#define ARG_EDIT    0x8
#define ARG_DELETE  0x10
#define ARG_MESSAGE 0x20

int usage (char *progname)
{
    fprintf(stderr, "--------------- USAGE: ---------------\n");
    fprintf(stderr, "%s host port [-l] [-c] [-r] [-e joke_title] [-d] [-m joke_message]\n", progname);
    fprintf(stderr, "   -l          list all jokes\n");
    fprintf(stderr, "   -c          create a new joke\n");
    fprintf(stderr, "   -r          get random joke\n");
    fprintf(stderr, "   -e T -m M   update joke with title \'T\' with joke message \'M\'\n");
    fprintf(stderr, "   -d T        delete joke with title \'T\'\n");
    exit (ERROR);
}

int errexit (char *format, char *arg)
{
    fprintf (stderr,format,arg);
    fprintf (stderr,"\n");
    exit (ERROR);
}

unsigned short cmd_line_flags = 0;
char* title = NULL;
char* joke = NULL;

// Parse command line arguments
void parseargs(int argc, char* argv[]) {
    int opt;

    while ((opt = getopt(argc, argv, "lcre:dm:")) != -1) {
        switch (opt) {
            case 'l':
                cmd_line_flags |= ARG_LIST;
                break;
            case 'c':
                cmd_line_flags |= ARG_CREATE;
                title = optarg;
                break;
            case 'r':
                cmd_line_flags |= ARG_GET;
                break;
            case 'e':
                cmd_line_flags |= ARG_EDIT;
                title = optarg;
                break;
            case 'd':
                cmd_line_flags |= ARG_DELETE;
                title = optarg;
                break;
            case 'm':
                cmd_line_flags |= ARG_MESSAGE;
                joke = optarg;
                break;
            default:
                usage(argv[0]);
        }
    }
    
    if (cmd_line_flags == 0) {
        fprintf(stderr, "error: no command line option given\n");
        usage(argv[0]);
    }
}

int main (int argc, char *argv [])
{
    struct sockaddr_in sin; // input buffer
    struct hostent *hinfo;
    struct protoent *protoinfo;
    char buffer [BUFLEN];
    int sd;

    parseargs(argc, argv);

    /* lookup the hostname */
    hinfo = gethostbyname (argv [HOST_POS]);
    if (hinfo == NULL)
        errexit ("cannot find name: %s", argv [HOST_POS]);

    /* set endpoint information */
    memset ((char *)&sin, 0x0, sizeof (sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons (atoi (argv [PORT_POS]));
    memcpy ((char *)&sin.sin_addr,hinfo->h_addr_list[0],hinfo->h_length);

    if ((protoinfo = getprotobyname (PROTOCOL)) == NULL)
        errexit ("cannot find protocol information for %s", PROTOCOL);

    /* allocate a socket */
    /* would be SOCK_DGRAM for UDP */
    sd = socket(PF_INET, SOCK_STREAM, protoinfo->p_proto);
    if (sd < 0)
        errexit("cannot create socket",NULL);

    /* connect the socket */
    if (connect (sd, (struct sockaddr *)&sin, sizeof(sin)) < 0)
        errexit ("cannot connect", NULL);

    /* build a request string */
    char req[BUFLEN];
    if (cmd_line_flags & ARG_LIST) {
        snprintf(req, sizeof(req), "LIST\n");
    } else if ((cmd_line_flags & (ARG_CREATE | ARG_MESSAGE)) == (ARG_CREATE | ARG_MESSAGE)) {
        snprintf(req, sizeof(req), "CREATE %s|%s\n", title, joke);
    } else if (cmd_line_flags & ARG_GET) {
        snprintf(req, sizeof(req), "GET\n");
    } else if ((cmd_line_flags & (ARG_EDIT | ARG_MESSAGE)) == (ARG_EDIT | ARG_MESSAGE)) {
        snprintf(req, sizeof(req), "EDIT %s|%s\n", title, joke);
    } else if (cmd_line_flags & ARG_DELETE) {
        snprintf(req, sizeof(req), "DELETE %s\n", title);
    } else {
        req[0] = '\0';
    }

    /* use stdio on the socket */
    FILE *sp = fdopen(sd, "r+");
    if (sp == NULL)
        errexit("fdopen failed", NULL);

    /* send the request to the socket */
    if (req[0] != '\0') {
        if (fputs(req, sp) == EOF)
            errexit("error sending request", NULL);
        fflush(sp); 
    }

    /* read a response line */
    memset (buffer,0x0,BUFLEN);
    if (fgets(buffer, sizeof(buffer), sp) == NULL)
        errexit("reading error", NULL);
    fprintf(stdout, "%s", buffer);

    /* closes the socket & exit */
    fclose(sp);
    exit (0);
}
