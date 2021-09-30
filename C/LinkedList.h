typedef struct Node
{
    struct Node *prev;
    struct Node *next;
    char *data;
} Node;

typedef struct Linkedlist
{
    Node *head;
    Node *tail;
} Linkedlist;

Linkedlist createList();

Node createNode(char *data);

void insertBefore(Linkedlist *list, Node *node, Node *newNode);

void insertAfter(Linkedlist *list, Node *node, Node *newNode);

void insertBegining(Linkedlist *list, Node *newNode);

void insertEnd(Linkedlist *list, Node *newNode);

struct Node* find(Linkedlist *list, char *data);

void delete(Linkedlist *list, Node *node);

void displayNode(Node *node);

void displayList(Linkedlist *list);
