/*-*- Mode: C; c-basic-offset: 8; indent-tabs-mode: nil -*-*/

#pragma once

/***
  This file is part of systemd.

  Copyright 2010 Lennart Poettering

  systemd is free software; you can redistribute it and/or modify it
  under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation; either version 2.1 of the License, or
  (at your option) any later version.

  systemd is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with systemd; If not, see <http://www.gnu.org/licenses/>.
***/

/* Missing glibc definitions to access certain kernel APIs */

#include <sys/resource.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/oom.h>

#include "macro.h"
#include "config.h"

#ifdef ARCH_MIPS
#include <asm/sgidefs.h>
#endif

#ifndef RLIMIT_RTTIME
#define RLIMIT_RTTIME 15
#endif

#ifndef BTRFS_IOCTL_MAGIC
#define BTRFS_IOCTL_MAGIC 0x94
#endif

#ifndef BTRFS_PATH_NAME_MAX
#define BTRFS_PATH_NAME_MAX 4087
#endif

#ifndef BTRFS_DEVICE_PATH_NAME_MAX
#define BTRFS_DEVICE_PATH_NAME_MAX 1024
#endif

#ifndef BTRFS_FSID_SIZE
#define BTRFS_FSID_SIZE 16
#endif

#ifndef BTRFS_UUID_SIZE
#define BTRFS_UUID_SIZE 16
#endif

#ifndef HAVE_LINUX_BTRFS_H
struct btrfs_ioctl_vol_args {
        int64_t fd;
        char name[BTRFS_PATH_NAME_MAX + 1];
};

struct btrfs_ioctl_dev_info_args {
        uint64_t devid;                         /* in/out */
        uint8_t uuid[BTRFS_UUID_SIZE];          /* in/out */
        uint64_t bytes_used;                    /* out */
        uint64_t total_bytes;                   /* out */
        uint64_t unused[379];                   /* pad to 4k */
        char path[BTRFS_DEVICE_PATH_NAME_MAX];  /* out */
};

struct btrfs_ioctl_fs_info_args {
        uint64_t max_id;                        /* out */
        uint64_t num_devices;                   /* out */
        uint8_t fsid[BTRFS_FSID_SIZE];          /* out */
        uint64_t reserved[124];                 /* pad to 1k */
};
#endif

#ifndef BTRFS_IOC_DEFRAG
#define BTRFS_IOC_DEFRAG _IOW(BTRFS_IOCTL_MAGIC, 2, \
                                 struct btrfs_ioctl_vol_args)
#endif

#ifndef BTRFS_IOC_DEV_INFO
#define BTRFS_IOC_DEV_INFO _IOWR(BTRFS_IOCTL_MAGIC, 30, \
                                 struct btrfs_ioctl_dev_info_args)
#endif

#ifndef BTRFS_IOC_FS_INFO
#define BTRFS_IOC_FS_INFO _IOR(BTRFS_IOCTL_MAGIC, 31, \
                                 struct btrfs_ioctl_fs_info_args)
#endif

#ifndef BTRFS_IOC_DEVICES_READY
#define BTRFS_IOC_DEVICES_READY _IOR(BTRFS_IOCTL_MAGIC, 39, \
                                 struct btrfs_ioctl_vol_args)
#endif

#ifndef BTRFS_SUPER_MAGIC
#define BTRFS_SUPER_MAGIC 0x9123683E
#endif

#ifndef MS_MOVE
#define MS_MOVE 8192
#endif

#ifndef MS_PRIVATE
#define MS_PRIVATE  (1 << 18)
#endif

#if !HAVE_DECL_GETTID
static inline pid_t gettid(void) {
        return (pid_t) syscall(SYS_gettid);
}
#endif

#ifndef MS_PRIVATE
#define MS_PRIVATE  (1 << 18)
#endif

#ifndef MS_REC
#define MS_REC 16384
#endif

#ifndef MAX_HANDLE_SZ
#define MAX_HANDLE_SZ 128
#endif

#ifndef __NR_name_to_handle_at
#  if defined(__x86_64__)
#    define __NR_name_to_handle_at 303
#  elif defined(__i386__)
#    define __NR_name_to_handle_at 341
#  elif defined(__arm__)
#    define __NR_name_to_handle_at 370
#  elif defined(__powerpc__)
#    define __NR_name_to_handle_at 345
#  else
#    error "__NR_name_to_handle_at is not defined"
#  endif
#endif

#if !HAVE_DECL_NAME_TO_HANDLE_AT
struct file_handle {
        unsigned int handle_bytes;
        int handle_type;
        unsigned char f_handle[0];
};

static inline int name_to_handle_at(int fd, const char *name, struct file_handle *handle, int *mnt_id, int flags) {
        return syscall(__NR_name_to_handle_at, fd, name, handle, mnt_id, flags);
}
#endif
