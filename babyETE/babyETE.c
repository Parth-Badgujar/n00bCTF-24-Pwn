#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>

__attribute__((constructor))
void setup(){
	setvbuf(stdout,NULL,2,0);
	setvbuf(stderr,NULL,2,0);
	setvbuf(stdin,NULL,2,0);
}

#define ABSENT_LIMIT 20

int main(){
	
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
		char reason[ABSENT_LIMIT * 10] ;
		printf("Give appropriate reason \n>> ") ;
		fgets(reason, daysAbsent * 10, stdin) ;  
		puts("\nOkay, we will allow you to sit for ETEs but maximum grade will be 8 :)") ;
	}
}

void dassi(){
	system("/bin/sh") ;
}

