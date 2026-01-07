#include <stdio.h>
#include "html.h"

// helper to escape <, >, &,back slash(\)
void print_escaped(FILE *fp, const char *text) 
{
    for (int i = 0; text[i]; i++) 
    {
        switch (text[i]) 
        {
            case '<': fprintf(fp, "&lt;"); break;
            case '>': fprintf(fp, "&gt;"); break;
            case '&': fprintf(fp, "&amp;"); break;
            case '\\' : fprintf(fp,"&#92;"); break;
            default: fputc(text[i], fp); break;
        }
    }
}

void html_begin(FILE *fp) 
{
    fprintf(fp, "<html><head>\n");
    fprintf(fp, "<link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\">\n");
    fprintf(fp, "</head><body>\n");
    fprintf(fp, "<div class='code'>\n");
}


void html_end(FILE *fp) 
{
    fprintf(fp, "</div></body></html>");
}

void source_to_html(FILE *fp, pevent_t *event) {
    if (event->type == EVENT_EOF) {
        return; // Don't print EOF marker
    }

    // Check if lexeme contains newline(s)
    for (int i = 0; event->lexeme[i]; i++) {
        if (event->lexeme[i] == '\n') {
            // When newline found, break lexeme into parts and insert <br>
            fputs("<br>\n", fp);
        } else {
            // Apply highlighting based on type for non-newline characters
            switch(event->type) {
                case EVENT_KEYWORD:
                    fprintf(fp, "<span class=\"keyword\">");
                    print_escaped(fp, &event->lexeme[i]);
                    fprintf(fp, "</span>");
                    break;
                case EVENT_COMMENT:
                    fprintf(fp, "<span class=\"comment\">");
                    print_escaped(fp, &event->lexeme[i]);
                    fprintf(fp, "</span>");
                    break;
                case EVENT_STRING:
                    fprintf(fp, "<span class=\"string\">");
                    print_escaped(fp, &event->lexeme[i]);
                    fprintf(fp, "</span>");
                    break;
                case EVENT_NUMBER:
                    fprintf(fp, "<span class=\"number\">");
                    print_escaped(fp, &event->lexeme[i]);
                    fprintf(fp, "</span>");
                    break;
                case EVENT_PREPROCESSOR:
                    fprintf(fp, "<span class=\"preproc\">");
                    print_escaped(fp, &event->lexeme[i]);
                    fprintf(fp, "</span>");
                    break;
                case EVENT_FUNCTION:
                    fprintf(fp, "<span class=\"function\">");
                    print_escaped(fp, &event->lexeme[i]);
                    fprintf(fp, "</span>");
                    break;
                case EVENT_INACTIVE:
                    fprintf(fp, "<span class=\"inactive\">");
                    print_escaped(fp, &event->lexeme[i]);
                    fprintf(fp, "</span>");
                    break;
                default:
                    print_escaped(fp, &event->lexeme[i]);
                    break;
            }
            break; // break because we handled printing one character
        }
    }
}
