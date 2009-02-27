#include <unistd.h>
#include <string.h>
#include <errno.h>
#ifndef _WIN32
  #include <sys/wait.h>
#else
  #include <windows.h>
#endif

#include "cxx-driver.h"
#include "cxx-driver-utils.h"
#include "cxx-utils.h"
#include "uniquestr.h"

typedef struct temporal_file_list_tag
{
    temporal_file_t info;
    struct temporal_file_list_tag* next;
}* temporal_file_list_t;

static temporal_file_list_t temporal_file_list = NULL;

void temporal_files_cleanup(void)
{
    temporal_file_list_t iter = temporal_file_list;

    while (iter != NULL)
    {
        if (iter->info != NULL)
        {
            if (CURRENT_CONFIGURATION(verbose))
            {
                fprintf(stderr, "Removing temporal filename '%s'\n", iter->info->name);
            }
            if (remove(iter->info->name) != 0
                    && errno != ENOENT)
            {
                fprintf(stderr, "Error while removing temporal filename: '%s'\n", strerror(errno));
            }
        }

        iter = iter->next;
    }

    temporal_file_list = NULL;
}

#ifndef _WIN32
static temporal_file_t new_temporal_file_unix(void)
{
    char template[256];
    snprintf(template, 255, "/tmp/%s_XXXXXX", compilation_process.exec_basename);
    template[255] = '\0';

    // Create the temporal file
    int file_descriptor = mkstemp(template);

    if (file_descriptor < 0) 
    {
        return NULL;
    }

    // Save the info of the new file
    temporal_file_t result = calloc(sizeof(*result), 1);
    result->name = uniquestr(template);
    // Get a FILE* descriptor
    // result->file = fdopen(file_descriptor, "w+");
    // if (result->file == NULL)
    // {
    //     running_error("error: cannot create temporary file (%s)", strerror(errno));
    // }

    // Link to the temporal_file_list
    temporal_file_list_t new_file_element = calloc(sizeof(*new_file_element), 1);
    new_file_element->info = result;
    new_file_element->next = temporal_file_list;
    temporal_file_list = new_file_element;

    return result;
}
#else
static temporal_file_t new_temporal_file_win32(void)
{
    char *template = NULL;

    template = _tempnam(NULL, compilation_process.exec_basename);
    if (template == NULL)
        return NULL;

    // Save the info of the new file
    temporal_file_t result = calloc(sizeof(*result), 1);
    result->name = strappend(uniquestr(template), ".tmp");
    // // Get a FILE* descriptor
    // result->file = fopen(result->name, "w+");
    // if (result->file == NULL)
    // {
    //     running_error("error: cannot create temporary file (%s)", strerror(errno));
    // }

    // Link to the temporal_file_list
    temporal_file_list_t new_file_element = calloc(sizeof(*new_file_element), 1);
    new_file_element->info = result;
    new_file_element->next = temporal_file_list;
    temporal_file_list = new_file_element;

    return result;
}
#endif

temporal_file_t new_temporal_file()
{
#ifndef _WIN32
    return new_temporal_file_unix();
#else
    return new_temporal_file_win32();
#endif
}

const char* get_extension_filename(const char* filename)
{
    return strrchr(filename, '.');
}

int execute_program(const char* program_name, const char** arguments)
{
    return execute_program_flags(program_name, arguments, /* stdout_f */ NULL, /* stderr_f */ NULL);
}

#ifndef _WIN32
static int execute_program_flags_unix(const char* program_name, const char** arguments, const char* stdout_f, const char* stderr_f)
{
    int num = count_null_ended_array((void**)arguments);

    const char** execvp_arguments = calloc(num + 1 + 1, sizeof(char*));

    execvp_arguments[0] = program_name;

    int i;
    for (i = 0; i < num; i++)
    {
        execvp_arguments[i+1] = arguments[i];
    }

    execvp_arguments[i+1] = NULL;

    if (CURRENT_CONFIGURATION(verbose))
    {
        int j = 0;
        while (execvp_arguments[j] != NULL)
        {
            fprintf(stderr, "%s ", execvp_arguments[j]);
            j++;
        }

        if (stdout_f != NULL)
        {
            fprintf(stderr, "1> %s ", stdout_f);
        }
        if (stderr_f != NULL)
        {
            fprintf(stderr, "2> %s ", stderr_f);
        }

        fprintf(stderr, "\n");
    }

    // This routine is UNIX-only
    pid_t spawned_process;
    spawned_process = fork();
    if (spawned_process < 0) 
    {
        running_error("error: could not fork to execute subprocess '%s' (%s)", program_name, strerror(errno));
    }
    else if (spawned_process == 0) // I'm the spawned process
    {
        // Redirect output files as needed
        if (stdout_f != NULL)
        {
            FILE *new_stdout = fopen(stdout_f, "w");
            if (new_stdout == NULL)
            {
                running_error("error: could not redirect standard output to '%s' (%s)",
                        stdout_f,
                        strerror(errno));
            }

            int fd = fileno(new_stdout);
            close(1);
            dup(fd);
        }
        if (stderr_f != NULL)
        {
            FILE *new_stderr = fopen(stderr_f, "w");
            if (new_stderr == NULL)
            {
                running_error("error: could not redirect standard error to '%s' (%s)",
                        stderr_f,
                        strerror(errno));
            }

            int fd = fileno(new_stderr);
            close(2);
            dup(fd);
        }
        
        // The cast is here because execvp prototype does not get 
        // 'const char* const*' but 'char *const*'
        execvp(program_name, (char**)execvp_arguments);

        // Execvp should not return
        running_error("error: execution of subprocess '%s' failed (%s)", program_name, strerror(errno));
    }
    else // I'm the parent
    {
        // Wait for my son
        int status;
        wait(&status);
        if (WIFEXITED(status))
        {
            return (WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status))
        {
            fprintf(stderr, "Subprocess '%s' was ended with signal %d\n",
                    program_name, WTERMSIG(status));

            return 1;
        }
        else
        {
            internal_error(
                    "Subprocess '%s' ended but neither by normal exit nor signal", 
                    program_name);
        }
    }
}
#else

static char* quote_string(const char *c)
{
    const char *p;
    int num_quotes = 0;
    for (p = c; *p != '\0'; p++)
    {
        if (*p == '"')
            num_quotes++;
    }

    if (num_quotes == 0)
        return strdup(c);

    char* result = calloc(sizeof(char), num_quotes + strlen(c) + 1);

    char *q = result;
    for (p = c; *p != '\0'; p++)
    {
        if (*p == '"')
        {
            *q = '\\'; q++;
            *q = '"'; q++;
        }
        else
        {
            *q = *p; q++;
        }
    }

    *q = '\0';

    return result;
}

static int execute_program_flags_win32(const char* program_name, const char** arguments, const char* stdout_f, const char* stderr_f)
{
    int num = count_null_ended_array((void**)arguments);

    const char* quoted_args_list[num + 1];
    quoted_args_list[0] = quote_string(program_name);
    int i;
    int length = 1 + strlen(quoted_args_list[0]);
    for (i = 0; i < num; i++)
    {
        quoted_args_list[i + 1] = quote_string(arguments[i]);
        length += 1 + 2 + strlen(quoted_args_list[i + 1]);
    }

    char quoted_args_str[length];

    quoted_args_str[0] = '\0';

    strcat(quoted_args_str, "\"");
    strcat(quoted_args_str, quoted_args_list[0]);
    strcat(quoted_args_str, "\"");
    for (i = 0; i < num; i++)
    {
        strcat(quoted_args_str, " "); 
        strcat(quoted_args_str, "\"");
        strcat(quoted_args_str, quoted_args_list[i + 1]);
        strcat(quoted_args_str, "\"");
    }

    if (CURRENT_CONFIGURATION(verbose))
    {
        fprintf(stderr, "%s", quoted_args_str);

        if (stdout_f != NULL)
        {
            fprintf(stderr, "1> %s ", stdout_f);
        }
        if (stderr_f != NULL)
        {
            fprintf(stderr, "2> %s ", stderr_f);
        }

        fprintf(stderr, "\n");
    }

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof (si);
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    
    // Redirect output files as needed
    if (stdout_f != NULL)
    {
        FILE *new_stdout = fopen(stdout_f, "w");
        if (new_stdout == NULL)
        {
            running_error("error: could not redirect standard output to '%s' (%s)",
                    stdout_f,
                    strerror(errno));
        }

        int fd = _fileno(new_stdout);
        si.hStdOutput = (HANDLE) _get_osfhandle(fd);
    }
    if (stderr_f != NULL)
    {
        FILE *new_stderr = fopen(stderr_f, "w");
        if (new_stderr == NULL)
        {
            running_error("error: could not redirect standard error to '%s' (%s)",
                    stderr_f,
                    strerror(errno));
        }

        int fd = _fileno(new_stderr);
        si.hStdError = (HANDLE) _get_osfhandle(fd);
    }

    ZeroMemory( &pi, sizeof(pi) );

    // Start the child process. 
    if(!CreateProcess( 
        NULL,   // Module name 
        quoted_args_str, // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        TRUE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi )           // Pointer to PROCESS_INFORMATION structure
    ) 
    {
        running_error( "CreateProcess failed (%d).\n", GetLastError() );
    }

    // Wait until child process exits.
    if (WaitForSingleObject( pi.hProcess, INFINITE ))
    {
        running_error("WaitForSingleObject failed\n", 0);
    }

    DWORD result = 0;
    GetExitCodeProcess(pi.hProcess, &result);

    return result;
}
#endif

int execute_program_flags(const char* program_name, const char** arguments, const char* stdout_f, const char* stderr_f)
{
#ifndef _WIN32
    return execute_program_flags_unix(program_name, arguments, stdout_f, stderr_f);
#else
    return execute_program_flags_win32(program_name, arguments, stdout_f, stderr_f);
#endif
}

int count_null_ended_array(void** v)
{
    int result = 0;
    if (v == NULL)
    {
        return result;
    }

    while (v[result] != NULL)
    {
        result++;
    }

    return result;
}

void timing_start(timing_t* t)
{
    memset(t, 0, sizeof(*t));
    
    gettimeofday(&(t->start), NULL);
}

void timing_end(timing_t* t)
{
    gettimeofday(&(t->end), NULL);

    double start_value = t->start.tv_sec*1e6 + t->start.tv_usec;
    double end_value = t->end.tv_sec*1e6 + t->end.tv_usec;

    double diff_value = end_value - start_value;

    t->elapsed_time = diff_value / 1e6;
}

double timing_elapsed(const timing_t* t)
{
    return (t->elapsed_time);
}

// Inspired on the GNOME's bug-buddy code
#ifndef _WIN32
void run_gdb(void)
{
    pid_t son = fork();

    if (son < 0)
    {
        fprintf(stderr, "fork failed\n");
        exit(EXIT_FAILURE);
    }
    else if (son == 0)
    {
        char dump_name[256];
        snprintf(dump_name, 255, "%s_%d.backtrace.txt", 
                give_basename(compilation_process.argv[0]),
                getppid());
        dump_name[255] = '\0';

        FILE* output_dump = fopen(dump_name, "w");

        if (output_dump != NULL)
        {
            fprintf(stderr, "Backtrace will be left at '%s'\n", dump_name);
            close(1);
            dup(fileno(output_dump));
            close(2);
            dup(fileno(output_dump));

            char pid[16];
            snprintf(pid, 15, "%d", getppid());
            pid[15] = '\0';

            char *program_path = strdup(compilation_process.argv[0]);

            char *args[] = { "--batch", 
                "--quiet",
                "-ex", "set confirm off",
                "-ex", "set backtrace limit 200",
                "-ex", "bt",
                "-ex", "thread apply all bt full",
                "-ex", "detach",
                "-ex", "q",
                program_path,
                pid,
                (void*)0,
            };

            execvp("gdb", args);

            fprintf(stderr, "execvp failed!\n");
            exit(EXIT_FAILURE);
        }
        else
        {
            fprintf(stderr, "Could not open for output dump file '%s'\n",
                    dump_name);
        }
    }
    else
    {
        int wait_result = 0;
        wait(&wait_result);
    }
}
#endif