/* This file use for NCTU OSDI course */
#include <fs.h>
#include <fat/diskio.h>
#include <fat/ff.h>
#include <kernel/drv/disk.h>
#include <kernel/timer.h>

#define DISK_ID 1

/*TODO: Lab7, low level file operator.*/

/**
  * @brief  Initial IDE disk
  * @param  pdrv: disk type
  * @retval disk error status
  *         - RES_OK: Initial success
  *         - STA_NOINIT: Intial failed
  */
DSTATUS disk_initialize (BYTE pdrv)
{
    printk("disk_initialize %d\n", pdrv);
    /* TODO */
    return 0;
}

/**
  * @brief  Get disk current status
  * @param  pdrv: disk type
  * @retval disk status
  *         - RES_OK: Normal status
  *         - STA_NODISK: ...
  */
DSTATUS disk_status (BYTE pdrv)
{
    printk("disk_status\n");
    /* TODO */
    return 0;
}

/**
  * @brief  Read serval sector form a IDE disk
  * @param  pdrv: disk type
  * @param  buff: destination memory start address
  * @param  sector: start sector number
  * @param  count: number of sector
  * @retval disk status (See diskio.h)
  *         - RES_OK: success
  *         - < 0: failed
  */
DRESULT disk_read (BYTE pdrv, BYTE* buff, DWORD sector, UINT count)
{
    int err = 0;
    int i = count;
    BYTE *ptr = buff;
    UINT cur_sector = sector;
    BYTE buf[512];
    
    printk("disk_read %d sector %d count %d\n", pdrv, sector, count);
    //printk("bufa:0x%x\n", buff[0]); 
    //printk("buf:0x%x\n", buf[510]); 
    int ide_status;
    ide_status = ide_read_sectors(DISK_ID, count, sector, (unsigned int)buff);
    //printk("bufb:0x%x\n", buf[510]); 
    //printk("ide_status:%d\n", ide_status);
    return ide_status;
    /* TODO */
}

/**
  * @brief  Write serval sector to a IDE disk
  * @param  pdrv: disk type
  * @param  buff: memory start address
  * @param  sector: destination start sector number
  * @param  count: number of sector
  * @retval disk status (See diskio.h)
  *         - RES_OK: success
  *         - < 0: failed
  */
DRESULT disk_write (BYTE pdrv, const BYTE* buff, DWORD sector, UINT count)
{
    int err = 0;
    int i = count;
    BYTE *ptr = buff;
    UINT cur_sector = sector;
    
    printk("disk_write %d sector %d count %d\n", pdrv, sector, count);
    ide_write_sectors(DISK_ID, count, sector, (unsigned int)ptr);
    /* TODO */    
    return 0;
}

/**
  * @brief  Get disk information form disk
  * @param  pdrv: disk type
  * @param  cmd: disk control command (See diskio.h)
  *         - GET_SECTOR_COUNT
  *         - GET_BLOCK_SIZE (Same as sector size)
  * @param  buff: return memory space
  * @retval disk status (See diskio.h)
  *         - RES_OK: success
  *         - < 0: failed
  */
DRESULT disk_ioctl (BYTE pdrv, BYTE cmd, void* buff)
{
    uint32_t *retVal = (uint32_t *)buff;
    printk("disk_ioctl drv=%d cmd=%d\n", pdrv, cmd);
    /* TODO */
    switch(cmd) {
	case GET_SECTOR_COUNT:
	    *retVal = ide_devices[DISK_ID].Size;
	    break;

	case GET_BLOCK_SIZE:
	    *retVal = 512;
	    break;
    }
    
    return 0;    
}


/**
  * @brief  Get OS timestamp
  * @retval tick of CPU
  */
DWORD get_fattime (void)
{
    /* TODO */
    int32_t tick;
    tick = sys_get_ticks();
    return (DWORD)tick;
}
