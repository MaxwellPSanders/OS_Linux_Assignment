//Name:maxwell sanders 
//ID #:1001069652
//Programming assignment #1
//this program is a shell used to execute other programs on the command line
//It takes in an input and parses it using strtok and then determines whether
//the command is invalid, whether it is asking to change directory, asking
//to show the pids available, or asking just to execute a command, all of which
//it can do

// The MIT License (MIT)
// 
// Copyright (c) 2016, 2017 Trevor Bakker 
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#define WHITESPACE " \t\n"      // We want to split our command line up into tokens
                                // so we need to define what delimits our tokens.
                                // In this case  white space
                                // will separate the tokens on our command line

#define MAX_COMMAND_SIZE 255    // The maximum command-line size

#define MAX_NUM_ARGUMENTS 10     // Mav shell NOW SUPPORTS 10 ARGUMENTS WOOHOO!!!

//have a pid node to create a list of pids
struct pid_node
{
    int pid;
    struct pid_node* next;
};

/*
 *Function: creates and returns a node
 *Parameters: takes in the id of the process that was spawned
 *Returns: A node which can be added to another structure 
 *Description:This function will be used to make a node that will
 *be attached to the structure that will hold the last 10 pids
 */
struct pid_node* addnode(int id)
{
    struct pid_node* node;
    node = malloc(sizeof(struct pid_node));
    node->pid = id;
    node->next = NULL;
};

int main()
{
  //variables for the queue
  int children = 0;
  struct pid_node* head = NULL;
  struct pid_node* tail = NULL;
  
  char * cmd_str = (char*) malloc( MAX_COMMAND_SIZE );

  //this will block interrupt and stop forcing the user to type exit... how diabolical
  signal(SIGINT, SIG_IGN);
  signal(SIGTSTP, SIG_IGN);

  while( 1 )
  {
    // Print out the msh prompt
    printf ("msh> ");

    // Read the command from the commandline.  The
    // maximum command that will be read is MAX_COMMAND_SIZE
    // This while command will wait here until the user
    // inputs something since fgets returns NULL when there
    // is no input
    while( !fgets (cmd_str, MAX_COMMAND_SIZE, stdin) );

    /* Parse input */
    char *token[MAX_NUM_ARGUMENTS];

    int   token_count = 0;                                 
                                                           
    // Pointer to point to the token
    // parsed by strsep
    char *arg_ptr;                                         
                                                           
    //have a string to put the command place in
    char place[100];

    //I am going to separate into tokens using strtok
    //which will allow me to take as much whitespace as I want
    //as well as make the inputs more versatile
    token[0] = strtok(cmd_str, " \t\n\0");
    for(token_count = 1; token_count < MAX_NUM_ARGUMENTS; token_count ++)
    {
      token[token_count] = strtok(NULL, " \t\n");
    }

    //if the first token does not exist then move on to the next command
    if(token[0] == NULL)
    {
      continue;
    }

    //check to see if the operation is to quit
    if(strcmp(token[0],"quit") == 0 || strcmp(token[0],"exit") == 0)
    {
      struct pid_node* temp = head;
      while(head!=NULL){
	temp = head->next;
        free(head);
	head = temp;
      }
      break;
    }

    //check to see if the operation is to showpids
    //if so then show the last 10 pids
    if(strcmp(token[0],"showpid") == 0 )
    {
      struct pid_node* current = head;
      printf("Last 10 pids\n");
      printf("============\n\n");
      while(current!=NULL){
	printf("%d\n", current->pid);
	current = current->next;
      }
      printf("\n");
      continue;	
    }

    //check to see if the operation changedirectory
    if(strcmp(token[0],"cd") == 0)
    {
      int ret;
      ret = chdir(token[1]);
      if(ret == -1)
      {
        perror("Cannot change directory");
      }
      continue;	
    } 

    //fork
    pid_t pid = fork();
    
    //error happened    
    if( pid == -1)
    {
      perror("fork failed: ");
      exit( EXIT_FAILURE );
    }
    else if( pid == 0 )
    {
      char place[40];
      //execute with respect to current directory
      sprintf(place, "./%s", token[0]);
      execl(place, token[0], token[1], token[2], token[3], token[4], 
                   token[5], token[6], token[7], token[8], token[9], NULL);
      //execute with respect to the usr/local/bin
      sprintf(place, "/usr/local/bin/%s", token[0]);
      execl(place, token[0], token[1], token[2], token[3], token[4], 
                   token[5], token[6], token[7], token[8], token[9], NULL);
      //execute with respect to the usr/bin
      sprintf(place, "/usr/bin/%s", token[0]);
      execl(place, token[0], token[1], token[2], token[3], token[4], 
                   token[5], token[6], token[7], token[8], token[9], NULL);
      //execute with respect to the /bin
      sprintf(place, "/bin/%s", token[0]);
      execl(place, token[0], token[1], token[2], token[3], token[4], 
                   token[5], token[6], token[7], token[8], token[9], NULL);

      //if all the exec's fail then send out the error message
      printf("%s: Command not found.\n", token[0]);

      fflush( NULL );
      exit( EXIT_SUCCESS );
    }
    else
    {
      int status;
      //wait for the child process to execute
      waitpid(pid, &status, 0 );
      fflush( NULL );
      
      //if the queue has no contents then start it off
      if(children == 0)
      {
      	head = addnode((int)pid);
        tail = head;  
      }
      //otherwise add it to the back
      else
      {
        tail->next = addnode((int)pid);
        tail = tail->next;
      }
      children ++;
      //if you now have more than ten numbers pop off the front
      if(children > 10)
      {
        struct pid_node* temp = head->next;
        free(head);
        head = temp;
      }   
    }

    /*debug check
    int token_index  = 0;
    for( token_index = 0; token_index < token_count; token_index ++ ) 
    {
      printf("token[%d] = %s\n", token_index, token[token_index] );  
    }
    */

  }

  free(cmd_str);

  return 0;
}
