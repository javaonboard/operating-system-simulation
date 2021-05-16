#include "simos.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//===============================================================
// The interface to interact with clients for program submission
// --------------------------
// Should change to create server socket to accept client connections
// -- Best is to use the select function to get client inputs
// Should change term.c to direct the terminal output to client terminal
//===============================================================

void program_submission ()
{ char fname[100];

  fprintf (infF, "Submission file: ");
  scanf ("%s", &fname);
  if (uiDebug) fprintf (bugF, "File name: %s has been submitted\n", fname);
  submit_process (fname);
}

//using pipe and dup to redirect the sim output to client
const char* execute_socket(){
    int fd[2];
    pipe(fd);
    dup2(fd[1], STDOUT_FILENO);
    char buffer[1024];
    memset(buffer,0,1024);
    execute_process ();
    fflush(stdout);
    read(fd[0],buffer,1024);
    close(fd);
    return buffer;
}

void socket_handling(char *argv[]){

    //create and init require attribute and variables
    printf("\nSim socket started.... \n");
    int sockfd, newsockfd, portno, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int ret;
    
    fd_set current_socket, ready_socket;
    FD_ZERO(&current_socket);
    FD_SET(sockfd, &current_socket);
    ready_socket = current_socket;
    
     //create a socket    
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) {
       error("ERROR opening socket");
       }
       
     bzero ((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     printf("port number is: %d\n", portno);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     memset(&(serv_addr.sin_zero), '\0', 8);
     
     //bind the socket to server address
     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){ 
       error("ERROR binding");
       }
     
     //start listening on given port  
     printf("please hit the enter\n");
     listen(sockfd,5);
    
     //using select metod to secure multiple client connection on resources 
     if(select(FD_SETSIZE, &ready_socket, NULL,NULL,NULL) < 0){
      perror("selet error");
      exit(EXIT_FAILURE);
    }
      printf("Server-select() is OK...\n");
    
     clilen = sizeof(cli_addr);
     
     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     
     if (newsockfd < 0){ 
       error ("ERROR accepting");
     }else{ printf ("Accept client socket %d, %d\n",
                   newsockfd, (int)cli_addr.sin_port);
     }
     bzero(buffer,256);
     printf("system is ready to get data from client...\n");
      
      
     //start comunication with client
     while(1){
     char buf[20];
     char message[] = "command> \n";
     send (newsockfd, message, strlen(message),0);
     read(newsockfd,buf,20);
     printf("client message: %s\n",buf);
     if(buf[0]=='s'){
           set_interrupt(submitInterrupt);
           char message[] = "please send your file...\n";
           send (newsockfd, message, strlen(message),0);
           write_file(newsockfd);
           char submitted_file[] = "prog1";
           submit_process (submitted_file);
           
       }
     if(buf[0]=='x'){
           char *ret = execute_socket();
           send (newsockfd, ret, strlen(ret),0);
     }  
       
       
     else if(buf[0]=='T'){
             write (newsockfd, NULL, 0);
             break;
             }
       else{
        write (newsockfd, "wrong input\n", 18);
       }
     }
     ret = write (newsockfd, "bye\n", 18);
     if (ret < 0) {error ("ERROR writing to socket\n");}
     close (newsockfd); 
     fprintf (infF, "Client exiting!!!\n");

}


pthread_t submissionThread;

//creating thread for server client communication
void start_client_submission (char *argv[])
{ 
  int ret = pthread_create (&submissionThread, NULL, socket_handling, argv);
  if (ret < 0) printf ("Client submission thread creation problem\n");
  else printf ("Client submission interface has been created successsfully\n");
}

//join and end the client thread
void end_client_submission ()
{ int ret;
  pthread_join (submissionThread, NULL);
  printf ("Client submission interface has finished %d\n", ret);
}

//recive data from client write in file.
void write_file(int sockfd){
  int n;
  FILE *fp;
  char *filename = "prog1";
  char buffer[255];
  
  fp = fopen(filename, "w");
  if(fp == NULL){
  perror("Error in creating file");
  exit(1);
  }
  
  printf("writing the file\n");
  recv(sockfd, buffer, sizeof(buffer),0);
  printf ("Here is client file content:\n %s \n", buffer);
  fprintf(fp, "%s", buffer);
  fclose(fp);
  printf("file recived successfully\n");
  return;
  
}




