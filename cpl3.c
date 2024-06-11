#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_KEY_LENGTH 10
#define TABLE_SIZE 100

// Define a structure for the hash table entry
typedef struct {
    int key;
    int value;
}Entry;

// Define a structure for the hash table
typedef struct {
    Entry entries[TABLE_SIZE];
} HashTable;

typedef struct Node{
    int level;
    struct Node* next;
    HashTable* ht;
  } Node;


void initialiseHashTable(HashTable* ht) 
{
    for (int i = 0; i < TABLE_SIZE; i++) {
        ht->entries[i].key = -1;
        ht->entries[i].value = 0;
    }
}
// Hash function to convert a key to an index
int hash(char* key) {
    int sum = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        sum =sum*10 + key[i];
    }
    return sum % TABLE_SIZE;
}

Node* begin(Node*head,int level) 
{
    Node* new_node = (Node*) malloc(sizeof(Node));
    new_node->level = level;
    new_node->next = head;
    new_node->ht=(HashTable*)malloc(sizeof(HashTable));
    initialiseHashTable(new_node->ht);
    return new_node;
}

Node* end(Node*head)
{
    Node *temp=head;
    head=head->next;
    free(temp->ht);
    free(temp);
    return head;
}

void assign(char*key,char* val,Node*head)
{
    int k=hash(key);
    int value=atoi(val);
    if(k==head->ht->entries[k].key)
    {
        printf("redeclaration of existing already present variable  %s\n",key);
    }
    else
    {
        head->ht->entries[k].key=k;
        head->ht->entries[k].value=value;
    }
}

void print (char* key,Node*head)
{
    int k=hash(key);
    Node *temp=head;
    for (int i=head->level;i>=0;i--) 
    {
        if(temp->ht->entries[k].key==k)
        {
            printf("%d\n",temp->ht->entries[k].value);
            return;
        }
        else
        {
            temp=temp->next;
        }
        
    }
       
        printf("no such variable");
    
}

int main() {
    Node*head=NULL;
    
    int level=0;
    FILE *iptr;
    iptr = fopen("items.txt","r");
    if (iptr == NULL) {
        printf("Error opening file iptr\n");
        exit(1);
    }
    char line[100];
    while(fscanf(iptr, "%s", line) == 1)
    {
        if(strcmp(line,"begin")==0)
        {
            level++;
            head=begin(head,level);
        }

        else if(strcmp(line,"end")==0)
        {           
            head=end(head);
            level--;
        }
        else if(strcmp(line,"assign")==0)
        {
            char name[100];
            char value[20];
            fscanf(iptr, "%s %s", name, value);
            assign(name, value,head);
        }

        else if(strcmp(line,"print")==0)
        {
            char name[100];
            fscanf(iptr, "%s", name);
            print(name,head);
        }
    }
    fclose(iptr);
    
    return 0;
}


 

