/*
 * operations on IDE disk.
 */

#include "serv.h"
#include <drivers/dev_disk.h>
#include <lib.h>
#include <mmu.h>

// Overview:
//  read data from IDE disk. First issue a read request through
//  disk register and then copy data from disk buffer
//  (512 bytes, a sector) to destination array.
//
// Parameters:
//  diskno: disk number.
//  secno: start sector number.
//  dst: destination for data read from IDE disk.
//  nsecs: the number of sectors to read.
//
// Post-Condition:
//  Panic if any error occurs. (you may want to use 'panic_on')
//
// Hint: Use syscalls to access device registers and buffers.
// Hint: Use the physical address and offsets defined in
// 'include/drivers/dev_disk.h':
//  'DEV_DISK_ADDRESS', 'DEV_DISK_ID', 'DEV_DISK_OFFSET',
//  'DEV_DISK_OPERATION_READ', 'DEV_DISK_START_OPERATION', 'DEV_DISK_STATUS',
//  'DEV_DISK_BUFFER'
void ide_read(u_int diskno, u_int secno, void *dst, u_int nsecs) {
  u_int begin = secno * BY2SECT;
  u_int end = begin + nsecs * BY2SECT;

  for (u_int off = 0; begin + off < end; off += BY2SECT) {
    uint32_t temp = diskno;
    /* Exercise 5.3: Your code here. (1/2) */
    uint32_t read = DEV_DISK_OPERATION_READ;
    uint32_t status = 0;
    if (syscall_write_dev(&temp, DEV_DISK_ADDRESS + DEV_DISK_ID,
                          sizeof(temp)) ==
        -E_INVAL) { //首先往id位置写入这次要使用的磁盘id号
      panic_on("error file disk id in read\n");
    }
    uint32_t curoff=begin+off;
    if (syscall_write_dev(&curoff, DEV_DISK_ADDRESS + DEV_DISK_OFFSET,
                          sizeof(off)) ==
        -E_INVAL) { //向偏移处写入在距离磁盘起始处off的位置开始操作
      panic_on("error file disk offset in read\n");
    }
    if (syscall_write_dev(&read, DEV_DISK_ADDRESS + DEV_DISK_START_OPERATION,
                          sizeof(int)) ==
        -E_INVAL) { //向操作开始位置写入0表示读
      panic_on("error file disk set read\n");
    }
    if (syscall_read_dev(&status, DEV_DISK_ADDRESS + DEV_DISK_STATUS,
                         sizeof(int)) == -E_INVAL) { //读取返回值
      panic_on("errro file disk read status\n");
    }
    if (status == 0) {
      panic_on("error file disk read return value\n");
    }
    void *curdst= dst + off;
    if (syscall_read_dev(curdst, DEV_DISK_ADDRESS + DEV_DISK_BUFFER,
                         DEV_DISK_BUFFER_LEN) ==
        -E_INVAL) { //从缓冲区读出要读的数据
      panic_on("error file disk read data from buffer\n");
    }
  }
}

// Overview:
//  write data to IDE disk.
//
// Parameters:
//  diskno: disk number.
//  secno: start sector number.
//  src: the source data to write into IDE disk.
//  nsecs: the number of sectors to write.
//
// Post-Condition:
//  Panic if any error occurs.
//
// Hint: Use syscalls to access device registers and buffers.
// Hint: Use the physical address and offsets defined in
// 'include/drivers/dev_disk.h':
//  'DEV_DISK_ADDRESS', 'DEV_DISK_ID', 'DEV_DISK_OFFSET', 'DEV_DISK_BUFFER',
//  'DEV_DISK_OPERATION_WRITE', 'DEV_DISK_START_OPERATION', 'DEV_DISK_STATUS'
void ide_write(u_int diskno, u_int secno, void *src, u_int nsecs) {
  u_int begin = secno * BY2SECT;
  u_int end = begin + nsecs * BY2SECT;

  for (u_int off = 0; begin + off < end; off += BY2SECT) {
    uint32_t temp = diskno;
    /* Exercise 5.3: Your code here. (2/2) */
    uint32_t write = DEV_DISK_OPERATION_WRITE;
    uint32_t status = 0;
    if (syscall_write_dev(&temp, DEV_DISK_ADDRESS + DEV_DISK_ID,
                          sizeof(temp)) ==
        -E_INVAL) { //首先往id位置写入这次要使用的磁盘id号
      panic_on("error file disk id in write\n");
    }
    uint32_t curoff=begin+off;
    if (syscall_write_dev(&curoff, DEV_DISK_ADDRESS + DEV_DISK_OFFSET,
                          sizeof(off)) ==
        -E_INVAL) { //向偏移处写入在距离磁盘起始处off的位置开始操作，offset处写的是绝对偏移
      panic_on("error file disk offset in write\n");
    }
    void * cursrc=src + off;
    if (syscall_write_dev(cursrc, DEV_DISK_ADDRESS + DEV_DISK_BUFFER,
                          DEV_DISK_BUFFER_LEN) ==
        -E_INVAL) { //向缓冲区写要写的数据
      panic_on("error file disk write data to buffer\n");
    }
    if (syscall_write_dev(&write, DEV_DISK_ADDRESS + DEV_DISK_START_OPERATION,
                          sizeof(int)) ==
        -E_INVAL) { //向操作开始位置写入1表示写
      panic_on("error file disk set write\n");
    }
    if (syscall_read_dev(&status, DEV_DISK_ADDRESS + DEV_DISK_STATUS,
                         sizeof(int)) == -E_INVAL) { //读取返回值
      panic_on("errro file disk write status\n");
    }
    if (status == 0) {
      panic_on("error file disk write return value\n");
    }
  }
}
