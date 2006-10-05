#include <stdio.h>
int main()
{
    printf("Hello Dirty World\n");
                    
    //block until user types something
    fgetc(stdin);
    return 0;
} 