/* This file use for NCTU OSDI course */

// It's handel the file system APIs 
#include <inc/stdio.h>
#include <inc/syscall.h>
#include <fs.h>
#include <fat/ff.h>

/*TODO: Lab7, file I/O system call interface.*/
// Below is POSIX like I/O system call 

int sys_open(const char *file, int flags, int mode)
{
    //We dont care the mode.
/* TODO */
	int fd = -1;
	int i;
	struct fs_fd* fd_file;
	extern struct fs_fd fd_table[FS_FD_MAX];
	for (i = 0; i < FS_FD_MAX; i++) {
		if (fd_table[i].opath == file) {
			fd = i;
			fd_file = &fd_table[i];
			break;
		}
	}
	
	if (fd == -1) {
		fd = fd_new();
		fd_file = fd_get(fd);
		fd_file->opath = file;
	}

	extern FIL file_objs[FS_FD_MAX];
	int ret = file_open(fd_file, file, flags);
	fd_put(fd_file);
	//printk("sys_fd open:%d\n", fd);
	//printk("open ret:%d\n", ret);
	printk("obj pos%x\n", (fd_table[fd].data));
	printk("act obj pos%x\n", &(file_objs[fd]));
	
	return fd;
}

int sys_close(int fd)
{
/* TODO */
	struct fs_fd* fd_file;
	fd_file = fd_get(fd);
	int ret;
	ret = file_close(fd_file);
	fd_put(fd_file);
	return ret;
}
int sys_read(int fd, void *buf, size_t len)
{
/* TODO */
	struct fs_fd* fd_file;
	fd_file = fd_get(fd);
	int ret = file_read(fd_file, buf, len);
	fd_put(fd_file);
	printk("sys_fd read:%d\n", fd);
	return ret;
}
int sys_write(int fd, const void *buf, size_t len)
{
/* TODO */
	struct fs_fd* fd_file;
	fd_file = fd_get(fd);
	int ret = file_write(fd_file, buf, len);
	fd_put(fd_file);
	printk("sys_fd write:%d\n", fd);
	return ret;
}

/* Note: Check the whence parameter and calcuate the new offset value before do file_seek() */
off_t sys_lseek(int fd, off_t offset, int whence)
{
/* TODO */
	struct fs_fd* fd_file;
	fd_file = fd_get(fd);
	int ret = file_lseek(fd_file, offset);
	fd_put(fd_file);
	return ret;
}

int sys_unlink(const char *pathname)
{
/* TODO */ 
	file_unlink(pathname);
}


              

