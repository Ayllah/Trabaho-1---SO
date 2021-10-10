#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<string.h>
#include <sys/wait.h>

int main(){
   int pid, idfila, fd[2], estado;
   struct mensagem{
      long pid; 
      int id;
      char msg[30];
   };
 
   struct mensagem mensagem_env, mensagem_rec; 

   /* cria */
   if ((idfila = msgget(0x2347, IPC_CREAT|0x1ff)) < 0){
     printf("erro na criacao da fila\n");
     return 1;
   }

   pid = fork();

   if (pid == 0){ //processo pai
      for(int i=0; i<10; i++){
         mensagem_env.pid = getpid();
         mensagem_env.id = i+1;
         strcpy(mensagem_env.msg, "teste de mensagem");
         msgsnd(idfila, &mensagem_env, sizeof(mensagem_env)-sizeof(long), 0);
      }
      if( msgctl(idfila, IPC_RMID, NULL ) != 0 ){
         printf("Erro ao deletar fila de mensagens\n");
         return 1;
      }
      return 0;
   }

   for(int i=0; i<10; i++){
      msgrcv(idfila, &mensagem_rec, sizeof(mensagem_rec)-sizeof(long), 0, 0);
      printf("mensagem %d recebida\n", mensagem_rec.id);
      sleep(5);
   }


   wait(&estado);
   
   return 0;
}
     
