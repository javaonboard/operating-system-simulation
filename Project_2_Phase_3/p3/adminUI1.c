#include <signal.h>
#include <sys/wait.h>
#include "simos.h"

int admin_to_sim[2];
int sim_to_admin[2];


void signalHandler(int sig_num){
  
  printf ("A admin signal is caught %d\n", sig_num);
  printf ("signal handling is in progress ...");
  //cacth_interrupt(admininter);
  //fflush(stdout);
  
}

int main (int argc, char *argv[]) {
       printf("program started ...\n");
      //fflush(stdout);
      int pid;
      char buf[1024];
      //int rounds = argv[1];
      //if(rounds<=0) rounds = 100;
      //char action[10];
      
      //hadnle pipe failure
      if(pipe (admin_to_sim)==-1 || pipe (sim_to_admin) == -1){
        printf("error eccord while creating pipe\n");
       return -1;
      }
      
      //pointer to pipe buffer
      int admin_to_sim_read = admin_to_sim[0];
      int admin_to_sim_write = admin_to_sim[1];
      int sim_to_admin_read = sim_to_admin[0];
      int sim_to_admin_write = sim_to_admin[1];
      
      pid = fork();
      printf("before fork\n");
      if (pid > 0)
        { 
              printf("in the parnet process ... \n");
              char input[10];
              //kill (pid, SIGRTMIN);
              close(admin_to_sim_read); // dont read from this pipe
              //close(sim_to_admin_write); // dont write on this pipe
              //printf("your command > ");
              //scanf("%s", input);
              process_admin_commands();
              dup2(sim_to_admin_
              
              
              input[0] = "s";
              write(admin_to_sim_write, input[0], sizeof(input[0]));
              close(admin_to_sim_write);
              sleep(5);
              char res[1024];
              wait(0);
              read(sim_to_admin_read, res, 1024);
              printf("This is a sim output %s", res);
            
        }else
        { /* the child process */
              char name[] = "simos";
              char *argument[] = {name, NULL};
              printf ("in the child process...\n");
              close(fileno(stdin)); // read from admin
              dup(admin_to_sim_read);
              close(admin_to_sim_read);
              
              //signal(SIGRTMIN, signalHandler);
              printf("middle 1 of child ...");
              
              close(fileno(stdout));
              dup(sim_to_admin_write);
              close(sim_to_admin_write);
              printf("middle 2 of child ...");
              close(admin_to_sim_write);
              close(sim_to_admin_read);
              printf("fire up the sim ... ");
              execvp("./simos.exe", argument); 
              sleep(5000);
              //kill (pid, SIGRTMIN);  
             
        }
        return (1);
      }
  

