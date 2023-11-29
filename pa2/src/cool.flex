/*
 *  The scanner definition for COOL.
 */

/*
 *  Stuff enclosed in %{ %} in the first section is copied verbatim to the
 *  output, so headers and global definitions are placed here to be visible
 * to the code in the file.  Don't remove anything that was here initially
 */
%{
#include <cool-parse.h>
#include <stringtab.h>
#include <utilities.h>

/* The compiler assumes these identifiers. */
#define yylval cool_yylval
#define yylex  cool_yylex

/* Max size of string constants */
#define MAX_STR_CONST 1025
#define YY_NO_UNPUT   /* keep g++ happy */

extern FILE *fin; /* we read from this file */

/* define YY_INPUT so we read from the FILE fin:
 * This change makes it possible to use this scanner in
 * the Cool compiler.
 */
#undef YY_INPUT
#define YY_INPUT(buf,result,max_size) \
  if ( (result = fread( (char*)buf, sizeof(char), max_size, fin)) < 0) \
    YY_FATAL_ERROR( "read() in flex scanner failed");

char string_buf[MAX_STR_CONST]; /* to assemble string constants */
char *string_buf_ptr;

extern int curr_lineno;

extern YYSTYPE cool_yylval;

/*
 *  Add Your own definitions here
 */

int comment_level = 0;
int error_exit = 0;
int str_len = 0;

%}

%option noyywrap

%x single_comment
%x nested_comment
%x string_const
%x escape

/*
 * Define names for regular expressions here.
 */

/* regular definitions */
delim		[ \f\r\t\v]
ws			{delim}+

uppercase [A-Z]
lowercase [a-z]
letter		({lowercase}|{uppercase})

digit       [0-9]


typeid  ("SELF_TYPE"|{uppercase}({letter}|{digit}|"_")*)
objectid  ("self"|{lowercase}({letter}|{digit}|"_")*)

string_start  "\""
string_end  "\""

single_comment_start  "\-\-"
single_comment_end  "\-\-"
nested_comment_start  "\(\*"
nested_comment_end  "\*\)"

%%

 /*
  * Define regular expressions for the tokens of COOL here. Make sure, you
  * handle correctly special cases, like:
  *   - Nested comments
  *   - String constants: They use C like systax and can contain escape
  *     sequences. Escape sequence \c is accepted for all characters c. Except
  *     for \n \t \b \f, the result is c.
  *   - Keywords: They are case-insensitive except for the values true and
  *     false, which must begin with a lower-case letter.
  *   - Multiple-character operators (like <-): The scanner should produce a
  *     single token for every such operator.
  *   - Line counting: You should keep the global variable curr_lineno updated
  *     with the correct line number
  */

{ws}					{ /* no action and no return */ }

(?i:class)					{ return CLASS; }
(?i:inherits)				{ return INHERITS; }
(?i:if)						  { return IF; }
(?i:then)					  { return THEN; }
(?i:else)					  { return ELSE; }
(?i:fi)						  { return FI; }
(?i:while)					{ return WHILE; }
(?i:loop)					  { return LOOP; }
(?i:pool)					  { return POOL; }
(?i:let)						{ return LET; }
(?i:in)						  { return IN; }
(?i:case)					  { return CASE; }
(?i:of)						  { return OF; }
(?i:esac)					  { return ESAC; }
(?i:new)						{ return NEW; }
(?i:isvoid)					{ return ISVOID; }
(?i:not)						{ return NOT; }
[t](?i:rue)					      { yylval.boolean = true; return BOOL_CONST; }
[f](?i:alse)					      { yylval.boolean = false; return BOOL_CONST; }


"\+"                     { return '+'; }
"\-"                     { return '-'; }
"\*"                     { return '*'; }
"\/"                     { return '/'; }
"\~"                     { return '~'; }
"<-"                    { return ASSIGN; }
"<="                    { return LE; }
"\<"                     { return '<'; }
"\="                     { return '='; }
"\@"                     { return '@'; }
"\."                     { return '.'; }
"\,"                     { return ','; }
"\;"                     { return ';'; }
"\:"                     { return ':'; }
"\("                     { return '('; }
"\)"                     { return ')'; }
"\{"                     { return '{'; }
"\}"                     { return '}'; }
"=>"					          { return DARROW; }


{single_comment_start}  { BEGIN(single_comment); }
<single_comment>\n  { curr_lineno++; BEGIN(INITIAL); }
<single_comment><<EOF>>  { BEGIN(INITIAL); }
<single_comment>.     { /* nothing */ }


{nested_comment_start} { comment_level++; BEGIN(nested_comment); }
<nested_comment>{nested_comment_start} { comment_level++; }
<nested_comment>{nested_comment_end} {
  comment_level--;

  if (comment_level == 0) {
    BEGIN(INITIAL);
  } else if (comment_level < 0) {
    cool_yylval.error_msg = "Unmatched *)";
    return ERROR;
  }
}

<nested_comment><<EOF>> { 
  if (error_exit) {
    yyterminate();
  }

  cool_yylval.error_msg = "EOF in comment";
  error_exit = 1;
  return ERROR;  
}
<nested_comment>.     { /* nothing */ }
<nested_comment>\n    { curr_lineno++; }
{nested_comment_end} {
  cool_yylval.error_msg = "Unmatched *)";
  return ERROR;
}

{string_start} {
  string_buf_ptr = string_buf;
  str_len = 0;
  BEGIN(string_const); 
}
<string_const>{string_end} { 
  cool_yylval.symbol = stringtable.add_string(string_buf, str_len);

  BEGIN(INITIAL); 
  return STR_CONST;
}
<string_const><<EOF>> { 
  if (error_exit) {
    yyterminate();
  }

  cool_yylval.error_msg = "EOF in string constant";
  error_exit = 1;
  return ERROR; 
}
<string_const>\0  {
  cool_yylval.error_msg = "String contains invalid character";
  BEGIN(escape);
  return ERROR;
}
<string_const>("\\n"|"\\t"|"\\b"|"\\f") { 
  if (str_len + 1 >= MAX_STR_CONST) {
    cool_yylval.error_msg = "String constant too long";
    BEGIN(escape); 
    return ERROR;
  }
  switch(yytext[1]) {
    case 'n':
      string_buf[str_len++] = '\n';
      break;
    case 't':
      string_buf[str_len++] = '\t';
      break;
    case 'b':
      string_buf[str_len++] = '\b';
      break;
    case 'f':
      string_buf[str_len++] = '\f';
      break;
    default:
      string_buf[str_len++] = '&';
      break;
  }
}

<string_const>"\\". {
  if (str_len + 1 >= MAX_STR_CONST) {
    cool_yylval.error_msg = "String constant too long";
    BEGIN(escape); 
    return ERROR;
  }
  string_buf[str_len++] = yytext[1]; 
}
<string_const>\\\n {
  curr_lineno++;
  if (str_len + 1 >= MAX_STR_CONST) {
    cool_yylval.error_msg = "String constant too long";
    BEGIN(escape); 
    return ERROR;
  }
  string_buf[str_len++] = '\n';
}
<string_const>\n {
  curr_lineno++;
  cool_yylval.error_msg = "Unterminated string constant";
  BEGIN(INITIAL);
  return ERROR;
}
<string_const>.     { 
  if (str_len + 1 >= MAX_STR_CONST) {
    cool_yylval.error_msg = "String constant too long";
    BEGIN(escape); 
    return ERROR;
  }
  string_buf[str_len++] = yytext[0];
}

<escape>\n  { BEGIN(INITIAL); }
<escape>"\""  { BEGIN(INITIAL); }
<escape>.   { /* nothihng */}


{typeid}        { cool_yylval.symbol = idtable.add_string(yytext, yyleng); return TYPEID; }
{objectid}      { cool_yylval.symbol = idtable.add_string(yytext, yyleng); return OBJECTID; }
{digit}+ 				{ cool_yylval.symbol = inttable.add_string(yytext, yyleng); return INT_CONST; }

\n              { curr_lineno++; }
.               { cool_yylval.error_msg = yytext; return ERROR; }

%%
