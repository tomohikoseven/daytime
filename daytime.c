#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include"daytime_port.h"

#define BUF_SIZE    1024

/* prototype */
static int
open_connection
(
    char *host,
    char *service
);
static FILE*
_fdopen
(
    int sock,
    const char *mode
);
static void
setHints
(
    struct addrinfo *hints
);
static void
getAddrinfo
(
    char            *host,
    char            *service,
    struct addrinfo *hints,
    struct addrinfo **res
);
static int
_connect
(
    struct addrinfo *res
);
static int
open_connection
(
    char    *host,
    char    *service
);

static FILE*
_fdopen
(
    int sock,
    const char *mode
)
{
    FILE *f = NULL;

    f = fdopen(sock, mode);
    if(!f)
    {
        perror("fdopen(3)");
        exit(1);
    }
    return f;
}

static void
setHints
(
    struct addrinfo *hints
)
{
    hints->ai_family = AF_UNSPEC;        /* Allow IPv4 or IPv6 */
    hints->ai_socktype = SOCK_STREAM;
}

static void
getAddrinfo
(
    char            *host,
    char            *service,
    struct addrinfo *hints,
    struct addrinfo **res
)
{
    int err = 0;

    err = getaddrinfo( host, service, hints, res );
    if( err != 0 )
    {
        fprintf(stderr, "getaddrinfo(3): %s\n", gai_strerror(err));
        exit(1);
    }
}

static int
_connect
(
    struct addrinfo *res
)
{
    int             sock    = 0;
    struct addrinfo *ai     = NULL;
    int             result  = 0;

    for( ai = res; ai; ai = ai->ai_next )
    {
        sock = socket( ai->ai_family, ai->ai_socktype, ai->ai_protocol );
        if( sock < 0 )
        {
            continue;
        }

        result = connect(sock, ai->ai_addr, ai->ai_addrlen);
        if( result < 0 )
        {
            close(sock);
            continue;
        }

        goto SUCCESS;
    }

    /* error */
    freeaddrinfo(res);
    fprintf(stderr, "socket(2)/connect(2) failed");
    exit(1);

SUCCESS:
    /* success */
    freeaddrinfo( res );
    return sock;
}

static int
open_connection
(
    char    *host,
    char    *service
)
{
    int     sock            = 0;
    struct addrinfo hints;
    struct addrinfo *res    = NULL;

    memset( &hints, 0, sizeof( struct addrinfo ) );

    setHints( &hints );

    getAddrinfo( host, service, &hints, &res );

    /* free res in _connect() */
    sock = _connect( res );

    return sock;
}

int
main
(
    int     argc,
    char    *argv[]
)
{
    int     sock;
    FILE    *f;
    char    buf[BUF_SIZE];
    char    port[PORT_SIZE];

    memset( buf, 0x00, sizeof(buf) );
    memset( port,0x00, sizeof(port));

    /* set port */
    sprintf( port, "%d", DEFAULT_PORT );

    //sock = open_connection( (argc>1 ? argv[1] : "localhost"), "daytime" );
    sock = open_connection( (argc>1 ? argv[1] : "localhost"), port );

    f = _fdopen( sock, "r" );

    fgets(buf, sizeof(buf), f);

    fclose(f);

    fputs(buf,stdout);

    return 0;
}
