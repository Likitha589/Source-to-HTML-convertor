#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "parser.h"

// C keywords
static const char *keywords[] = {
    "auto","break","case","char","const","continue","default","do","double",
    "else","enum","extern","float","for","goto","if","int","long","register",
    "return","short","signed","sizeof","static","struct","switch","typedef",
    "union","unsigned","void","volatile","while"
};

static const char *funcs[] = {
    "printf", "scanf", "gets", "puts", "malloc", "calloc", "free"
};

#define NUM_KEYWORDS (sizeof(keywords)/sizeof(keywords[0]))
#define NUM_FUNCS (sizeof(funcs)/sizeof(funcs[0]))

int is_keyword(const char *word) {
    for (int i = 0; i < NUM_KEYWORDS; i++) {
        if (strcmp(word, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

int is_func(const char *word) {
    for (int i = 0; i < NUM_FUNCS; i++) {
        if (strcmp(word, funcs[i]) == 0)
            return 1;
    }
    return 0;
}

pevent_t* get_parser_event(FILE *fp)
{
    static pevent_t event;
    static int mode=0;

    int c = fgetc(fp);

    if (c == EOF) {
        event.type = EVENT_EOF;
        strcpy(event.lexeme, "");
        return &event;
    }
    
    if(c=='#')
    {
        int i=0;
        event.lexeme[i++]='#';
        while((c=fgetc(fp))!=EOF && c!='\n')
        {
            event.lexeme[i++]=c;
        }
        event.lexeme[i]='\0';
        if(c=='\n') ungetc(c,fp);
        if(strstr(event.lexeme,"if 0")!=NULL)
        {
            mode=1;
        }
        else if(strstr(event.lexeme,"endif")!=NULL)
        {
            mode=0;
        }
        if(mode)
        event.type=EVENT_INACTIVE;
        else
        event.type=EVENT_PREPROCESSOR;
        return &event;
    }

    if(mode==1)
    {
        event.lexeme[0]=c;
        event.lexeme[1]='\0';
        event.type=EVENT_INACTIVE;
        return &event;
    }

    // Identifiers / Keywords
    if (isalpha(c) || c == '_') {
        int i = 0;
        event.lexeme[i++] = c;
        while ((c = fgetc(fp)) != EOF && (isalnum(c) || c == '_')) {
            event.lexeme[i++] = c;
        }
        event.lexeme[i] = '\0';
        if (c != EOF) ungetc(c, fp);

        if (is_keyword(event.lexeme))
            event.type = EVENT_KEYWORD;
        else if (is_func(event.lexeme))
            event.type = EVENT_FUNCTION;
        else
            event.type = EVENT_IDENTIFIER;
    }

    // Numbers
    else if (isdigit(c)) {
        int i = 0;
        event.lexeme[i++] = c;
        while ((c = fgetc(fp)) != EOF && isdigit(c)) {
            event.lexeme[i++] = c;
        }
        event.lexeme[i] = '\0';
        if (c != EOF) ungetc(c, fp);

        event.type = EVENT_NUMBER;
    }

    // Strings
    else if (c == '"') {
        int i = 0;
        event.lexeme[i++] = c;
        while ((c = fgetc(fp)) != EOF && c != '"') {
            event.lexeme[i++] = c;
        }
        if (c == '"') event.lexeme[i++] = '"';
        event.lexeme[i] = '\0';

        event.type = EVENT_STRING;
    }

    // Check for comments or divide operator
else if (c == '/') {
    int next = fgetc(fp);
    if (next == '/') {
        // Single-line comment
        int i = 0;
        strcpy(event.lexeme, "//");
        i = 2;
        while ((c = fgetc(fp)) != EOF && c != '\n') {
            event.lexeme[i++] = c;
        }
        event.lexeme[i] = '\0';
        if (c == '\n') ungetc(c, fp);

        event.type = EVENT_COMMENT;
    }
    else if (next == '*') {
        // Multi-line comment
        int i = 0;
        strcpy(event.lexeme, "/*");
        i = 2;
        while ((c = fgetc(fp)) != EOF) {
            event.lexeme[i++] = c;
            if (c == '*' ) {
                int maybeSlash = fgetc(fp);
                if (maybeSlash == '/') {
                    event.lexeme[i++] = '/';
                    break;
                } else {
                    if (maybeSlash != EOF) ungetc(maybeSlash, fp);
                }
            }
        }
        event.lexeme[i] = '\0';
        event.type = EVENT_COMMENT;
    }
    else {
        // Just a divide operator
        if (next != EOF) ungetc(next, fp);
        event.lexeme[0] = '/';
        event.lexeme[1] = '\0';
        event.type = EVENT_OTHER;
    }
}

    // Other single characters
    else {
        event.lexeme[0] = c;
        event.lexeme[1] = '\0';
        event.type = EVENT_OTHER;
    }

    return &event;
}
