#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

__attribute__((constructor))
void setup(){
  setvbuf(stdout,NULL,2,0);
  setvbuf(stderr,NULL,2,0);
  setvbuf(stdin,NULL,2,0);
}

void vuln(){
    char buf[0x20];
    puts("Enter your name: ");
    gets(buf);
}

int main(int argc, char const *argv[])
{
    printf("Have this: %p\n",vuln);
    vuln();
    return 0;
}