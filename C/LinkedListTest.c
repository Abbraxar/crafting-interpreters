#include <stdio.h>
#include "LinkedList.h"

void testCreateList()
{
    printf("Test createList\n");
    Linkedlist list = createList();
    printf("List created\n");
    displayList(&list);
}

void testCreateNode()
{
    printf("Test createNode with value a\n");
    Node node = createNode("a");
    printf("Node value: %s\n", node.data);
}

void testInsertBegining()
{
    printf("Test insertBegining with value a\n");
    Linkedlist list = createList();
    Node node = createNode("a");
    insertBegining(&list, &node);
    printf("Head node value: %s\n", list.head->data);
}

void testInsertEnd()
{
    printf("Test insertEnd with value a\n");
    Linkedlist list = createList();
    Node node = createNode("a");
    insertEnd(&list, &node);
    printf("Tail node value: %s\n", list.tail->data);
}

void testFind()
{
    printf("Test find from three nodes a b c\n");
    Linkedlist list = createList();
    Node node1 = createNode("a");
    insertEnd(&list, &node1);
    Node node2 = createNode("b");
    insertEnd(&list, &node2);
    Node node3 = createNode("c");
    insertEnd(&list, &node3);
    displayList(&list);
    printf("Found node value for b: %s\n", find(&list, "b")->data);
    printf("Found node value for c: %s\n", find(&list, "c")->data);
    printf("Found node value for a: %s\n", find(&list, "a")->data);
}

void testDelete()
{
    printf("Test delete from three nodes a b c\n");
    Linkedlist list = createList();
    Node node1 = createNode("a");
    insertEnd(&list, &node1);
    Node node2 = createNode("b");
    insertEnd(&list, &node2);
    Node node3 = createNode("c");
    insertEnd(&list, &node3);
    displayList(&list);
    printf("Delete b: ");
    delete(&list, find(&list, "b"));
    displayList(&list);
    printf("Delete a: ");
    delete(&list, find(&list, "a"));
    displayList(&list);
    printf("Delete d: ");
    delete(&list, find(&list, "d"));
    displayList(&list);
}

int main() {
    testCreateList();
    testCreateNode();
    testInsertBegining();
    testInsertEnd();
    testFind();
    testDelete();

    return 0;
}
