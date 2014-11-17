#ifndef PROFILER_H
#define PROFILER_H

void push_chuck_stack(int);
int top_chuck_stack();
int pop_chuck_stack();
int empty_check_stack();

void push_scatter_time(int, double);
void push_upload_time(int, double);
void push_exec_time(int, double);
void push_download_time(int, double);
void push_gather_time(int, double);

#endif