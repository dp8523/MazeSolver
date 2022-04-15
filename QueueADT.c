// File: QueueADT.c
//
// Description: Implementation of QueueADT to create and utilize the data type.
//
// Author: Dara Prak  dp8523
//

#include <stdlib.h>
#include <assert.h>

#define INIT_CAP 5 ///< the initial capacity when a QueueADT is created
#define EXPAND 2 ///< multiplied by the current capacity when it must grow

/// Struct that contains all the properties of a QueueADT

struct queue_s
{
    void **content; ///< array of generic elements as void *
    int length; ///< number of elements currently stored
    int capacity; ///< maximum amount of space in content

    /// Compares elements during insertion
    ///
    /// @param a first element to be compared
    /// @param b second element to be compared
    /// @return an int indicating if the a is greater, equal or smaller than b
    int (*cmp)(const void *a, const void *b);
};

typedef struct queue_s *QueueADT;

#define _QUEUE_IMPL_

#include "QueueADT.h"

/// Create a QueueADT

QueueADT que_create(int (*cmp)(const void *a, const void *b))
{
    QueueADT queue = (QueueADT) malloc(sizeof(struct queue_s));
    queue->content = (void **) calloc(sizeof(void *), INIT_CAP);
    queue->length = 0;
    queue->capacity = INIT_CAP;
    queue->cmp = cmp;
    return queue;
}


/// Deallocate the supplied QueueADT and its contents

void que_destroy(QueueADT queue)
{
    free(queue->content);
    free(queue);
}


/// Reset the content, length, and capacity of the QueueADT

void que_clear(QueueADT queue)
{
    free(queue->content);
    queue->content = (void **) calloc(sizeof(void *), INIT_CAP);
    queue->length = 0;
    queue->capacity = INIT_CAP;
}


/// Insert the specified data into the Queue in the appropriate place

void que_insert(QueueADT queue, void *data)
{
    if(queue->length == queue->capacity)
    {
        queue->content = (void **) realloc(queue->content, sizeof(void *) *
                         queue->capacity * EXPAND);
        queue->capacity *= EXPAND;
    }

    if(queue->cmp == NULL || que_empty(queue))
    {
        queue->content[queue->length] = data;
    }
    else if(queue->cmp(data, queue->content[queue->length - 1]) >= 0)
    {
        queue->content[queue->length] = data;
    }
    else
    {
        for(int i = 0; i < queue->length; i++)
        {
            if(queue->cmp(data, queue->content[i]) < 0)
            {
                for(int j = queue->length - 1; j >= i; j--)
                {
                    queue->content[j+1] = queue->content[j];
                }
                queue->content[i] = data;
                break;
            }
        }
    }

    queue->length++;
}


/// Remove and return the first element from the QueueADT

void * que_remove(QueueADT queue)
{
    assert(!que_empty(queue));
    void *element = queue->content[0];
    for(int i = 1; i < queue->length; i++)
    {
        queue->content[i - 1] = queue->content[i];
    }
    queue->length--;
    return element;
}


/// Tell whether or not the supplied QueueADT is empty

bool que_empty(QueueADT queue)
{
    if(queue->length == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

