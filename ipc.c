#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/mman.h>

#include "ipc.h"


static int open_file( const char *const file_path)
{
    int fd = open( file_path, O_RDONLY, PERM);

    if ( fd == -1 )
    {
        perror( "cannot open file");

        return 1;
    }

    return fd;
}

static sem_t *get_sem( const char *const sem_name)
{
    sem_t *sem = sem_open( sem_name, O_CREAT | O_RDWR, PERM, 0);
    
    if ( sem == SEM_FAILED )
    {
        sem = sem_open( sem_name, O_RDWR, PERM, 0);

        if ( sem == SEM_FAILED )
        {
            perror( "cannot open semaphore");
        } 
    }

    return sem;
    
}

static void close_sem()
{
    sem_unlink( SEM_NAME1);
    sem_unlink( SEM_NAME2);
    sem_unlink( SEM_NAME3);
}

static void *get_shm( const ssize_t n_pages)
{
    int fd = shm_open( SHM_NAME, O_CREAT | O_RDWR, PERM);

    if ( fd == -1 )
    {
        perror( "cannot get shm");

        return NULL;
    }

    if ( ftruncate( fd, getpagesize() * n_pages) == -1 )
    {
        perror( "cannot ftruncate shm");
        
        return NULL;
    }


    void *const shared_mem = mmap( NULL, getpagesize() * n_pages, PROT, MAP_SHARED, fd, 0);

    if ( shared_mem == MAP_FAILED )
    {
        perror( "cannot mmap shm");

        return NULL;
    }
 
    return shared_mem; 
}

static int get_msg()
{
    const key_t id = ftok( MSG_NAME, 0);
    int msgid = msgget( id, IPC_CREAT | PERM);

    if ( msgid == -1 )
    {
        msgid = msgget( id, PERM);

        if ( msgid == -1 )
        {
            perror( "cannot get msg");
        }
    }

    return msgid;
}


void test_shm( const char *const file_path, const ssize_t n_pages)
{
    sem_t *const sem_p = get_sem( SEM_NAME1);
    sem_t *const sem_c = get_sem( SEM_NAME2);
    sem_t *const sem_f = get_sem( SEM_NAME3);

    if ( sem_p == SEM_FAILED || 
         sem_c == SEM_FAILED ||
         sem_f == SEM_FAILED )
    {
        return;
    }


    char *const shm_buf = (char *)get_shm( n_pages);

    if ( shm_buf == NULL )
    {
        return;
    }


    pid_t pid = fork();

    if ( pid == -1 )
    {
        perror( "cannot fork");

        return;
    }

    if ( pid == 0 )
    {
        while ( true )
        {
            sem_wait( sem_c);

            int is_finish = 0;
            sem_getvalue( sem_f, &is_finish);

            if ( is_finish )
            {
                break;
            }

            sem_post( sem_p);
        }

        sem_post( sem_p);

        return;
    }
    else
    {
        int fd = open_file( file_path);

        if ( fd == -1 )
        {
            exit( 1);
        }


        while ( true )
        {
            ssize_t n_read = read( fd, shm_buf, getpagesize() * n_pages);

            if ( n_read == -1 )
            {
                perror( "cannot read");

                exit( 1);
            }

            sem_post( sem_c);
            sem_wait( sem_p);

            if ( n_read != getpagesize() * n_pages)
            {
                munmap( shm_buf, getpagesize() * n_pages);
                sem_post( sem_f);
                sem_post( sem_c);
                sem_wait( sem_p);

                close_sem();
                shm_unlink( SHM_NAME);

                break;
            }
        }
    }
}

void test_pipe( const char *const file_path, const ssize_t n_pages)
{    
    int fds[2] = { 0 };

    if ( pipe( fds) < 0 )
    {
        perror( "cannot open pipe");

        return;
    }


    pid_t pid = fork();

    if ( pid == 0 )
    {
        close( fds[1]);


        char *const buffer = calloc( getpagesize() * n_pages, sizeof(char));

        if ( buffer == NULL )
        {
            perror( "cannot allocate");

            return;
        }


        while ( true )
        {
            ssize_t n_read = read( fds[0], buffer, getpagesize() * n_pages);

            if ( n_read == -1 )
            {
                perror( "cannot read");
                
                exit( 1);
            }

            if ( n_read != getpagesize() * n_pages)
            {
                break;
            }
        }
    }
    else 
    {
        close( fds[0]);


        int fd = open_file( file_path);

        if ( fd == -1 )
        {
            return;
        }


        char *const buffer = calloc( getpagesize() * n_pages, sizeof(char));

        if ( buffer == NULL )
        {
            perror( "cannot allocate");

            return;
        }


        while ( true )
        {
            ssize_t n_read = read( fd, buffer, getpagesize() * n_pages);

            if ( n_read == -1 )
            {
                perror( "cannot read");
                return;
            }

            write( fds[1], buffer, n_read);

            if ( n_read != getpagesize() * n_pages)
            {
                break;
            }
        }

        free ( buffer);
        close( fd);
    }
}


void test_msg( const char *const file_path, const ssize_t n_pages)
{
    int msgid = get_msg();

    sem_t *const sem_f = get_sem( SEM_NAME1);
    sem_t *const sem_c = get_sem( SEM_NAME2);
    sem_t *const sem_p = get_sem( SEM_NAME3);

    pid_t pid = fork();

    if ( pid == -1 )
    {
        perror( "cannot fork");

        return;
    }

    if ( pid == 0 )
    {
        char *const buffer = calloc( getpagesize() * n_pages + sizeof(long), sizeof(char));

        if ( buffer == NULL )
        {
            perror( "cannot allocate");

            return;
        }


        while ( true )
        {
            sem_wait( sem_c);

            if ( msgrcv( msgid, buffer, getpagesize() * n_pages, 0, 0) == -1 )
            {
                perror( "cannot msgrcv");
                free( buffer);

                return;
            }

            int is_finish = 0;
            sem_getvalue( sem_f, &is_finish);

            if ( is_finish )
            {
                break;
            }

            sem_post( sem_p);

        }

        free( buffer);
        sem_post( sem_p);
    }
    else 
    {
        int fd = open_file( file_path);

        if ( fd == -1 )
        {
            return;
        }

        char *const buffer = calloc( getpagesize() * n_pages + sizeof(long), sizeof(char));

        if ( buffer == NULL )
        {
            perror( "cannot allocate");    

            return;
        }

        *(long*)buffer = MSG_TYPE;


        while ( true )
        {
            ssize_t n_read = read( fd, buffer + sizeof(long), getpagesize() * n_pages);

            if ( n_read == -1 )
            {
                perror( "cannot read");

                break;
            }

            if ( msgsnd( msgid, buffer, n_read, 0) == -1 )
            {
                perror( "cannot msgsnd");

                break;
            }
            
            sem_post( sem_c);
            sem_wait( sem_p);

            if ( n_read != getpagesize() * n_pages )
            {
                sem_post( sem_f);
                msgsnd( msgid, buffer, 1, 0);

                sem_post( sem_c);
                sem_wait( sem_p);
                
                break;
            }
        }
 
        msgctl( msgid, IPC_RMID, NULL);
        close_sem();
        free( buffer);
        close( fd);
    }
}

