#include "fat/ff.h"

/* Low level I/O API test */
void fs_test()
{
    FATFS fs;      /* File system object (volume work area) */
    FIL fil;       /* File object */
    FRESULT res;   /* API result code */
    UINT bw;       /* Bytes written */

    /* Register work area (do not care about error) */
    res = f_mount(&fs, "/", 1);
    if (res != 0)
    {
        printk("Mount failed %d, try do mkfs.\n", res);
        /* Create FAT volume with default cluster size */
        res = f_mkfs("/", 0, 0);
	//printk("mkfs res:%d\n", res);
        res = f_mount(&fs, "/", 1); //Remount
	//printk("mount res:%d\n", res);
    }

    /* Create a file as new */
    res = f_open(&fil, "hello.txt", FA_CREATE_ALWAYS | FA_WRITE);
    
    if (res != 0)
        printk("Open failed %d\n", res);

    /* Write a message */
    f_write(&fil, "Hello, World!\r\n", 15, &bw);

    /* Close the file */
    char buf[100];
    
    f_close(&fil);
    res = f_open(&fil, "hello.txt", FA_READ);
    printk("open1:%d\n", res);
    res = f_read(&fil, buf, 20, &bw);
    printk("open:%d\n", res);
    printk("%s\n", buf);

    DIR dir;
    UINT i;
    static FILINFO fno;
    char* path = "/";
    char large[4000] = {0};
    int by;

    res = f_open(&fil, "hello1.txt", FA_CREATE_ALWAYS | FA_WRITE);
    f_close(&fil);
    res = f_open(&fil, "hello2.txt", FA_CREATE_ALWAYS | FA_WRITE);
    f_close(&fil);
    res = f_open(&fil, "hello3.txt", FA_CREATE_ALWAYS | FA_WRITE);
    f_close(&fil);
    res = f_open(&fil, "hello4.txt", FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
    for (i = 0; i < 4000; i++)
    {
	large[i] = i;
    }
    int ret;
    f_write(&fil, large, 4000, &by);
    printk("write:%d ", by);
    printk("%d\n", large[513]);
    ret = f_lseek(&fil, 0);
    printk("seek:%d\n", ret);
    for (i = 0; i < 4000; i++)
    {
	large[i] = 0;
    }
    printk("%d\n", large[513]);
    ret = f_read(&fil, large, 4000, &by);
    printk("read:%d %d \n", ret, by);
    printk("%d\n", large[510]);
    printk("%d\n", large[511]);
    printk("%d\n", large[512]);
    printk("%d\n", large[513]);
    printk("%d\n", large[514]);
    for (i = 1536; i < 2048; i++)
    {
	//printk("%d ", large[i]);
	//if((large[i]&0xFF) != (i&0xFF))
	//	printk("Failed at %d, read %x but want %d\n", i, large[i], i&0xFF);
    }
    
    ret = f_unlink("ello1.txt");
    printk("unlink:%d\n", ret);
    i = 0;
    f_close(&fil);
    res = f_opendir(&dir, path);                       /* Open the directory */
    if (res == FR_OK) {
        for (;;) {
            res = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fno.fattrib & AM_DIR) {                    /* It is a directory */
                //sprintf(&path[i = strlen(path)], "/%s", fno.fname);
                //res = scan_files(path);                    /* Enter the directory */
                //if (res != FR_OK) break;
                //path[i] = 0;
                printk("hiXD\n");
            } else {                                       /* It is a file. */
                //printk("%s/%s\n", path, fno.fname);
            }
        }
        f_closedir(&dir);
    }
    ret = f_mkdir("aaa");
    printk("mkdir:%d\n", ret);

    ret = f_chdir("/aaa");
    printk("chdir:%d\n", ret);

    ret = f_mkdir("aaa");
    printk("mkdir:%d\n", ret);

    /* Unregister work area */
    //f_mount(0, "", 0);
}
