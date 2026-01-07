/*
 THIS IS A COMMENT
 */
#include<stdio.h>
#if 1
int main()
{
        int number,count=0,cnt,rem,mul=1,result=0;
        printf("enter a number: ");
        scanf("%d",&number);
        int q=number;
        while(q!=0)
        {
                q=q/10;
                count++;
        }
        cnt=count;
        q=number;
        while(q!=0)
        {
                rem=q%10;
                while(cnt!=0)
                {
                mul=mul*rem;
                cnt--;
                }
                result=result+mul;
                cnt=count;
                q=q/10;
                mul=1;

        }
        if(result==number)
        printf("%d is an armstrong number\n",number);
        else
        printf("BRUH\n");
        return 0;
}
#endif
//STRAY CODE
#if 0
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

    int lines=0;
    int ch;
    while((ch=fgetc(sfp))!=EOF)
    {
        if(ch=='\n')
        lines++;
    }
    rewind(sfp);

    /*for(int i=1;i<=lines+1;i++)
    {
        fprintf(dfp,"%d\n",i);
    }*/

    fprintf(dfp,"<div class='code-area'>\n");

    do
    {
        event=get_parser_event(sfp);
        source_to_html(dfp,event);
    } while(event->type!=EVENT_EOF);

    html_end(dfp);

    fclose(sfp);
    fclose(dfp);

    printf("HTML file generated: %s\n", dest);
    return 0;
}
#endif