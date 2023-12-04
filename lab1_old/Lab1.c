#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

int main()
{
    int pipe1[2];
    int err1 = pipe(pipe1);

    int pipe2[2];
    int err2 = pipe(pipe2);

    int pipe_child[2];
    int err3 = pipe(pipe_child);

    if (err1 == -1 || err2 == -1 || err3 == -1)
    {
        perror("pipe");

        return -1;
    }

    int n = 3;
    
    printf("Enter %d symbols\n", (n - 1));
    
    char str[n];
    fgets(str, n, stdin);

    pid_t id_parent = getpid(); 

    pid_t pid1 = Create_process();
    
    pid_t id1;
    if (pid1 == 0)
        id1 = getpid();

    pid_t pid2;
    if (getpid() == id_parent)
        pid2 = Create_process();

    pid_t id_current = getpid();

    if (id_current == id1 || id_current == id_parent)
    {
        if (pid1 == 0)    
        {    
            printf("Hello! I'm a Child1 process. I need to translate strings to uppercase\n");
                
            close(pipe1[1]);
            close(pipe_child[0]);

            read(pipe1[0], str, sizeof(str));
            close(pipe1[0]);
                
            for (int i = 0; i < strlen(str); ++i)
                str[i] = tolower(str[i]);
                            
            write(pipe_child[1], str, sizeof(str));
            close(pipe_child[1]);
        }    
        else
        {
            printf("Hello! I'm a Parent process. I need to create two child processes and print result\n");        
        
            close(pipe1[0]); 

            write(pipe1[1], str, sizeof(str));
            close(pipe1[1]);
        }
    }

    if (id_current != id1)
    {
        if (pid2 == 0)
        {        
            printf("Hello! I'm a Child2 process. I need to replace spaces with a character: _\n");
            
            close(pipe2[0]);
            close(pipe_child[1]);

            read(pipe_child[0], str, sizeof(str));
            close(pipe_child[0]);
	                
            for (int i = 0; i < strlen(str); ++i)
                if (str[i] == ' ')
                    str[i] = '_';
                                
            write(pipe2[1], str, sizeof(str));
            close(pipe2[1]);                
        }
        else
        {
            close(pipe2[1]);
            
            read(pipe2[0], str, sizeof(str));
            close(pipe2[0]);

            printf("Result = %s\n", str);
        
            exit(-2);        
        }
    }
}

int Create_process() {    
    pid_t pid = fork();
    if (-1 == pid)
    {
        perror("fork");
        exit(-1);
    }
    return pid;
}
