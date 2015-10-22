#include <stdio.h>
#include <string.h>

void dispmenu();
void disptitle(char* title);
void disp();
void dispfoot();
void lineBegin();
void lineEnd();

void disptitle(char* title)
{
    int i;
    sendtocom0("\n\n");
    for (i=0; i<10; i++) sendtocom0(" ");
    for (i=0; i<62; i++) sendtocom0("-");
    sendtocom0("\n");
    for (i=0; i<10; i++) sendtocom0(" "); sendtocom0("|");
    for (i=0; i<60; i++) sendtocom0(" "); sendtocom0("|\n");
    for (i=0; i<10; i++) sendtocom0(" "); sendtocom0("|");
    for (i=0; i<20; i++) sendtocom0(" ");
    sendtocom0(title);
    for (i=0; i<(40-strlen(title)); i++) sendtocom0(" ");
    sendtocom0("|\n");
    disp();
}

void disp(void)
{
    int i;
    
    for (i=0; i<10; i++) sendtocom0(" "); sendtocom0("|");
    for (i=0; i<60; i++) sendtocom0(" "); sendtocom0("|\n");
}

void dispspace(int num)
{
    int i;
    for (i=0; i<num; i++) sendtocom0(" ");
}

void dispfoot()
{
    int i;

    for (i=0; i<10; i++) sendtocom0(" ");
    for (i=0; i<62; i++) sendtocom0("-");
    sendtocom0("\n\n");
}

void lineBegin()
{
    int i;

    for (i=0; i<10; i++) sendtocom0(" ");
    sendtocom0("|");
}

void lineEnd(int n)
{
    int i;

    for (i=0; i<60-n; i++) sendtocom0(" ");
    sendtocom0("|\n");
}

void dispmenu()
{
    char menu[6][35]={\
            "     LH-1008 TEST MENU          \0",\
            "    [1] SELF-TEST               \0",\
            "    [2] COM TEST                \0",\
            "    [3] GPS TEST                \0",\
            "    [4] IM TEST                 \0",\
            "    [5] CAN TEST                \0"};

    int i, j;
    sendtocom0("\r\n\r\n");
    for(i=0; i<15; i++) sendtocom0(" ");
    for(i=0; i<46; i++) sendtocom0("-");
    sendtocom0("\r\n");
    for(i=0; i<15; i++) sendtocom0(" "); sendtocom0("|");
    for(i=0; i<45; i++) sendtocom0(" "); sendtocom0("|\r\n");
    for(i=0; i<15; i++) sendtocom0(" "); sendtocom0("|");
    for(i=0; i<6; i++) sendtocom0(" ");

    sendtocom0(menu[0]);
    
    for(i=0; i<7; i++) sendtocom0(" ");
    sendtocom0("|\r\n");
    for(i=0; i<15; i++) sendtocom0(" "); sendtocom0("|");
    for(i=0; i<45; i++) sendtocom0(" "); sendtocom0("|\r\n");

    for(j=1; j<=5; j++) {

        for (i=0; i<15; i++) sendtocom0(" "); sendtocom0("|");
        for (i=0; i<6; i++) sendtocom0(" ");
        
        sendtocom0(menu[j]);
        
        for (i=0; i<7; i++) sendtocom0(" ");
        sendtocom0("|\r\n");
    }

    for (i=0; i<15; i++) sendtocom0(" "); sendtocom0("|");
    for (i=0; i<45; i++) sendtocom0(" "); sendtocom0("|\r\n");
    for (i=0; i<15; i++) sendtocom0(" ");
    for (i=0; i<46; i++) sendtocom0("-"); sendtocom0("\r\n");

}
