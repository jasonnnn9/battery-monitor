#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

/* Newlib's reentrant wrappers read the global syscall error value. */
#undef errno
extern int errno;

/* This firmware has no file descriptors, console, or process services. */
static int unsupported(void)
{
    errno = ENOSYS;
    return -1;
}

int _close(int fd)
{
    (void)fd;
    return unsupported();
}

int _fstat(int fd, struct stat *status)
{
    (void)fd;
    (void)status;
    return unsupported();
}

pid_t _getpid(void)
{
    return unsupported();
}

int _isatty(int fd)
{
    (void)fd;
    errno = ENOSYS;
    return 0;
}

int _kill(pid_t pid, int signal)
{
    (void)pid;
    (void)signal;
    return unsupported();
}

_off_t _lseek(int fd, _off_t offset, int origin)
{
    (void)fd;
    (void)offset;
    (void)origin;
    return unsupported();
}

_READ_WRITE_RETURN_TYPE _read(int fd, void *buffer, size_t length)
{
    (void)fd;
    (void)buffer;
    (void)length;
    return unsupported();
}

_READ_WRITE_RETURN_TYPE _write(int fd, const void *buffer, size_t length)
{
    (void)fd;
    (void)buffer;
    (void)length;
    return unsupported();
}
