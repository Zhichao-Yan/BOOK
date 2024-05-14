/*
 * @Author: yan yzc53@icloud.com
 * @Date: 2023-10-26 19:39:45
 * @LastEditors: yan yzc53@icloud.com
 * @LastEditTime: 2023-10-26 19:39:48
 * @FilePath: /book/CSAPP/code/ch7/dll.c
 * @Description: 
 * @QQ: 1594047159@qq.com
 * Copyright (c) 2023,All Rights Reserved. 
 */
/* $begin dll */
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int x[2] = {1, 2};
int y[2] = {3, 4};
int z[2];

int main() 
{
    void *handle;
    void (*addvec)(int *, int *, int *, int);
    char *error; 

    /* Dynamically load the shared library that contains addvec() */
    handle = dlopen("./libvector.so", RTLD_LAZY);
    if (!handle) {
	fprintf(stderr, "%s\n", dlerror());
	exit(1);
    }

    /* Get a pointer to the addvec() function we just loaded */
    addvec = dlsym(handle, "addvec");
    if ((error = dlerror()) != NULL) {
	fprintf(stderr, "%s\n", error);
	exit(1);
    }

    /* Now we can call addvec() just like any other function */
    addvec(x, y, z, 2);
    printf("z = [%d %d]\n", z[0], z[1]);

    /* Unload the shared library */
    if (dlclose(handle) < 0) {
	fprintf(stderr, "%s\n", dlerror());
	exit(1);
    }
    return 0;
}
/* $end dll */