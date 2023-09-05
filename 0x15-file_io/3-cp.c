#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>

#define BUFSIZE 1024

/**
 * safe_read - Reads from a file descriptor and handles errors
 * @fd: The file descriptor to read from
 * @buffer: The buffer to store the read data
 * @count: The number of bytes to read
 *
 * Return: The number of bytes read, or -1 on failure
 */
ssize_t safe_read(int fd, void *buffer, size_t count)
{
    ssize_t bytes_read;

    bytes_read = read(fd, buffer, count);
    if (bytes_read == -1)
    {
        dprintf(STDERR_FILENO, "Error: Can't read from file %d\n", fd);
        exit(98);
    }

    return (bytes_read);
}

/**
 * safe_write - Writes to a file descriptor and handles errors
 * @fd: The file descriptor to write to
 * @buffer: The buffer containing the data to write
 * @count: The number of bytes to write
 */
void safe_write(int fd, const void *buffer, size_t count)
{
    ssize_t bytes_written;

    bytes_written = write(fd, buffer, count);
    if (bytes_written == -1)
    {
        dprintf(STDERR_FILENO, "Error: Can't write to file %d\n", fd);
        exit(99);
    }
}

/**
 * close_fd - Closes a file descriptor and handles errors
 * @fd: The file descriptor to close
 */
void close_fd(int fd)
{
    int closed;

    closed = close(fd);
    if (closed == -1)
    {
        dprintf(STDERR_FILENO, "Error: Can't close fd %d\n", fd);
        exit(100);
    }
}

int main(int argc, char *argv[])
{
    int fd_from, fd_to;
    char buffer[BUFSIZE];
    ssize_t r;

    if (argc != 3)
    {
        dprintf(STDERR_FILENO, "Usage: cp file_from file_to\n");
        exit(97);
    }

    /* Open the source file for reading */
    fd_from = open(argv[1], O_RDONLY);
    if (fd_from == -1)
    {
        dprintf(STDERR_FILENO, "Error: Can't read from file %s\n", argv[1]);
        exit(98);
    }

    /* Create or truncate the destination file for writing */
    fd_to = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0664);
    if (fd_to == -1)
    {
        dprintf(STDERR_FILENO, "Error: Can't write to %s\n", argv[2]);
        exit(99);
    }

    /* Copy data from source file to destination file */
    while ((r = safe_read(fd_from, buffer, BUFSIZE)) > 0)
    {
        safe_write(fd_to, buffer, (size_t)r);
    }

    /* Close file descriptors */
    close_fd(fd_from);
    close_fd(fd_to);

    return (0);
}
