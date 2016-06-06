/* This file use for NCTU OSDI course */
/* It's contants fat file system operators */

#include <inc/stdio.h>
#include <fs.h>
#include <fat/ff.h>
#include <diskio.h>

extern struct fs_dev fat_fs;

/*TODO: Lab7, fat level file operator.*/

/* Note: 1. Get FATFS object from fs->data
*        2. Check fs->path parameter then call f_mount.
*/
int fat_mount(struct fs_dev *fs, const void* data)
{
	char *path;
	int i;
/*
	printk("fat path : %s\n", fat_fs.path);
	printk("siz of %d\n", sizeof(fat_fs.path));
	for (i = 0; i < 64; i++) {
		if (fat_fs.path[i] == '\0'){
			printk("P:%c\n", fat_fs.path[i]);
			break;
		}
		else{
			printk("P:%c\n", fat_fs.path[i]);
			*(path + i) = fat_fs.path[i];
		}
	}
*/
	int res;
	res = f_mount((fs->data), (fs->opath), 1);
	return -res;
}

/* Note: Just call f_mkfs at root path '/' */
int fat_mkfs(const char* device_name)
{
	return f_mkfs("/", 0, 0);
}

/* Note: Convert the POSIX's open flag to elmfat's flag.
*        Example: if file->flags == O_RDONLY then open_mode = FA_READ
*                 if file->flags & O_APPEND then f_seek the file to end after f_open
*        Reference: http://elm-chan.org/fsw/ff/en/open.html 
*/
int fat_open(struct fs_fd* file)
{
	unsigned char open_mode;
	uint32_t flag = file->flags;
	open_mode = 0;
	if (flag == 0) open_mode |= (/*FA_OPEN_EXISTING |*/ FA_READ);
	if (flag & O_WRONLY)  open_mode |= FA_WRITE;
	if (flag & O_RDWR) open_mode |= (FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
	if (flag & O_ACCMODE) open_mode |= 0;
	if (flag & O_CREAT) open_mode |= FA_CREATE_NEW;
	if (flag & O_EXCL) open_mode |= 0;
	if (flag & O_TRUNC || flag & O_APPEND) open_mode |= FA_CREATE_ALWAYS;

	int ret = f_open((file->data), (file->opath), open_mode);
	return -ret;
	/*switch(file->flags) {
		case O_RDONLY:
			open_mode = FA_OPEN_EXISTING | FA_READ;
		case O_WRONLY:
			open_mode = FA_OPEN_ALWAYS | FA_WRITE;
		case O_RDWR:
			open_mode = FA_OPEN_ALWAYS | FA_READ | FA_WRITE;
		case O_ACCMODE:
			open_mode = 0;
		case O_CREAT:
			open_mode = FA_CREATE_NEW;
		case O_EXCL:
			open_mode = 0;
		case O_TRUNC:
			open_mode = FA_CREATE_ALWAYS;
		case O_APPEND:
			open_mode = FA_CREATE_ALWAYS;
		case O_DIRECTORY:
			open_mode = FA_OPEN_EXISTING;
	}*/
}

int fat_close(struct fs_fd* file)
{
	int ret;
	ret = f_close(file->data);
	return -ret;
}
int fat_read(struct fs_fd* file, void* buf, size_t count)
{
	unsigned int bw;
	char t[100];
	int ret = f_read(file->data, t, count, &bw);
	strncpy(buf, t, count);
	if (ret != 0)
		return -ret;
	else
		return bw;
}
int fat_write(struct fs_fd* file, const void* buf, size_t count)
{
	unsigned int bw;
	int ret;
	ret = f_write(file->data, buf, count, &bw);
	if (ret != 0)
		return -ret;
	else
		return bw;
}
int fat_lseek(struct fs_fd* file, off_t offset)
{
	int ret;
	ret = f_lseek(file->data, offset);
	return -ret;
}
int fat_unlink(/*struct fs_fd* file,*/ const char *pathname)
{
	int ret;
        ret = f_unlink(pathname);
	return -ret;
}

struct fs_ops elmfat_ops = {
    .dev_name = "elmfat",
    .mount = fat_mount,
    .mkfs = fat_mkfs,
    .open = fat_open,
    .close = fat_close,
    .read = fat_read,
    .write = fat_write,
    .lseek = fat_lseek
};



