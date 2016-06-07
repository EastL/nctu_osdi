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
	struct fs_fd* fd_file;
	extern struct fs_fd fd_table[FS_FD_MAX];
	int fd = -1;
/*
	int i;
	for (i = 0; i < FS_FD_MAX; i++) {
		if (fd_table[i].opath == file) {
			fd = i;
			fd_file = &fd_table[i];
			break;
		}
	}
*/	
	if (fd == -1) {
		fd = fd_new();
		fd_file = fd_get(fd);
		fd_file->opath = file;
	}

	extern FIL file_objs[FS_FD_MAX];
	int ret = file_open(fd_file, file, flags);
	fd_put(fd_file);

	//maintain obj size
	FIL *object;
	object = fd_file->data;
	size_t size = object->obj.objsize;
	fd_file->size = size;
	//printk("sys_fd open:%d\n", fd);
	//printk("open ret:%d\n", ret);
	//printk("flags:%d\n", flags);
	//printk("sysflags:%d\n", mode);
	
	return fd;
}

int sys_close(int fd)
{
/* TODO */
	struct fs_fd* fd_file;
	fd_file = fd_get(fd);
	
	//clear size and pos
	fd_file->size = 0;
	fd_file->pos = 0;

	int ret;
	ret = file_close(fd_file);
	fd_put(fd_file);

	//clear obj
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
	//printk("sys_fd ret:%d\n", ret);
	//printk("sys_fd obj:%x\n", fd_file->data);
	//printk("sys_fd read:%d\n", fd);
	return ret;
}
int sys_write(int fd, const void *buf, size_t len)
{
/* TODO */
	struct fs_fd* fd_file;
	fd_file = fd_get(fd);

	int ret = file_write(fd_file, buf, len);
	fd_put(fd_file);

	//maintain obj size
	FIL *object;
	object = fd_file->data;
	size_t size = object->obj.objsize;
	fd_file->size = size;

	return ret;
}

/* Note: Check the whence parameter and calcuate the new offset value before do file_seek() */
off_t sys_lseek(int fd, off_t offset, int whence)
{
/* TODO */
	struct fs_fd* fd_file;
	fd_file = fd_get(fd);

	off_t new_offset;
	if (whence == SEEK_SET) new_offset = offset;
	else if (whence == SEEK_CUR) new_offset = fd_file->pos + offset;
	else if (whence == SEEK_END) new_offset = fd_file->size + offset;

	//printk("ooooooooo%d\n", new_offset);
	int ret = file_lseek(fd_file, new_offset);
	fd_put(fd_file);
	return ret;
}

int sys_unlink(const char *pathname)
{
/* TODO */ 
	file_unlink(pathname);
}

int sys_opendir(const char *path)
{
	int fd = -1;
	struct fs_fd* fd_file;

	if (fd == -1) {
		fd = fd_new();
		fd_file = fd_get(fd);
		fd_file->opath = path;
	}

	fd_file = fd_get(fd);

	int ret = file_opendir(fd_file, path);
	fd_put(fd_file);
	return ret;
}

int sys_readdir(int fd, const void *fileinfo)
{
	struct fs_fd* fd_file;
	fd_file = fd_get(fd);
	
	int ret;
	ret = file_readdir(fd_file, fileinfo);
	return ret;
}

