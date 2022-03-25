#include <jvs_core.h>

pid_t jvs_thread_tid(void) {
    return syscall(SYS_gettid);
}