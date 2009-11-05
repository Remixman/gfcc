%{

#include <ctype.h>
#include "cxx-configfile-lexer.h"
#include "cxx-configfile-parser.h"
#include "cxx-utils.h"
#include "uniquestr.h"

char configfile_lexer_free_text = 0;

static int yywrap();

static void handle_token(const char *);

typedef
struct location_tag
{
    FILE* file;
    const char *filename;
    int line;
    int column;
} location_t;

location_t current_location = { NULL, NULL, 1, 1};

%}

%x linecomment
%x maybe_comes_option_line
%x option_line
%x option_value
%x profile_name
%x flaglist

%%

<linecomment>{

[\n] { BEGIN(INITIAL); }

[^\n]* { /* Ignore comment */ }

}


<profile_name>{

[ \t]+ { /* Ignore whitespace */ }

"]" { handle_token(yytext); return ']'; }
":" { handle_token(yytext); return ':'; }
">" { handle_token(yytext); return '>'; }

[-_+.[:alpha:]][-_+.[:alnum:]]* {
    handle_token(yytext);
    return CONFIGFILE_NAME;
}

[\n] {
    current_location.line++;
    BEGIN(INITIAL);
    return EOL; 
}

}

<flaglist>{

[ \t]+ { /* Ignore whitespace */ }
"," { handle_token(yytext); return ','; }
"}" { handle_token(yytext); BEGIN(maybe_comes_option_line); return '}'; }
"!" { handle_token(yytext); return '!'; }

[-_+.[:alpha:]][-_+.[:alnum:]]* {
    handle_token(yytext);
    return CONFIGFILE_NAME;
}

}

<maybe_comes_option_line>{

[ \t]+ { /* Ignore whitespace */ }

[-_+.[:alpha:]][-_+.[:alnum:]]* {
    handle_token(yytext);
    BEGIN(option_line);
    return CONFIGFILE_NAME;
}

[\n] { 
    current_location.line++;
    BEGIN(INITIAL);
    return EOL; 
}

}

<option_line>{

[ \t]+ { /* Ignore whitespace */ }

=[ \t]* { handle_token(yytext); BEGIN(option_value); return '='; }

}

<option_value>{ 

[^\n]* { 
    handle_token(yytext); 
    return CONFIGFILE_OPTION_VALUE; 
}

[\n] {
    current_location.line++;
    BEGIN(INITIAL);
    return EOL; 
}

<<EOF>> {
    BEGIN(INITIAL);
    return EOL;
}

}

"#" { /* Start of comment */ BEGIN(linecomment); }
"{" { handle_token(yytext); BEGIN(flaglist); return '{'; }
"[" { handle_token(yytext); BEGIN(profile_name); return '['; }

[-_+.[:alpha:]][-_+.[:alnum:]]* {
    handle_token(yytext);
    BEGIN(option_line);
    return CONFIGFILE_NAME;
}

[ \t]+ { /* Ignore whitespace at top level */ }
[\n] {
    // This is like whitespace
    current_location.line++;
}


<*>. {
    unsigned char c = *yytext;
    // Update location info
    handle_token(yytext);
    if (!isprint(c))
    {
        fprintf(stderr, "warning: ignoring invalid character '\\0x%03x' found in configuration file\n", c);
    }
    else
    {
        fprintf(stderr, "warning: ignoring invalid character '%c' found in configuration file\n", c);
    }
}


%%

static int yywrap()
{
    return 1;
}


static void handle_token(const char * text)
{
    configfilelval.str = uniquestr(text);
    // No token spans more than one line
    configfilelloc.filename = current_location.filename;
    configfilelloc.first_line = configfilelloc.last_line = current_location.line;
    configfilelloc.first_column = current_location.column;
    configfilelloc.last_column = configfilelloc.first_column + strlen(text);
    current_location.column = configfilelloc.last_column;
}

extern int configfiledebug;

int open_configuration_file_for_scan(const char* scanned_filename)
{
	FILE* file;

	file = fopen(scanned_filename, "r");

	if (file == NULL)
	{
		fprintf(stderr, "warning: cannot open configuration file '%s' (%s)", scanned_filename, strerror(errno));
        return 0;
	}

    memset(&current_location, 0, sizeof(current_location));
    current_location.file = file;
    current_location.filename = uniquestr(scanned_filename);
	current_location.line = 1;
	current_location.column = 1;

    configfile_flex_debug = 0;
    configfiledebug = 0;

    struct yy_buffer_state* scanning_buffer = yy_create_buffer(file, YY_BUF_SIZE);

	yy_switch_to_buffer(scanning_buffer);

    return 1;
}

void close_configuration_file_for_scan(void)
{
    if (current_location.file != NULL)
    {
        fclose(current_location.file);
    }
}