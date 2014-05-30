#include <stdio.h>
#include <stdlib.h>

//#include "ListaSequencial.h"
//include "ListaSequencialOrdenada.h"

#include "linkedlist.h"

int main()
{
  int opcao,r,searchIndex;
  User newUser;
  User *foundUser;
  
  Node *nodeList;
  
  // Criar uma nova lista
  
  nodeList = newList();
  
  do
  {
    // Exibir menu principal na tela
    
    //system("clear");
    printf("\nUSER LIST TEST:\n");
    printf("\n");
    printf("  [1] ADD USER\n");
    printf("  [2] LIST USERS\n");
    printf("  [3] GET USER SOCKET\n");
    printf("  [4] REMOVE USER\n");
    printf("  [5] ENCERRAR O PROGRAMA\n");
    printf("\n");
    printf("  CHOOSE: ");
    scanf("%d",&opcao);
    
    // Add user
    
    if (opcao == 1)
    {
      system("clear");
      printf("  ADD USER:\n");
      printf("\n");
      printf("SOCKET No: "); scanf("%d",&(newUser.sock));
      printf("NAME:      "); scanf("%s",newUser.name);

      r = insert(&nodeList, &newUser);
      
      if (r==INSERTION_SUCCESS) printf("\nSuccess!\n");
      if (r==INSUFFICIENT_MEMORY)  printf("\nErro 1: Memory full!\n");
      if (r==DUPLICATE_SOCKET)   printf("\nErro 2: Socket full!\n");

      //getchar();

    }
    
    // List all nodes
    
    if (opcao == 2)
    {
      system("clear");
      listAllS(nodeList);
      //getchar

    }

    // Get user information from index. 

    if (opcao == 3)
    {

      system("clear");
      printf("  GET USER SOCKET:\n");
      printf("\n");
      printf("INDEX: "); scanf("%d",&searchIndex);

      foundUser = getUser(nodeList,searchIndex);
      
      if (foundUser == NULL)
      {
        printf("\nERROR: User not found.\n");
      }
      else
      {
        printf("%10d %20s\n",
             (*foundUser).sock,
             (*foundUser).name);
      }
      //getchar();

    }

    // OPCAO 4 - EXCLUIR UM ALUNO DA TURMA

    if (opcao == 4)
    {

      system("clear");
      printf("  REMOVE USER:\n");
      printf("\n");
      printf("INDEX: "); scanf("%d",&searchIndex);

      r = delete(&nodeList,searchIndex);
      
      if (r==FORBIDDEN_PROCEDURE) printf("\nproibido!\n");
      if (r==REMOVAL_SUCCESS) printf("\nExcluido.\n");
      if (r==NO_SUCH_USER) printf("\nERROR: User not found.\n");

      //getchar();
    }

  }
  while (opcao != 5);
  
  return 0;
  
}
