#include<string.h>
#define USERNAME_SIZE 20

typedef enum
{
  INSERTION_SUCCESS    = 100,
  INSUFFICIENT_MEMORY     = 101,
  DUPLICATE_SOCKET      = 102,

  REMOVAL_SUCCESS    = 200,
  NO_SUCH_USER = 201,

  FORBIDDEN_PROCEDURE      = 300
}
ReturnCode;

typedef enum { false, true } bool;

typedef struct
{
  int index; //main key
  int   sock; 
  char  name[USERNAME_SIZE+1];
} User;

typedef struct linkedList
{
  User user;
  struct linkedList *nextNode; //points to an older node
  struct linkedList *prevNode; //points to a newer node
} Node; 

/*
Create list:
  List is created with a fake user, with the invalid index -1.
  This fake user is used to store our number of elements on the
'sock' field.
*/
Node *newList()
{
  Node *newNode;
  User newuser;

  newuser.index = -1; //invalid index for operations. only read by getElNum()
  newuser.sock = 0; //here we'll keep track of the number of elements

  newNode = malloc(sizeof(Node));

  newNode->user = newuser;
  newNode->nextNode = NULL;
  newNode->prevNode = NULL;

  printf("lista criada");
  return newNode;
}

//Insert node:-----------------------------
ReturnCode insert(Node **t, User *a) //includes user 'a' in list 't' 
{
  Node *newNode;
  Node *p;
  p=*t;
  printf("1");

  //WARNING - Not checking if index or sock already exists.
  //In our application it won't be necessary

  // 1. Create new node on memory
  
  newNode = malloc(sizeof(Node));
  if (newNode == NULL) return INSUFFICIENT_MEMORY;

  printf("2");

  // 2. Attributes the new user to this node
  
  newNode->user = *a;
  newNode->user.index = p->user.sock;
  
  printf("3");
  // 3. New node gets in the list right after index '-1'(head)
  
  newNode->nextNode = p->nextNode;
  newNode->prevNode = p;
  
  printf("4");
  // head nextNode points to the new node

  if(p->nextNode) p->nextNode->prevNode = newNode;
  p->nextNode = newNode;
  p->user.sock++;
  
  // 5. Returns it was successfull
  
  return INSERTION_SUCCESS;
}

//Searches---------------------------------

//Get full user from index
User *getUser(Node *t, int index)
{
  Node *p;

  for (p=t; p; p=p->nextNode)
  {
    if (p->user.index == index) return &(p->user);
  }
  return NULL;
}

//Get socket from index
int getSock(Node *t, int index)
{
  Node *p;

  for (p=t; p; p=p->nextNode)
  {
    if (p->user.index == index) return (p->user.sock);
  }
  return -1;
}

//Get index from socket
int getFromSock(Node *t, int index)
{
  Node *p;

  for (p=t; p; p=p->nextNode)
  {
    if (p->user.sock == index) return (p->user.index);
  }
  return -1;
}

//List elements:---------------------------

//List element indices and their respective sockets
void listAll(Node *t)
{
  int n = t->user.sock;
  int sock = 0;
  int i = 0;

  for(i; i<n; i++)
  {
    sock = getSock(t, i);
    printf("Usuario[%d]:  sock = %d \n", i, sock);
  }
}

/*
Lists all elements in two steps:
  The first shows each element and to wich element its
nextNode pointer points to;
  The second shows each element and to wich element its
prevNode pointer points to;
  Model:
  [ index | username ] ->
*/
void listAllS(Node *t)
{
  Node *p;

  printf("\n Next node sequence: \n");
  p=t;
  while (p->nextNode != NULL)
  {
    printf("[i: %d | name: %s] -> ", p->user.index, p->user.name);
    p=p->nextNode;
  }
  printf("[i: %d | name: %s ] next-> ", p->user.index, p->user.name);
  printf("\n Previous node sequence: \n");
  for (p; p; p=p->prevNode)
  {
    printf("[%d : %s] -> ", p->user.index, p->user.name);
  }
  printf("\n");
}

//Delete element:----------------------------
//Searches by index and deletes
int delete(Node **t, int index)
{
  //
  printf("1");
  if (index<0)
    return FORBIDDEN_PROCEDURE;
  Node *n, *anterior, *posterior;
  bool found = false;
  anterior = *t;
  for(n=(**t).nextNode; n; n=n->nextNode)
  {

    if(n->user.index == index) {
      found = true;  
      break;      
    }
    printf("%s\n", n->user.name);
    anterior = n;
  }
  printf("2");

  if(found == true)
  {
    anterior->nextNode = n->nextNode;

    posterior = n;
    if(posterior->nextNode != NULL)
    {
      posterior = posterior->nextNode;  
      posterior->prevNode = n->prevNode;
    }
    free(n);
    
    printf("Index anterior [%d]=",anterior->user.index);
    while(anterior->user.index > -1)
    {
      printf("Editando [%d]...\n", anterior->user.index );
      anterior->user.index -= 1;
      anterior = anterior->prevNode;
    }

    (**t).user.sock -= 1;
  
    return REMOVAL_SUCCESS;
  }
  else
  {
    return NO_SUCH_USER;
  }
}