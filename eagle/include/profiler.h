#ifndef PROFILER_H
#define PROFILER_H

void init_profiler(int, int);

void push_chuck_stack(int);
int top_chuck_stack();
void pop_chuck_stack();
int empty_check_stack();

void push_exec_size(int esize);
void push_exec_time(double etime);

int full_scatter_time_stack();
int full_upload_time_stack();
int full_exec_time_stack();
int full_download_time_stack();
int full_gather_time_stack();

int create_exec_time_function(int);

void push_scatter_time(int size, double stime);
void push_gather_time(int size, double stime);

void trace_exec_time();

#endif