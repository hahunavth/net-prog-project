#ifndef __FILE_UTILS_H__
#define __FILE_UTILS_H__

/*
 * @brief Thao tac tung dong trong file.
 *
 * Created on Wed Nov 16 2022
 * Copyright (c) 2022 Vu Thanh Ha
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <unistd.h>

/**
 * NOTE: Iim kiem tu vi tri SEEK_CUR
 * - Neu tim toan bo can quay ve dau file truoc!
 *    + fseek(file, 0, SEEK_SET);
 */
extern int f_replace(FILE *f, const char *match, const char *replace);
extern int f_delete(FILE *f, const char *match);
extern int f_add(FILE *f, const char *item);

#endif