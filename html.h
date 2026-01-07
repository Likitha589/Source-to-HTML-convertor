#ifndef HTML_H
#define HTML_H

#include <stdio.h>
#include "parser.h"

void html_begin(FILE *fp);
void html_end(FILE *fp);
void source_to_html(FILE *fp,pevent_t *event);

#endif