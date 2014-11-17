#ifndef PROFILER_H
#define PROFILER_H

void init_profiler(int);

void push_chuck_stack(int);
int top_chuck_stack();
void pop_chuck_stack();
int empty_check_stack();

void push_scatter_size(int esize);
void push_upload_size(int esize);
void push_exec_size(int esize);
void push_download_size(int esize);
void push_gather_size(int esize);

void push_scatter_time(double etime);
void push_upload_time(double etime);
void push_exec_time(double etime);
void push_download_time(double etime);
void push_gather_time(double etime);

int full_scatter_time_stack();
int full_upload_time_stack();
int full_exec_time_stack();
int full_download_time_stack();
int full_gather_time_stack();

#endif