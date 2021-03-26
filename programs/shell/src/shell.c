#include "shell.h"
#include "stdio.h"
#include "stdlib.h"
#include "snakeos.h"
int main(int argc, char** argv)
{
    print("SnakeOS: SHAH RAZA        JAMSHID ALI         MUZAMAL ALI        ABDUL MOHIZ\n");
    while(1) 
    {
        print("> ");
        char buf[1024];
        snakeos_terminal_readline(buf, sizeof(buf), true);
        print("\n");
        snakeos_system_run(buf);
        print("\n");
    }
    return 0;
}
