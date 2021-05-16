#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include "simos.h"



void cacth_interrupt(int sig_num){
     printf ("Admin signal recieved %d\n", sig_num);
     set_interrupt (adcmdInterrupt);
}


int main(int argc, char *argv[]) {
    int fd1[2];
    int fd2[2];
    char buf1[10];
    char buf2[100];

        
    if(pipe(fd1)||pipe(fd2)){
      perror("pipe");
      return -1;
    }
    int pid = fork();
    switch(pid){
        case -1:
            perror("fork");
            return -1;
        case 0:
            // sim
             
            signal(SIGRTMIN, cacth_interrupt);
            sleep(2);
            printf("child process...\n");
            close(fd1[1]);close(fd2[0]);
            initialize_system ();
            sleep(1);
            read(fd1[0], buf2, sizeof(buf2));
            close(fd1[0]);
            printf("entered command: %s\n ",buf2);
            one_admin_command (buf2[0]); 
            sleep(1);
            signal(SIGRTMIN, cacth_interrupt2);
            execute_process_iteratively();
            kill(getpid(), SIGRTMIN);
            close(fd1[0]);
            write(fd2[1], stdout, sizeof(stdout));
            close(fd2[1]);
            system_exit();

            
        default:
            // admin
              
              sleep(0);
              close(fd1[0]);close(fd2[1]);
              signal(SIGRTMIN, cacth_interrupt); // ctach the user input signal
              printf("command> ");
              scanf ("%s",  buf1);
              printf("****command was %s\n", buf1);
              write(fd1[1], buf1, sizeof(buf1)); 
              close(fd1[1]);
              kill(getpid(), SIGRTMIN);
              sleep(2);
              kill(getpid(), SIGRTMIN);
              read(fd2[0], buf2, sizeof(buf2));
              close(fd2[0]);
              printf("Sim-output: %s\n",buf2);
    
    }
    
    printf("END~\n");
    return 0;
}  