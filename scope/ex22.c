// file name : ex22.c
// created at: Wed 11 Dec 2013 02:22:35 PM CST

#include <stdio.h>

#include "ex22.h"
#include "../dbg/dbg.h"

int THE_SIZE = 10000;

static int THE_AGE = 27;
// 虽然其他文件不能直接访问 static 变量THE_AGE，
// 但下面这两个函数可以访问 THE_AGE。
int get_age(){
        return THE_AGE;
}

void set_age(int age){
        THE_AGE = age;
}

double update_ratio(double new_ratio){
        static double ratio = 1.0;

        double old_ratio = ratio;
        ratio = new_ratio;

        return old_ratio;
}

void print_size(){
        log_info("I think the size is : %d.", THE_SIZE);
}
