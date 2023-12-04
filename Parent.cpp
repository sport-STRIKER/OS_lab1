#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <cerrno>
#include <cstring>
#include  <stdio.h>
using namespace std;

int main() 
{
    cout << "Enter" << endl;
    
    string str;

    getline(cin, str);

    int pipe1[2];
    int err1 = pipe(pipe1);

    int pipe2[2];
    int err2 = pipe(pipe2);

    if (err1 == -1 || err2 == -1)
    {
        perror("pipe");

        return -1;
    }

    pid_t pid1 = fork();
    if (pid1 == -1) 
    {
        perror("fork");
        
        return 1;
    }


    if (pid1 == 0) 
    {
        cout << "Hello! I'm a Child1 process. I need to translate strings to uppercase" << endl;
       
        close(pipe1[0]);
        
        if (dup2(pipe1[1], STDOUT_FILENO) == -1) 
        {
            perror("dup2");
            
            return 1;
        }

        if (execl("./Child1", "./Child1", str.c_str(), nullptr) == -1) 
        {
            perror("execl");
            
            return 1;
        }
    }

    close(pipe1[1]);

    pid_t pid2 = fork();
    if (pid2 == -1) 
    {
        perror("fork");
        
        return 1;
    }


    if (dup2(pipe1[0], STDIN_FILENO) == -1) 
    {
        perror("Dup2 error");
            
        return 1;
    }

    if (pid2 == 0) 
    {
        cout << "Hello! I'm a Child2 process. I need to replace spaces with a character: _" << endl;
        
        close(pipe2[0]);
        
        getline(cin, str);
        
        close(pipe1[0]);
        
        if (dup2(pipe2[1], STDOUT_FILENO) == -1) 
        {
            perror("dup2");
            
            return 1;
        }
        
        if (execl("./Child2", "./Child2", str.c_str(), nullptr) == -1) 
        {
            perror("execl");
            
            return 1;
        }
    }

    close(pipe1[0]);
    close(pipe2[1]);

    if (pid1 > 0 and pid2 > 0) 
    {
        cout << "Hello! I'm a Parent process. I need to create two child processes and print result" << endl;
        
        if (dup2(pipe2[0], STDIN_FILENO) == -1) 
        {
            perror("Dup2 error");
            
            return 1;
        }
        
        getline(cin, str);
        
        cout <<  str << endl;
        
        wait(nullptr);
        wait(nullptr);
    }

    return 0;
}
