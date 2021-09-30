#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"

Linkedlist createList()
{
    Linkedlist linkedlist;
    linkedlist.head = NULL;
    linkedlist.tail = NULL;

    return linkedlist;
}

Node createNode(char *data)
{
    Node newNode;
    int len = strlen(data) + 1;
    newNode.data = malloc(len);
    strncpy(newNode.data, data, len);
    newNode.prev = NULL;
    newNode.next = NULL;

    return newNode;
}

void insertBefore(Linkedlist *list, Node *node, Node *newNode)
{
    newNode->next = node;
    if (node->prev == NULL) {
        list->head = newNode;
    } else {
        newNode->prev = node->prev;
        node->prev->next = newNode;
    }
    node->prev = newNode;
}

void insertAfter(Linkedlist *list, Node *node, Node *newNode)
{
    newNode->prev = node;
    if (node->next == NULL) {
        list->tail = newNode;
    } else {
        newNode->next = node->next;
        node->next->prev = newNode;
    }
    node->next = newNode;
}

void insertBegining(Linkedlist *list, Node *newNode)
{
    if (list->head == NULL) {
        list->head = newNode;
        list->tail = newNode;
        newNode->prev = NULL;
        newNode->next = NULL;
    } else {
        insertBefore(list, list->head, newNode);
    }
}

void insertEnd(Linkedlist *list, Node *newNode)
{
    if (list->tail == NULL) {
        insertBegining(list, newNode);
    } else {
        insertAfter(list, list->tail, newNode);
    }
}

Node* find(Linkedlist *list, char *data)
{
    if (list == NULL || list->head == NULL) {
        return NULL;
    }
    Node *node = list->head;
    while (node != NULL) {
        if (strcmp(node->data, data) == 0) {
            return node;
        }
        node = node->next;
    }
    return NULL;
}

void delete(Linkedlist *list, Node *node)
{
    if (node == NULL) {
        return;
    }
    if (node->prev == NULL) {
        list->head = node->next;
    } else {
        node->prev->next = node->next;
    }
    if (node->next == NULL) {
        list->tail = node->prev;
    } else {
        node->next->prev = node->prev;
    }
    free(node->data);
}

void displayNode(Node *node)
{
    if (node == NULL) {
        printf("{ EMPTY }");
    } else {
        printf("{");
        if (node->prev == NULL) {
            printf(" NULL ");
        } else {
            printf(" &%s ", node->prev->data);
        }
        printf(" %s ", node->data);
        if (node->next == NULL) {
            printf(" NULL ");
        } else {
            printf(" &%s ", node->next->data);
        }
        printf("}");
    }
}

void displayList(Linkedlist *list)
{
    if (list == NULL || list->head == NULL) {
        printf("Empty list\n");

        return;
    }
    Node *node = list->head;
    while (node != NULL) {
        displayNode(node);
        node = node->next;
    }
    printf("\n");
}
