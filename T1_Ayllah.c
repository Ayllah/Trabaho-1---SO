#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/wait.h>

/**
 * * Trabalho Prático 1 - de 26/08/2021 a 11/10/2021
 * 
 * * Os alunos devem confeccionar um programa com 2 processos da seguinte maneira:
 * * a) Processo 1: cria o processo 2 e envia 10 mensagens por fila de mensagens numeradas (1 a 10) para o processo 2. 
 * *    Quando o processo 2 morrer, remove a fila de mensagem e termina a execução.
 * 
 * * b) Processo 2: executa um loop para receber 10 mensagens, onde 
 * *    (i) recebe mensagem,  
 * *    (ii) imprime o numero da mensagem e 
 * *    (iii) dorme 5 segundos. 
 * *    Ao final do loop, termina a execução.
 * 
 * * Observação: devem ser usadas chamadas de sistema Unix para criação e término de processos e chamadas de sistema do mecanismo IPC fila de 
 * * mensagens.
 * * Observação: deve ser entregue o programa fonte em C no formato .txt
*/

int main()
{
   int pid, idfila, fd[2], estado;
   char num_str[3]; 
   struct mensagem
   {
      long pid;
      char msg[30];
   };

   struct mensagem mensagem_env, mensagem_rec; 

   /* cria fila de mensagem*/
   if ((idfila = msgget(0x1700564, IPC_CREAT|0x1B6)) < 0)         //! Erro na criação da fila
   {
      printf("erro na criacao da fila\n");
      exit(1);
   }

   //Processo 1
   //! Cria processo 2
   pid = fork();  

   //! Envia 10 mensagens por fila de mensagens numeradas (1 a 10) para o processo 2.
   for(int i = 1; i < 11; i++)
   {
      if(pid == 0)
         break;
   
      mensagem_env.pid = getpid();
      sprintf(num_str, "%d", i);
      strcpy(mensagem_env.msg, num_str);

      /* envia mensagem */
      if (msgsnd(idfila, &mensagem_env, sizeof(mensagem_env)-sizeof(long), 0) < 0)     //! Erro ao enviar mensagem para fila
      {
         printf("erro ao enviar para a fila\n");
         exit(1);
      }
   }


   //Processo 2
   if(pid == 0)   //! Executa processo 2
   {
      //! Executa um loop para receber 10 mensagens
      for(int i = 0; i < 10; i++)
      {
         //! Recebe mensagem
         if(msgrcv(idfila, &mensagem_rec, sizeof(mensagem_rec)-sizeof(long), 0, 0) < 0)      //! Erro ao receber mensagem da fila
         {
            printf("erro ao receber da fila\n");
            exit(1);
         }

         //! Imprime o numero da mensagem
         //printf("mensagem recebida = %ld %s\n", mensagem_rec.pid, mensagem_rec.msg);
         printf("numero da mensagem = %s\n", mensagem_rec.msg);

         //! Dorme 5 segundos
         sleep(5);
      }
      exit(0);    //! Termina a execução
   }
   

   //Processo 1
   wait (&estado); //! Bloqueia o processo 1 até que o processo 2 termine

   //! Remove a fila de mensagem e termina a execução
   if(msgctl(idfila, IPC_RMID, 0) < 0)                                                 //! Erro ao remover fila
   {
      printf("erro ao remover a fila\n");
      exit(1);
   }

   exit(0);     //! Termina a execução
}


