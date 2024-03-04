#include <stdio.h>
#include <string.h>
#include <stdlib.h>

__attribute__((constructor))
void setup(){
	setvbuf(stdout,NULL,2,0);
	setvbuf(stderr,NULL,2,0);
	setvbuf(stdin,NULL,2,0);
}

#define ABSENT_LIMIT 20

int main(){
    char name[20]; 
    printf("What is your name ?\n>> ") ;
    fgets(name , 20, stdin); 
    name[strcspn(name, "\n")] = '\0' ;
    puts("You seem to like the new attendance system huh !") ;
    puts("How many days were you absent ?") ;
    int daysAbsent ;
	scanf("%d", &daysAbsent) ;
	getchar(); 
	if (daysAbsent > ABSENT_LIMIT){
		puts("Sorry, you won't be able to give ETE :(") ;
		exit(0) ;
	}
	else {
        int count = 5 ;
        while (count--){
            char reason[ABSENT_LIMIT * 10 + 1] ;
            printf("Give appropriate reason \n>> ") ;
            fgets(reason, ABSENT_LIMIT * 10, stdin) ;
            printf("Wtf ");
            puts(name) ;
            printf("Who gives reason like this : ") ;
            printf(reason); 
            printf("\nThis isn't accepted !") ;
        }
	}
    puts("\nOkay, we will allow you to sit for ETEs but maximum grade will be 8 :)") ;
    return 0 ;
}

