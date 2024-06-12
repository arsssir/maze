#ifndef _MDIR_H_
#define _MDIR_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>

void delete_directory_content(const char *path);

void create_directory(const char *path);

void if_dir(const char *dir_name);

#endif