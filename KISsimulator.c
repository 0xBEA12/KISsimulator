#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct allowedMACs 
{  	
  int linkedMAC;  	
  struct allowedMACs *next; 
} links;
///////////////////////////////////////////////////////////////
typedef struct tree_node 
{
	int MAC;
	char firstName[21];
	char lastName[21];
	int broadcastRange;
	int x;
	int y;
	links *deviceLinks;
	int numLinks;
	struct tree_node *left;
	struct tree_node *right;
} BSTnode;
///////////////////////////////////////////////////////////////
BSTnode* CreateNode(FILE* KISSI)
{
  BSTnode* newNode = (BSTnode*)malloc(sizeof(BSTnode));
  fscanf(KISSI, "%d %s %s %d %d %d", &newNode->MAC, newNode->firstName, newNode->lastName, &newNode->broadcastRange, &newNode->x, &newNode->y);
  newNode->deviceLinks = NULL;
  newNode->numLinks = 0;
  newNode->left = NULL;
  newNode->right = NULL;
  return newNode;
}
///////////////////////////////////////////////////////////////
int KISParticipating(BSTnode* currentPtr, int MAC)
{
  if(currentPtr != NULL)
  {
    if(currentPtr->MAC == MAC)
      return 1;
    else if(MAC < currentPtr->MAC)
      return KISParticipating(currentPtr->left, MAC);
    else 
      return KISParticipating(currentPtr->right, MAC);
  }
  else 
    return 0;
}
///////////////////////////////////////////////////////////////
BSTnode* Insert(BSTnode* root, BSTnode* tempNode1)
{
  if(root == NULL) 
    return tempNode1;
  else
  {
    if(tempNode1->MAC > root->MAC)
    root->right = Insert(root->right, tempNode1);
    else root->left = Insert(root->left, tempNode1);
    
    return root;
  }
}
///////////////////////////////////////////////////////////////
int LinkCheck(BSTnode* tempNode1, BSTnode* tempNode2, int MAC1, int MAC2)
{
  //checks if two nodes are linked together
  //1 means a node doesn't exist
  //2 means they are linked already
  //0 means the nodes exist but are not linked
  if(tempNode1 == NULL || tempNode2 == NULL)
  {
    return 1;
  }
  
  links* helpPtr1 = tempNode1->deviceLinks;
  links* helpPtr2 = tempNode2->deviceLinks;

  while(helpPtr1 != NULL)
  {
    if(helpPtr1->linkedMAC == MAC2)
    {
      return 2;
    }
    helpPtr1 = helpPtr1->next;
  }

  while(helpPtr2 != NULL)
  {
    if(helpPtr2->linkedMAC == MAC1)
    {
      return 2;
    }
    helpPtr2 = helpPtr2->next;
  }

  return 0;
}
///////////////////////////////////////////////////////////////
links* LinkDevices(links* head, int MAC)
{
  //links devices together
  links* helpPtr;
  links* tempLink = (links*)malloc(sizeof(links));
  tempLink->linkedMAC = MAC;
  tempLink->next = NULL;

  if(head == NULL) 
    return tempLink;
  else
  {
    helpPtr = head;
    while(helpPtr->next != NULL)
    {
      helpPtr = helpPtr->next;
    }

    helpPtr->next = tempLink;
      return head;
  }
}
///////////////////////////////////////////////////////////////
links* UnlinkDevices(links* head, int MAC)
{
  links *helpPtr, *current;

  if(head == NULL)
    return head;
  else
  {
    current = head;

    if(current->linkedMAC == MAC)
    {
      helpPtr = current->next;
      free(current);
        return helpPtr;
    }

    while(current->next != NULL)
    {
      if(current->next->linkedMAC == MAC)
      {
        helpPtr = current->next;
        current->next = helpPtr->next;
        helpPtr->next = NULL;
        free(helpPtr);
          return head;
      }

      current = current->next;
    }

    return head;
  }
}
///////////////////////////////////////////////////////////////
BSTnode* NodeSearch(BSTnode* currentPtr, int MAC)
{
  if(currentPtr != NULL)
  {
    if(currentPtr->MAC == MAC) 
      return currentPtr;
    else if(MAC < currentPtr->MAC)
      return NodeSearch(currentPtr->left, MAC);
    else 
      return NodeSearch(currentPtr->right, MAC);
  }
  else 
  {
    return NULL;
  }
}

void Inorder(BSTnode* p, FILE* KISSO) 
{
  if (p != NULL) 
  {
    Inorder(p->left, KISSO);
    fprintf(KISSO, "\tMAC %d, %s %s, currently at position (%d, %d), %d Links\n", p->MAC, p->firstName, p->lastName, p->x, p->y, p->numLinks);
    Inorder(p->right, KISSO);
  }
}
///////////////////////////////////////////////////////////////
//distance between broadcaster P and accessor Q
int EucDi(int Px, int Py, int Qx, int Qy)
{
  int x = (Px - Qx);
  int y = (Py - Qy);
  int xSquared = x * x;
  int ySquared = y * y;
  int preDistance = xSquared + ySquared;
  int distance = sqrt(preDistance);
    return distance;
}
///////////////////////////////////////////////////////////////
//returns number of activelinks with the broadcaster
int ActiveLinks(BSTnode *tempNode1, BSTnode *tempNode2, BSTnode *root, int broadcastRange)
{
  int activeLinks = 0;
  links *helpPtr;
  helpPtr = tempNode1->deviceLinks;
  //traverse broadcasters deviceLinks and determine
  //if those link are in range
  while(helpPtr != NULL)
  {
    tempNode2 = NodeSearch(root, helpPtr->linkedMAC);

    if(EucDi(tempNode1->x, tempNode1->y, tempNode2->x, tempNode2->y) <= tempNode1->broadcastRange)
    {
      activeLinks++;
    }

    helpPtr = helpPtr->next;
  }

  return activeLinks;
}
///////////////////////////////////////////////////////////////
void PrintActiveLinks(FILE* KISSO, BSTnode *tempNode1, BSTnode *tempNode2, BSTnode *root, int broadcastRange)
{
  links *helpPtr;
  helpPtr = tempNode1->deviceLinks;
  //traverse broadcasters deviceLinks and determine
  //if those link are in range
  while(helpPtr != NULL)
  {
    tempNode2 = NodeSearch(root, helpPtr->linkedMAC);

    if(EucDi(tempNode1->x, tempNode1->y, tempNode2->x, tempNode2->y) <= tempNode1->broadcastRange)
    {
      fprintf(KISSO, "\t\tMAC %d, %s %s, currently at position (%d, %d)\n", tempNode2->MAC, tempNode2->firstName, tempNode2->lastName, tempNode2->x, tempNode2->y);
    }

    helpPtr = helpPtr->next;
  }

}
///////////////////////////////////////////////////////////////
void Move(BSTnode* currentPtr, int X, int Y)
{
  //randomize each node's x and y coordinates
  //in ascending order(inorder)
  if(currentPtr != NULL)
  {
    Move(currentPtr->left, X, Y);
    currentPtr->x = rand() % X;
    currentPtr->y = rand() % Y;
    Move(currentPtr->right, X, Y);
  }
}
///////////////////////////////////////////////////////////////
int main() 
{
  FILE* KISSI = fopen("KISsimulator.in", "r");
  FILE* KISSO = fopen("KISsimulator.out", "w");
  FILE* KISSO2 = fopen("KISsimulator.out", "r");
  FILE* KSS2 = fopen("KSS2", "r");
  int seed, sizeX, sizeY, numCommands, tempMAC1, tempMAC2;
  char command[20];
  BSTnode* root = NULL;
  BSTnode *tempNode1, *tempNode2;

  fscanf(KISSI, "%d", &seed);
  srand(seed);
  fscanf(KISSI, "%d %d", &sizeX, &sizeY);
  fscanf(KISSI, "%d", &numCommands);

for(int i = 0; i < numCommands; i++)
{
  fscanf(KISSI, "%s", command);

  //JOIN
  if(strcmp(command, "JOIN") == 0)
  {
    tempNode1 = CreateNode(KISSI);

    if(KISParticipating(root, tempNode1->MAC) == 0)
    {
      root = Insert(root, tempNode1);
      fprintf(KISSO, "%s %s, MAC %d, joined KIS.\n", tempNode1->firstName, tempNode1->lastName, tempNode1->MAC);
    }
    else
    {
      fprintf(KISSO, "Cannot Perform JOIN Command:\n");
      fprintf(KISSO, "\tMAC %d, %s %s - already a participant in the KIS program.\n", tempNode1->MAC, tempNode1->firstName, tempNode1->lastName);
    }
  }

  //FINDMAC
  if(strcmp(command, "FINDMAC") == 0)
  {
    fscanf(KISSI, "%d", &tempMAC1);
    tempNode1 = NodeSearch(root, tempMAC1);

    if(tempNode1 == NULL)
    {
      fprintf(KISSO, "MAC %d not found in the KIS system.\n", tempMAC1);
    }
    else
    {
      if(tempNode1->numLinks == 1)
        fprintf(KISSO, "Found:  MAC %d, %s %s, currently at position (%d, %d), %d Link\n", tempNode1->MAC, tempNode1->firstName, tempNode1->lastName, tempNode1->x, tempNode1->y, tempNode1->numLinks);
      else
      {
        fprintf(KISSO, "Found:  MAC %d, %s %s, currently at position (%d, %d), %d Links\n", tempNode1->MAC, tempNode1->firstName, tempNode1->lastName, tempNode1->x, tempNode1->y, tempNode1->numLinks);
      }
    }
  }

  //LINK
  if((strcmp(command, "LINK") == 0))
  {
    fscanf(KISSI, "%d %d", &tempMAC1, &tempMAC2);
    if(KISParticipating(root, tempMAC1) == 0 || KISParticipating(root, tempMAC1) == 0)
    {
      fprintf(KISSO, "Cannot Perform LINK Command:\n");
      if(KISParticipating(root, tempMAC1) == 0)
      {
        fprintf(KISSO, "\tMAC %d - This MAC Address is not in the KIS system.\n", tempMAC1);
      }
      if(KISParticipating(root, tempMAC2) == 0)
      {
        fprintf(KISSO, "\tMAC %d - This MAC Address is not in the KIS system.\n", tempMAC2);
      }
    }

    tempNode1 = NodeSearch(root, tempMAC1);
    tempNode2 = NodeSearch(root, tempMAC2);

    int linkCheck = LinkCheck(tempNode1, tempNode2, tempMAC1, tempMAC2);

    if(linkCheck == 0)
    {
      tempNode1->deviceLinks = LinkDevices(tempNode1->deviceLinks, tempMAC2);
      tempNode2->deviceLinks = LinkDevices(tempNode2->deviceLinks, tempMAC1);
      tempNode1->numLinks++;
      tempNode2->numLinks++;
      fprintf(KISSO, "MAC %d and MAC %d are now linked.\n", tempNode1->MAC, tempNode2->MAC);
    }
    else if(linkCheck == 2)
    {
      //if(z == 1) return 0;
      fprintf(KISSO, "Cannot Perform LINK Command:\n");
      fprintf(KISSO, "\tMAC %d and MAC %d are already linked.\n", tempMAC1, tempMAC2);
    }
    
  }
  
  //UNLINK
  if((strcmp(command, "UNLINK") == 0))
  {
    fscanf(KISSI, "%d %d", &tempMAC1, &tempMAC2);

    if(KISParticipating(root, tempMAC1) == 0 || KISParticipating(root, tempMAC1) == 0)
    {
      printf("FASDFASDF");
      fprintf(KISSO, "Cannot Perform UNLINK Command:\n");
      if(KISParticipating(root, tempMAC1) == 0)
      {
        fprintf(KISSO, "\tMAC %d - This MAC Address is not in the KIS system.\n", tempMAC1);
      }
      if(KISParticipating(root, tempMAC2) == 0)
      {
        fprintf(KISSO, "\tMAC %d - This MAC Address is not in the KIS system.\n", tempMAC2);
      }
    }

    tempNode1 = NodeSearch(root, tempMAC1);
    tempNode2 = NodeSearch(root, tempMAC2);
    
    int linkCheck = LinkCheck(tempNode1, tempNode2, tempMAC1, tempMAC2);

    if(linkCheck == 2)
    {
      tempNode1->deviceLinks = UnlinkDevices(tempNode1->deviceLinks, tempMAC2);
      tempNode2->deviceLinks = UnlinkDevices(tempNode2->deviceLinks, tempMAC1);
      tempNode1->numLinks--;
      tempNode2->numLinks--;
      fprintf(KISSO, "MAC %d and MAC %d are no longer linked.\n", tempNode1->MAC, tempNode2->MAC);
    }
    else if(linkCheck == 0)
    {
      fprintf(KISSO, "Cannot Perform UNLINK Command:\n");
      fprintf(KISSO, "\tMAC %d and MAC %d are not currently linked.\n", tempMAC1, tempMAC2);
    }

  }

  //QUIT
  if((strcmp(command, "QUIT") == 0))
  {
    fscanf(KISSI, "%d", &tempMAC1);
    if(KISParticipating(root, tempMAC1) == 0)
    {
      fprintf(KISSO, "Cannot Perform QUIT Command:\n");
      fprintf(KISSO, "\tMAC %d not found in the KIS system.\n", tempMAC1);
    }
    else
    {
      tempNode1 = NodeSearch(root, tempMAC1);
      links* devicesToRemove = tempNode1->deviceLinks;
      
      while(devicesToRemove != NULL)
      {
        // Find tempNode2
        tempNode2 = NodeSearch(root, devicesToRemove->linkedMAC);
        UnlinkDevices(tempNode2->deviceLinks, devicesToRemove->linkedMAC);
        devicesToRemove = devicesToRemove->next;
      }

      devicesToRemove = tempNode1->deviceLinks;

      while (devicesToRemove != NULL)
      {
        links* next = devicesToRemove->next;
        free(devicesToRemove);
        devicesToRemove = next;
      }

      fprintf(KISSO, "MAC %d has been removed from the KIS system.\n", tempNode1->MAC);
    }
  }

  //SHOWCONNECTIONS
  if((strcmp(command, "SHOWCONNECTIONS") == 0))
  {
    
    fscanf(KISSI, "%d", &tempMAC1);
    tempNode1 = NodeSearch(root, tempMAC1);

    if(tempNode1 == NULL)
    {
      fprintf(KISSO, "Cannot Perform SHOWCONNECTIONS Command:\n");
      fprintf(KISSO, "\tMAC %d - This MAC Address is not in the KIS system.\n", tempMAC1);
    }
    else
    {
      fprintf(KISSO, "Connections for MAC %d, %s %s, currently at position (%d, %d):\n", tempMAC1, tempNode1->firstName, tempNode1->lastName, tempNode1->x, tempNode1->y);

      fprintf(KISSO, "\tThere are a total of %d link(s).\n", tempNode1->numLinks);

      fprintf(KISSO, "\tThere are %d active link(s) within the broadcast range of %d:\n", ActiveLinks(tempNode1, tempNode2, root, tempNode1->broadcastRange), tempNode1->broadcastRange);

      PrintActiveLinks(KISSO, tempNode1, tempNode2, root, tempNode1->broadcastRange);
    }
    
  }

  //PRINTKISMEMBERS
  if(strcmp(command, "PRINTKISMEMBERS") == 0)
  {
    if(root == NULL)
    {
      fprintf(KISSO, "Cannot Perform PRINTKISMEMBERS Command:\n");
      fprintf(KISSO, "\tThere are currently no participants of the KIS system.\n");
    }
    else
    {
      fprintf(KISSO, "Members of KnightsHoc Internet Share:\n");
      Inorder(root, KISSO);
    }
  }

  //MOVEDEVICES
  if(strcmp(command, "MOVEDEVICES") == 0)
  {
    if(root != NULL)
    {
       Move(root, sizeX, sizeY);
       fprintf(KISSO, "All devices successfully moved.\n");
    }
    else
    {
      fprintf(KISSO, "Cannot Perform MOVEDEVICES Command:\n");
      fprintf(KISSO, "\tThere are currently no participants of the KIS system.\n");
    }
    
  }
}

  return 0;
}