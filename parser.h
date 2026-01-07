#ifndef PARSER_H
#define PARSER_H
#include <stdio.h>
typedef enum
{
    EVENT_KEYWORD,
    EVENT_IDENTIFIER,
    EVENT_NUMBER,
    EVENT_STRING,
    EVENT_CHAR,
    EVENT_COMMENT,
    EVENT_PREPROCESSOR,
    EVENT_FUNCTION,
    EVENT_INACTIVE,
    EVENT_OTHER,
    EVENT_EOF
}event_type_t;

typedef struct
{
    event_type_t type;
    char lexeme[1024];
}pevent_t;

pevent_t* get_parser_event(FILE *fp);
int is_keyword(const char *word);

#endif
