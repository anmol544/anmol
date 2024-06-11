#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<stdbool.h>
#define max_aisle 10
#define today 20240310   //10 march 2024


typedef struct Item {
    int item_id;
    char item_name[50];
    int quantity;
    char expiry_date[9];         //assume YYYYMMDD format
    int threshold_quantity;
    struct Item* next; // Pointer to the next item in the aisle
    char type[50]; // Type of the item
}Item;

typedef struct Aisle {
    Item* head;  // Head pointer of the linked list
    int aisle_number; // id of aisles
    char type[50]; // type of the item
}Aisle;

Aisle aisles[max_aisle];

typedef struct billNode
{
    int id; // id of the item purchased
    int quantity; // quantity of the item
    struct billNode* next; // Pointer
}billNode;

typedef struct bill{
    struct bill* next; // Head pointer of the linked list
    billNode*  head; // Pointer
    int billno; // id no of the bill

}bill;

void displayAllBills(bill*head_Bill)
{   bill*temp=head_Bill; 
    while(temp!=NULL)
    {
        while(temp->head !=NULL)
        {
            printf("%d ", temp->head->id);
            printf("%d \n", temp->head->quantity);
            temp->head = head_Bill->head->next;
        }
        temp = temp->next;
    }
}
Item* available(int id, int quantity,char*expiry_date)//input date
{
     for (int i = 0; i < max_aisle; i++)
    {
        Item *current = aisles[i].head;
        while (current != NULL)
        {
            if (current->item_id == id)
            {
                int result = strcmp(current->expiry_date, expiry_date);
                if (current->quantity >= quantity && result >= 0)
                {
                    return current;
                }
            }
            current = current->next;
        }
    }
    
    return NULL;
}



bill* addBill(int  billno, bill* head)
{   
    bill*ptr=(bill*)malloc(sizeof(bill));
    ptr->head=NULL;
    ptr->billno=billno;
    ptr->next=head;
    return ptr;
}

void startpurchasing(bill *head_Bill)
{   
    bill *temp_bill = head_Bill; 
    
    while (temp_bill != NULL)
    {   
        billNode *temp_node = temp_bill->head;
        
        while (temp_node != NULL)
        {   
            Item *ptr = available(temp_node->id, temp_node->quantity, "00000000");
            
            if (ptr != NULL)
            {
                if (ptr->quantity >= temp_node->quantity)
                {
                    ptr->quantity -= temp_node->quantity;
                    printf("Purchased: %s\n", ptr->item_name);
                }
                else
                {
                    printf("Not enough quantity available for item: %s\n", ptr->item_name);
                }
            }
            else
            {
                printf("Item not available: %d\n", temp_node->id);
            }
            temp_node = temp_node->next;
        }
        temp_bill = temp_bill->next;
    }
}


billNode* addbillNode(int quantity,int id, bill* bptr)
{
    billNode*ptr=(billNode*)malloc(sizeof(billNode));
    ptr->id=id;
    ptr->quantity=quantity;
    ptr->next=bptr->head;
    return ptr;
}


bool isSubstring(const char* substr, const char* str) {
    int len_str = strlen(str);
    int len_substr = strlen(substr);

    if (len_str < len_substr) {
        return false;  // Substring cannot be longer than the original string
    }

    for (int i = 0; i <= len_str - len_substr; ++i) {
        int j;
        for (j = 0; j < len_substr; ++j) {
            if (str[i + j] != substr[j]) {
                break;
            }
        }
        if (j == len_substr) {
            return true;  // Substring found
        }
    }

    return false;  // Substring not found
}

void addtoaisle(Item*ptr,int id)    //check to add at head
{
    if (aisles[id].head==NULL)
    {
        aisles[id].head=ptr;
    }
   else {
        // Find the correct position to insert the new node
        Item* prev = aisles[id].head;
        Item* current = prev->next;
        
        while (current != NULL && current->item_id < ptr->item_id) {
            prev = current;
            current = current->next;
        }

        // Check if the item with the same id exists
        if (current != NULL && current->item_id == ptr->item_id) {
            // Update the existing node with the new data and free the new node
            current->quantity += ptr->quantity;
            // current->threshold_quantity = ptr->threshold_quantity;
            // strcpy(current->expiry_date, ptr->expiry_date);
            // strcpy(current->type, ptr->type);
            free(ptr);
        } else {
            // Insert the new node between prev and current
            ptr->next = current;
            prev->next = ptr;
        }
    }
}
// Function to create a new item node
Item* createItem(int id, const char* name, int qty, const char* exp_date, int threshold_quantity,char* type) {

    Item* newItem = ( Item*)malloc(sizeof(Item));
    if (newItem != NULL) {
        newItem->item_id = id;
        snprintf(newItem->item_name, sizeof(newItem->item_name), "%s", name);
        newItem->quantity = qty;
        snprintf(newItem->expiry_date, sizeof(newItem->expiry_date), "%s", exp_date);
        newItem->threshold_quantity = threshold_quantity;
        newItem->next = NULL;
        strcpy(newItem->type ,type);
    }
    return newItem;
}

void addItem(int id, const char* name, int quantity, const char* exp_date, int threshold_quantity,char* type)
{
    Item* ptr=createItem(id, name, quantity, exp_date, threshold_quantity, type);
    if(ptr!=NULL)
    {  
        int i=0;
        while(!isSubstring(ptr->type ,aisles[i].type) && i!=max_aisle)
        {
            i++;
        }
        if(isSubstring(ptr->type ,aisles[i].type))
        {
            addtoaisle(ptr,i);
        }
        else
        {
            printf("no such aisle");
        }
    }
    
}

void deleteitem(int id)
{
    int i=0;
    while( i!=max_aisle)
    {   
        Item *prev=aisles[i].head;
        Item* temp=prev->next;
        while(temp!=NULL && temp->item_id!=id)
        {
            prev=prev->next;
            temp=temp->next;
        }
        if(temp!=NULL && temp->item_id==id) 
        {
            prev->next=temp->next;
            free(temp);
            return;
        }
        i++;
    }
    printf("no such item ID found");
}
// Function to display all items in the aisle  ////to be modified

void displayAisle(int id) {
    printf("aisle number: %d\n",aisles[id].aisle_number);
    printf("type: %s\n",aisles[id].type);
    printf("ID\t Name\tQty\tExpiry \tThr quantity\n");
    Item* current = aisles[id].head;
    while (current != NULL) {
        printf("%d \t %s \t %d \t %s \t %d \n", current->item_id, current->item_name, current->quantity, current->expiry_date,current->threshold_quantity);
        current = current->next;
    }
}


// Function to delete aisle
void freeAisle(int id) {
    Item* current = aisles[id].head;
    strcpy(aisles[id].type, ".");
    while (current != NULL) {
        Item* temp = current;
        current = current->next;
        free(temp);
    }
}

void checkthreshold(int id)
{
      int i=0;
    while(aisles[i].type!="." && i!=max_aisle)
    {   
        Item *prev=aisles[i].head;
        Item* temp=prev->next;
        while(temp!=NULL && temp->item_id!=id)
        {
            prev=prev->next;
            temp=temp->next;
        }
        if(temp!=NULL && temp->item_id==id) 
        {
           if ( temp->quantity  <  temp->threshold_quantity )
           {
            printf("quantity below threshold , replenish it!\n");
           }
           return;
        }
        i++;
    }
    printf("no such item ID found\n");
}

void initializeAisles(){ 
    for(int i=0; i<max_aisle; i++)
    {
        aisles[i].head = NULL;
        aisles[i].aisle_number = i;
        strcpy(aisles[i].type, ".");   
    }  
     strcpy(aisles[0].type, "dairy");  
     strcpy(aisles[1].type, "pulse");  
     strcpy(aisles[2].type, "clean"); 
     strcpy(aisles[3].type, "food");
     strcpy(aisles[4].type, "veg"); 
}

void rte_expiry()
{
    int i;
    for(i=0; i<max_aisle; i++)
    {
        if(isSubstring("food",aisles[i].type)) break;
    }
    if(strcmp(aisles[i].type, "food") == 0)
    {
        int d=today%100;
        int m=(today/100)%100;
        int y=(today/10000);
        d-=7;
        if (d<0)
        {
            if (m==2||m==4||m==6||m==8||m==9||m==11||m==1)
            {
                d+=31;
                m--;
            }
            else if (m==1||m==5||m==7||m==8||m==10||m==12)
            {
                d+=30;
                m--;
            }
            else
            {   if (y%4==0)
                {
                    if (y%400==0)
                    {
                        d+=28;
                        m--;
                    }        
                    else
                    {
                        d+=29;
                        m--;
                    }
                    
                }
                else
                {
                    d+=28;
                    m--;
                }

            }
            
            if (m<0)
            {
            m+=12;
            y--;
            }
        }
        char check[50];
        sprintf(check, "%04d%02d%02d", y, m, d);

        Item* temp=aisles[i].head;
        int res;
        while(temp!=NULL)
        {
            res=strcmp(check , temp->expiry_date);
            if (res<=0) printf("rte expiry within a week Item id %d Item name %s \n",temp->item_id,temp->item_name);
            temp=temp->next;
        }


    }
    else printf("no ready to eat food");
    }

void mergeaisles(int id1, int id2)
{
   strcat( aisles[id1].type, aisles[id2].type);
   Item *temp=aisles[id1].head;
   if (temp==NULL) aisles[id1].head=aisles[id2].head;
   while(temp->next!=NULL)
   {
    temp=temp->next;
   }
   temp->next=aisles[id2].head;
   strcpy(aisles[id2].type,".");
   aisles[id2].head=NULL;
}

void dairyexp()

{
    int i;
    for(i=0; i<max_aisle; i++)
    {
        if(isSubstring("dairy",aisles[i].type)) break;
    }
    if(strcmp(aisles[i].type, "dairy") == 0)
    {
        int d=today%100;
        int m=(today/100)%100;
        int y=(today/10000);
        d-=1;
        if (d<0)
        {
            if (m==2||m==4||m==6||m==8||m==9||m==11||m==1)
            {
                d+=31;
                m--;
            }
            else if (m==1||m==5||m==7||m==8||m==10||m==12)
            {
                d+=30;
                m--;
            }
            else
            {   if (y%4==0)
                {
                    if (y%400==0)
                    {
                        d+=28;
                        m--;
                    }        
                    else
                    {
                        d+=29;
                        m--;
                    }
                    
                }
                else
                {
                    d+=28;
                    m--;
                }

            }
            
            if (m<0)
            {
            m+=12;
            y--;
            }
        }
        char check[50];
        sprintf(check, "%04d%02d%02d", y, m, d);


        Item* temp=aisles[i].head;
        int res;
        while(temp!=NULL)
        {
            res=strcmp(check , temp->expiry_date);
            if (res<=0) printf("dairy expiry within a day Item id %d Item name %s \n",temp->item_id,temp->item_name);
        }


    }
    else printf("no ready to eat food");
    }

void addaisle(char* type ,int id)
{
    strcpy(aisles[id].type , type);
}

void frequentlybought(int id ,bill*head_Bill) 
{   printf("frequently bought:\n");
    int found ; 
    bill* temp_bill = head_Bill;
    while(temp_bill!=NULL )
    {   found=0;
        billNode * temp = temp_bill->head;
        while(temp!=NULL)
        {
            if(temp->id==id)
            {   
                found++;                
            }
            temp=temp->next;
        }
        if(found>0)
        {
            temp=temp_bill->head;
            {
                while(temp!=NULL)
                {
                    if(temp->id!=id)
                    {
                        printf("id no.%d\n", temp->id);
                    }
                    temp=temp->next;
                }
            }
        }
        temp_bill=temp_bill->next;
    }
}

void deletebills( bill* head_Bill)
{
     bill *temp_bill = head_Bill; 
     bill *prev_bill;
    while (temp_bill != NULL)
    {   
        billNode *temp_node = temp_bill->head;
        billNode *prev;
        while (temp_node != NULL)
        {   
            
            prev = temp_node;
            temp_node = temp_node->next;
            free (prev);
        }
        temp_bill->head = NULL;
        prev_bill = temp_bill;
        temp_bill = temp_bill->next;
        free (prev_bill);
    }
    
}

int main() {


    initializeAisles();
    FILE *iptr,*bptr;

    iptr = fopen("items.txt","r");
    if (iptr == NULL) {
        printf("Error opening file iptr\n");
        exit(1);
    }
    int id;
    char item_name[50];
    int quantity=1;
    char expiry_date[9];         //assume YYYYMMDD format
    int threshold_quantity=0;

    char type[50]; // Type of the item
    
   while (fscanf(iptr, "%d", &id) == 1 && id>=0) 
   {
    // Read the rest of the items on the same line
    fscanf(iptr, "%s %d %s %d %s", item_name, &quantity, expiry_date, &threshold_quantity, type);
   
    // Add item to aisle
    addItem(id, item_name, quantity, expiry_date, threshold_quantity, type);
    
    }
    fclose(iptr);
     displayAisle(2);

    // printf("id=%d", id);
    bptr = fopen("bills.txt", "r");
    if (bptr == NULL) {
        printf("Error opening file bptr\n");
        exit(1);
    }

    int billno;
    
    bill*head_Bill= NULL;
    while (fscanf(bptr, "%d", &billno) == 1 && billno>=0) 
   {    
       head_Bill=addBill(billno,head_Bill);
       while( fscanf(bptr, "%d", &id) == 1 && id>=0) 
       {
        // Read the rest of the items on the same line
        fscanf(bptr, "%d", &quantity);
        // Add item to aisle
        head_Bill->head=addbillNode(quantity,  id , head_Bill);
       }
   }
    fclose(bptr);
    
    // startpurchasing(head_Bill);
    
    deleteitem(12);
    displayAisle(2);
    // frequentlybought(30,head_Bill);
    for(int i=0;i<max_aisle;i++)
    {
        freeAisle(i);
    }
    
    deletebills(head_Bill);
    head_Bill = NULL;
    return 0;
    
}
