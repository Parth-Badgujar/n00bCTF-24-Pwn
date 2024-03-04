#include <stdio.h>
#include <stdlib.h>
#include <string.h>
__attribute__((constructor))
void setup(){
	setvbuf(stdout,NULL,2,0);
	setvbuf(stderr,NULL,2,0);
	setvbuf(stdin,NULL,2,0);
}

int main(){
	char inp[64];
    printf("Everthing at once, go for it !\n>> ") ;
    gets(inp) ;
    printf("Here is your outcome >> ") ;
    printf(inp) ;
    printf("\n") ;     
    puts("Ok, now time for the grand finale, go for it !") ;
    gets(inp) ;
    return 0 ;
}