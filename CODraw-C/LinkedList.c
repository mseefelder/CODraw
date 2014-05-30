typedef enum
{
  INCLUSAO_BEM_SUCEDIDA    = 100,
  MEMORIA_INSUFICIENTE     = 101,
  MATRICULA_DUPLICADA      = 102,

  EXCLUSAO_BEM_SUCEDIDA    = 200,
  MATRICULA_NAO_ENCONTRADA = 201
}
ReturnCode;

typedef struct
{
  int dia;
  int mes;
  int ano;
} Data;

typedef struct
{
  int index; //campo chave
  int   sock;
  char  name[USERNAME_SIZE+1];
} User;
/*
typedef struct
{
  int   matricula;  // campo chave
  char  name[TAM_MAX_NOME_ALUNO+1];
  float cr;
  Data  dataNascimento;
} User;
*/

typedef struct linkedList
{
  User user;
  struct linkedList *nextNode;
} Node; 

//----------------------------------------------------
// Criar uma nova turma
//----------------------------------------------------
/*
Node *criarTurma()
{
  return NULL;
}
*/

Node *createList()
{
  return NULL;
}

Node *newlist()
{
  Node *newNode;
  User = newuser;

  newuser.index = -1; //so vai ser lido por getElNum()
  newuser.sock = 0; //numero de elementos da lista

  newNode = malloc(sizeof(Node))
  if (newNode == NULL) return MEMORIA_INSUFICIENTE;

  newNode->user = &newuser;

  newNode->nextNode = NULL;

  return newNode;
}

//----------------------------------------------------
// Incluir um novo user em uma turma
//----------------------------------------------------

ReturnCode insert(Node **t, User *a) //inclui um user 'a' na lista 't' 
{
  Node *newNode;

  // 1. Criar uma nova celula para armazenar o novo item
  
  newNode = malloc(sizeof(Node));
  if (newNode == NULL) return MEMORIA_INSUFICIENTE;

  // 2. Armazenar o novo user na nova celula. Coloca o index dele como
  //o tamanho da lista.
  
  newNode->user = *a;
  newNode->index = t->user.sock;
  
  // 3. Nova celula entra logo depois da -1(head)
  
  newNode->nextNode = t->nextNode;
  
  // head passa a apontar pra nova celula, soma mais um elemento

  t->nextNode = *newNode;
  t->user.sock++;
  
  // 5. Retornar informando incluso bem-sucedida
  
  return INCLUSAO_BEM_SUCEDIDA;
}


//----------------------------------------------------
// Lisar users e seus sockets
//----------------------------------------------------
void listAll(Node *t)
{
  int n = t->user.sock;
  int sock = 0;

  for(int i = 0; i<n; i++)
  {
    sock = getSock(*t, i);
    printf("Usuario[%d]:  sock = %d", i, sock);
  }
}
//----------------------------------------------------
// Procurar um user, retorna o ponteiro para o mesmo
//----------------------------------------------------
int *getUser(Node *t, int index)
{
  Node *p;

  for (p=t; p; p=p->nextNode)
  {
    if (p->user.index == index) return &(p->user);
  }
  return NULL;
}
//----------------------------------------------------
// Pesquisar o socket de um determinado user
//----------------------------------------------------
int getSock(Node *t, int index)
{
  Node *p;

  for (p=t; p; p=p->nextNode)
  {
    if (p->user.index == index) return (p->user.sock);
  }
  return NULL;
}

//----------------------------------------------------
// Excluir um user
//----------------------------------------------------
int delete(Node **t, int index)
{
  if(index == -1)
    return FORBIDDEN_PROCEDURE;
  Node *found, *p, *anterior;

  // 1. Localizar o registro a ser excluido da lista
  
  anterior = NULL;
  found = NULL;
  for (p=*t->nextNode; p; p=p->nextNode)
  {
    if (p->user.index == index) found = p;
    if (found) p->nextNode.user.index = p->user.index;
    anterior = p;
  }

  // 2. se a matricula procurada tiver sido encontrada
  
  if (p)
  {
     if (anterior) // user a ser excluido NAO eh o primeiro da lista
     {
        // user anterior passa a apontar o user seguinte ao excluido
        anterior->nextNode = p->nextNode;
        
        // liberar memoria ocupada pelo user excluido
        free(p);
     }
     else          // user a ser excluido eh o primeiro da lista
     {
        // liberar memoria ocupada pelo user excluido
        free(*t);

        // user seguinte ao excluido passa a ser o primeiro da lista
        *t->nextNode = p->nextNode;
     }

    // 2.3. Retornar indicando exclusao bem sucedida
    
    return EXCLUSAO_BEM_SUCEDIDA;
  }
  else  // matricula nao encontrada
  {
    return MATRICULA_NAO_ENCONTRADA;
  }
}

//----------------------------------------------------
// Pesquisar dados de um user
//----------------------------------------------------
/*
User *pesquisar(Node *t, int matriculaProcurada)
{
  Node *p;

  for (p=t; p; p=p->nextNode)
  {
    if (p->user.matricula == matriculaProcurada) return &(p->user);
  }
  return NULL;
}
*/

//----------------------------------------------------
// Excluir um user de uma turma
//----------------------------------------------------
/*
int excluir(Node **t, int matriculaProcurada)
{
  Node *p, *anterior;

  // 1. Localizar o registro a ser excluido da lista
  
  anterior = NULL;
  for (p=*t; p; p=p->nextNode)
  {
    if (p->user.matricula == matriculaProcurada) break;
    anterior = p;
  }

  // 2. se a matricula procurada tiver sido encontrada
  
  if (p)
  {
     if (anterior) // user a ser excluido NAO eh o primeiro da lista
     {
        // user anterior passa a apontar o user seguinte ao excluido
        anterior->nextNode = p->nextNode;
        
        // liberar memoria ocupada pelo user excluido
        free(p);
     }
     else          // user a ser excluido eh o primeiro da lista
     {
        // liberar memoria ocupada pelo user excluido
        free(*t);

        // user seguinte ao excluido passa a ser o primeiro da lista
        *t = p->nextNode;
     }

    // 2.3. Retornar indicando exclusao bem sucedida
    
    return EXCLUSAO_BEM_SUCEDIDA;
  }
  else  // matricula nao encontrada
  {
    return MATRICULA_NAO_ENCONTRADA;
  }
}
*/

/*
//incluir user criada anteriormente
ReturnCode insert(Node **t, User *a) //inclui um user 'a' na lista 't' 
{
  Node *newNode;

  // 1. Criar uma nova celula para armazenar o novo item
  
  newNode = malloc(sizeof(Node));
  if (newNode == NULL) return MEMORIA_INSUFICIENTE;

  // 2. Armazenar o novo user na nova celula.
  
  newNode->user = *a;
  
  // 3. Fazer a nova celula referenciar a primeira celula da lista
  
  newNode->nextNode = *t;
  
  // 4. Passar a referenciar como primeira celula da lista a nova celula.

  *t = newNode;
  
  // 5. Retornar informando incluso bem-sucedida
  
  return INCLUSAO_BEM_SUCEDIDA;
}
*/


//----------------------------------------------------
// Listar todos os alunos de uma turma
//----------------------------------------------------
/*
void listar(Node *t)
{
  Node *p;
  
  // 1. Imprimir cabecalho
  
  system("cls");
  printf("t = %p\n\n",t);
  printf ("           MATRICULA  NOME                 CR\n");

  // 2. Imprimir alunos


  for (p=t; p; p=p->nextNode)
  {
    printf("%p : %10d %20s %4.1f (proximo: %p)\n",
             p,
             p->user.matricula,
             p->user.name,
             p->user.cr,
             p->nextNode);
  }
}
*/