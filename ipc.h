#ifndef IPC_H
#define IPC_H


#include <unistd.h>
#include <semaphore.h>


#define SEM_NAME1 "SEM1"
#define SEM_NAME2 "SEM2"
#define SEM_NAME3 "SEM3"

#define SHM_NAME "SHARED"

#define MSG_NAME "MESSAGE"
#define MSG_TYPE  228
#define MSG_FINAL 100

#define PERM 0644 
#define PROT PROT_READ | PROT_WRITE


void test_shm ( const char *const file_path, const ssize_t n_pages);
void test_pipe( const char *const file_path, const ssize_t n_pages);
void test_msg ( const char *const file_path, const ssize_t n_pages);


#endif // IPC_H
