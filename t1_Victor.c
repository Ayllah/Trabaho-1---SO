#include<errno.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

//using namespace std;

int main(){
  int pid, idfila, fd[2], estado, num_msg;
  struct mensagem{ long pid; int msg;};
  struct mensagem mensagem_env, mensagem_rec, *buf;
  if ((idfila = msgget(0x17011, IPC_CREAT|0x1ff)) < 0){
    printf("erro na criacao da fila\n");
    exit(1);
  }
  struct msqid_ds *qbuf;
  pid = fork();
  if (pid == 0){
    num_msg = 1;
    while (num_msg < 11) {
      msgrcv(idfila, &mensagem_rec, sizeof(mensagem_rec), 0, 0);
      printf("mensagem recebida = %ld %d\n", mensagem_rec.pid, mensagem_rec.msg);
      num_msg++;
      sleep(5);
    }
    msgctl(idfila, IPC_RMID, qbuf);
    exit(0);
  }
  mensagem_env.pid = getpid();
  num_msg = 1;
  while (num_msg < 11) {
    mensagem_env.msg = num_msg;
    msgsnd(idfila, &mensagem_env, sizeof(mensagem_env), 0);
    num_msg++;
  }
  wait (&estado);
  exit(0);
 }
