#include <iostream>
#include <vector>
using namespace std;
#define HEAP_SIZE 100

struct Node {
    int size;
    bool free;
    Node* next;
};

class HeapManager 
{

private:

    Node* Head;

public:
    HeapManager(int heapSize)           // constructor
    {
        Head = new Node{heapSize, true, nullptr};
    }

    void* allocate(int alloc_size)    //assume heap to store any values
    {
        Node* current = Head;
        
        while (current!=nullptr && !(current->free==true && current->size >= alloc_size))   // Find a free block large enough 
        {
            current = current->next;
        }

        if (current==nullptr) {
            cout << "Insufficient memory,not allocated" << endl;
            return nullptr;
        }

        // If the block is larger than needed, split it
        if (current->size > alloc_size) 
        {
            Node* newBlock = new Node{current->size - alloc_size, true, current->next};
            current->size = alloc_size;
            current->free = false;
            current->next = newBlock;
        }
        else if(current->size == alloc_size)
        {
            current->free = false;
        }
        cout<<"allocated\n";
        return reinterpret_cast<void*>(current);
    }

    void free(void* ptr)
    {
        Node* ToFree = reinterpret_cast<Node*>(ptr) ;
        ToFree->free = true;
        cout<<"freed\n";
        
       Node* current = Head;
        while (current != nullptr && current->next != nullptr) {
        if (current->free && current->next->free) 
        {
            // Merge free node with its next free node
            current->size += current->next->size;
            Node* temp = current->next;
            current->next = current->next->next;
            cout<<"merged\n";
            delete temp; 
        } 
        else 
        {   
            current = current->next;
        }
    }
    }
    ~HeapManager()  //destructor
    {
        Node* current = Head;
        while (current != nullptr) 
        {
        Node* next = current->next;
        delete current;
        current = next;
        }
    }
};

int main() {
    HeapManager heap(HEAP_SIZE);

    void* ptr1 = heap.allocate(10);
    void* ptr2 = heap.allocate(20);

    heap.free(ptr1);

    void* ptr3 = heap.allocate(30);
    heap.free(ptr2);
    
    
    return 0;
}
