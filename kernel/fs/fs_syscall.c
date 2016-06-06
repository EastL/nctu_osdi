/* This file use for NCTU OSDI course */

// It's handel the file system APIs 
#include <inc/stdio.h>
#include <inc/syscall.h>
#include <fs.h>

/*TODO: Lab7, file I/O system call interface.*/
// Below is POSIX like I/O system call 

int sys_open(const char *file, int flags, int mode)
{
    //We dont care the mode.
/* TODO */
	int fd;
	fd = fd_new();
	struct fs_fd* fd_file;
	fd_file = fd_get(fd);

	file_open(&fd_file, &file, flags);
	return fd;
}

int sys_close(int fd)
{
/* TODO */
	struct fs_fd* fd_file;
	fd_file = fd_get(fd);
	fd_put(&fd_file);
	return file_close(fd_file);
}
int sys_read(int fd, void *buf, size_t len)
{
/* TODO */
	struct fs_fd* fd_file;
	fd_file = fd_get(fd);
	return file_read(fd_file, buf, len);
}
int sys_write(int fd, const void *buf, size_t len)
{
/* TODO */
	struct fs_fd* fd_file;
	fd_file = fd_get(fd);
	return file_write(fd_file, buf, len);
}

/* Note: Check the whence parameter and calcuate the new offset value before do file_seek() */
off_t sys_lseek(int fd, off_t offset, int whence)
{
/* TODO */
	struct fs_fd* fd_file;
	fd_file = fd_get(fd);
	file_lseek(fd_file, offset);
}

int sys_unlink(const char *pathname)
{
/* TODO */ 
	file_unlink(pathname);
}


              

