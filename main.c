#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ipc.h"
#include "opt.h"


int main( const int argc, const char *const argv[])
{
    if ( argc != 4 )
    {
        printf( "Incorrect number of arguments\n");

        return 1;
    }


    const char *const file_path = argv[1];
    const char *const opt       = argv[2];
    const size_t      n_pages   = atoi( argv[3]); 

    if ( strcmp( opt, OPT_MSG) == 0 )
    {
        test_msg( file_path, n_pages);
    }
    else if ( strcmp( opt, OPT_SHM) == 0 )
    {
        test_shm( file_path, n_pages); 
    }
    else if ( strcmp( opt, OPT_PIPE) == 0 )
    {
        test_pipe( file_path, n_pages);
    }
    else
    {
        printf( "Unknown option <%s>\n", opt);

        return 1;
    } 

    return 0; 
}
