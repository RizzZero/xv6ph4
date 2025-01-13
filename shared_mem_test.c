#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define NCHILD 10
#define LOCKFILE "lockfile"

// Acquire a user-level lock using a file
void acuire_user() {
    while (open(LOCKFILE, O_CREATE | O_WRONLY) < 0);
}

// Release the user-level lock
void release_user() {
    unlink(LOCKFILE);
}

// Test opening shared memory
void test_open_shared_mem() {
    int shmid = 0; 
    void *addr = (void *)open_shared_mem(shmid); 

    if (addr == (void *)-1) {
        printf(1, "test_open_shared_mem: Failed to open shared memory\n");
        return;
    }

    printf(1, "test_open_shared_mem: Shared memory region with shmid %d attached at address %p\n", shmid, addr);
}

// Test closing shared memory
void test_close_shared_mem() {
    int shmid = 0; 
    void *addr = (void *)open_shared_mem(shmid); 

    if (addr == (void *)-1) {
        printf(1, "test_close_shared_mem: Failed to open shared memory\n");
        return;
    }

    if (close_shared_mem(addr) == -1) { 
        printf(1, "test_close_shared_mem: Failed to close shared memory\n");
        return;
    }

    printf(1, "test_close_shared_mem: Shared memory region detached successfully\n");
}

// Test incrementing a shared memory variable across child processes
void test_sharedmem_increment() {
    int shmid = 0; 
    void *addr = (void *)open_shared_mem(shmid); 

    if (addr == (void *)-1) {
        printf(1, "test_sharedmem_increment: Failed to open shared memory\n");
        return;
    }

    // Initialize shared memory variable
    *(int *)addr = 0;

    for (int i = 0; i < NCHILD; i++) {
        int pid = fork();
        if (pid < 0) {
            printf(1, "test_sharedmem_increment: Fork failed\n");
            return;
        } else if (pid == 0) {
            // Child process increments the shared memory variable
            acuire_user();
            (*(int *)addr)++;
            release_user();
            exit();
        }
    }

    // Wait for all child processes to finish
    for (int i = 0; i < NCHILD; i++) {
        wait();
    }

    printf(1, "test_sharedmem_increment: Final value in shared memory: %d\n", *(int *)addr);
}

// Main function
int main(void) {
    printf(1, "Starting shared memory tests...\n");

    test_sharedmem_increment();
    test_open_shared_mem();
    test_close_shared_mem();

    exit();
}
