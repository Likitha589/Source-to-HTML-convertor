#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "html.h"
#include "parser.h"

int main(int argc, char *argv[]) 
{
    FILE *sfp,*dfp;
    char dest[100];
    pevent_t *event;

    if (argc < 2) {
        printf("ERROR :\n");
        printf("Usage: %s <source_file>\n", argv[0]);
        return 1;
    }

    if((sfp=fopen(argv[1],"r"))==NULL)
    {
        printf("ERROR: File %s couldnt be opened\n",argv[1]);
        return 2;
    }

    if(argc>2)
        sprintf(dest,"%s.html",argv[2]); 
    else
    sprintf(dest,"%s.html",argv[1]);

    if((dfp=fopen(dest,"w"))==NULL)
    {
        printf("ERROR : Could not create %s output file",dest);
        return 3;
    }

    html_begin(dfp);

    fprintf(dfp,"<div class='code-area'>\n");

    do
    {
        event=get_parser_event(sfp);
        source_to_html(dfp,event);
    } while(event->type!=EVENT_EOF);

    fprintf(dfp, "</div>\n"); 
    html_end(dfp);

    fclose(sfp);
    fclose(dfp);

    printf("HTML file generated: %s\n", dest);
    return 0;
}
