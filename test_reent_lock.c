#include "types.h"
#include "stat.h"
#include "user.h"
#include "spinlock.h"

struct reentrantlock rl;

void recursive_lock(int depth) {
    if (depth == 0) {
        return; //no more recursion
    }

    acquirereentrantlock(&rl);
    printf(1, "Depth %d: Lock taken\n", depth);
    recursive_lock(depth-1);
    releasereentrantlock(&rl);
    printf(1,"Depth %d: Lock released\n", depth);

}

int main() {
    initreentrantlock(&rl, "Test Lock");
    printf(1, "start reentrant lock test\n");
    recursive_lock(5);
    printf(1,"locking and unlocking done\n");
    exit();
}

