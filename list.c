#include "list.h"
#include "stdlib.h"
#include "string.h"
#include "task.h"

void insert(struct node **start_node, Task *new_task)
{
    struct node *new_node = malloc(sizeof(struct node));

    new_node->task = new_task;
    new_node->next_task = *start_node;

    *start_node = new_node;
}

void delete(struct node **start_node, Task *deleting_task)
{
    struct node *temp_node;
    struct node *previous_node;

    temp_node = *start_node;

    // Если удаляемый элемент находится в начале списка
    if (strcmp(deleting_task->name, temp_node->task->name) == 0)
    {
        *start_node = (*start_node)->next_task;
    }
    else
    {
        previous_node = *start_node;
        temp_node = temp_node->next_task;

        while (strcmp(deleting_task->name, temp_node->task->name) != 0)
        {
            previous_node = temp_node;
            temp_node = temp_node->next_task;
        }

        previous_node->next_task = temp_node->next_task;
    }
}

struct node* reverse(struct node *start_node)
{
    if (start_node == NULL || start_node->next_task == NULL)
        return start_node;

    struct node *reverse_node = reverse(start_node->next_task);
    start_node->next_task->next_task = start_node;

    start_node->next_task = NULL;

    return reverse_node;
}
