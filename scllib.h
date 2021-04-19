#ifndef _SCL_H
#define _SCL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define True 1
#define False 0
/*
#define FORMAT(x) _Generic((x),\
        char : "%c",    unsigned char : "%c"\
          int: "%d",       unsigned int: "%u",\
         long: "%ld",      unsigned long: "%lu",\
    long long: "%lld", unsigned long long: "%llu",\
        float: "%f",             double: "%lf",\
  long double: "%llf",                       \
      default: "%p")
*/
#define is_pointer(x) _Generic((x),\
          int: 0,       unsigned int: 0,\
         long: 0,      unsigned long: 0,\
    long long: 0, unsigned long long: 0,\
        float: 0,             double: 0,\
  long double: 0,                       \
      default: 1)

#define REGISTER_CREATE(type_t, scl_name)                           \
scl_name * scl_name##_create_node(type_t data){                     \
    scl_name * new_node = (scl_name *) malloc(sizeof(scl_name));    \
    if (new_node != NULL) {                                         \
        new_node->next = NULL;                                      \
        new_node->info = data;                                      \
    }                                                               \
    return new_node;                                                \
}                                                                   \
                                                                    \
scl_name * scl_name##_create_cb( void (* callback)(scl_name *) ){   \
    scl_name * new_node = (scl_name *) malloc(sizeof(scl_name));    \
    if (new_node != NULL) {                                         \
        callback(new_node);                                         \
        new_node->next = NULL;                                      \
    }                                                               \
    return new_node;                                                \
}                                                                   \
                                                                    \
unsigned int scl_name##_ncreate(scl_name ** head_ptr, unsigned int n, type_t data) {\
                                                                    \
    if (head_ptr == NULL)                                               \
        return 0;                                                   \
    scl_name * prev = scl_name##_create_node(data);                 \
                                                                    \
    unsigned int ind = 1;                                           \
                                                                    \
    if (prev == NULL)                                               \
        return 0;                                                   \
                                                                    \
    ( * head_ptr) = prev;                                           \
                                                                    \
    for ( ; ind < n ; ind++ ) {                                       \
        prev->next = scl_name##_create_node(data);                  \
                                                                    \
        if (prev->next == NULL)                                     \
            return ind;                                             \
                                                                    \
        prev->info = data;                                          \
        prev = prev->next;                                          \
    }                                                               \
                                                                    \
    return ind;                                                     \
}                                                                   \
                                                                    \
unsigned int scl_name##_ncreate_cb(scl_name ** head_ptr, unsigned int n, void (* callback)(scl_name * )) {\
                                                                    \
    if (head_ptr == NULL)                                               \
        return 0;                                                   \
    scl_name * prev = scl_name##_create_cb(callback);                  \
                                                                    \
    unsigned int ind = 1;                                           \
                                                                    \
    if (prev == NULL)                                               \
        return 0;                                                   \
                                                                    \
    ( * head_ptr) = prev;                                           \
                                                                    \
    for ( ; ind < n ; ind++ ) {                                     \
        prev->next = scl_name##_create_cb(callback);                  \
                                                                    \
        if (prev->next == NULL)                                     \
            return ind;                                             \
                                                                    \
        prev = prev->next;                                           \
        prev->next = NULL;                                          \
    }                                                               \
                                                                    \
    return ind;                                                     \
}                                                                   \


#define REGISTER_POP(type_t, scl_name) \
void  _##scl_name##_popnode_recursive(scl_name * node, scl_name ** prev) { \
                                                                            \
    if (node == NULL)                                      \
        return;                                                             \
                                                                            \
    if ( node->next != NULL){                                            \
        (*prev) = node;                                                  \
        _##scl_name##_popnode_recursive( node->next , prev);                 \
    }else{                                                                  \
        (*prev)->next = NULL;                                               \
        if (is_pointer(node->info) && (void *)node->info != NULL )                  \
            free((void *)node->info);                                               \
        free( node );                                                        \
    }                                                                       \
                                                                            \
    return;                                                                 \
}                                                                           \
                                                                            \
void scl_name##_pop_rec(scl_name ** head) {                             \
    scl_name * prev;                                                        \
                                                                            \
    prev = (* head);                                                     \
                                                                            \
    if (prev->next == NULL){                                                \
        *head = NULL;                                                       \
        if (is_pointer(prev->info) && (void *)prev->info != NULL )                  \
            free((void *)prev->info);                                               \
        free(prev);                                                         \
        return;                                                             \
    }                                                                       \
                                                                            \
    _##scl_name##_popnode_recursive( *head, &prev);                         \
}                                                                           \


#define REGISTER_APPEND(type_t, scl_name) \
void  _##scl_name##_appendnode_recursive(scl_name ** node, scl_name * new_node) {  \
                                                                            \
    if (*node == NULL || node == NULL)                                      \
        return;                                                             \
                                                                            \
    if ((*node)->next == NULL){                                             \
        (*node)->next = new_node;                                           \
    } else {                                                                \
        _##scl_name##_appendnode_recursive(&((* node)->next), new_node);                 \
    }                                                                       \
                                                                            \
    return;                                                                 \
}                                                                           \
                                                                            \
int scl_name##_append_rec(scl_name ** node, type_t data){                   \
    if (*node == NULL || node == NULL)                                      \
        return False;                                                       \
                                                                            \
    scl_name * new = scl_name##_create_node(data);                          \
    if (new == NULL)                                                        \
        return False;                                                       \
                                                                            \
    _##scl_name##_appendnode_recursive(node, new);                          \
                                                                            \
    return True;                                                            \
}                                                                           \


#define REGISTER_INSERT(type_t, scl_name)\
void _##scl_name##_insertnode_recursive(scl_name * node, scl_name * prev,  scl_name * new, unsigned int * index){\
    if (node == NULL)                                                       \
        return;                                                             \
                                                                            \
    if ( *index > 0 ){                                                      \
        *index = (*index) - 1;                                              \
        _##scl_name##_insertnode_recursive(node->next, node, new, index );  \
    } else {                                                                \
        new->next = node;                                                   \
        prev->next = new;                                                   \
                                                                            \
    }                                                                       \
    return;                                                                 \
}                                                                           \
                                                                            \
int scl_name##_insertnode_rec(scl_name ** head, scl_name * new, unsigned int index) {\
    unsigned int i = index;                                                 \
    scl_name * prev = *head;                                                 \
    if (*head == NULL || head == NULL)                                      \
        return False;                                                       \
                                                                            \
    if (index == 0){                                                        \
        new->next = *head;                                                  \
        *head = new;                                                        \
    }else                                                                   \
        _##scl_name##_insertnode_recursive(*head, prev, new, &i);           \
                                                                            \
    return True;                                                            \
}                                                                           \
                                                                            \
int scl_name##_insert_rec(scl_name ** head, type_t data, unsigned int index) {\
    if (*head == NULL || head == NULL)                                      \
        return False;                                                       \
                                                                            \
    scl_name * new = scl_name##_create_node(data);                          \
    if (new == NULL)                                                        \
        return False;                                                       \
    return scl_name##_insertnode_rec(head, new, index);                     \
}                                                                           \

#define REGISTER_REPLACE(type_t, scl_name)\
void _##scl_name##_replacenode_recursive(scl_name * node, scl_name * prev,  scl_name * new, unsigned int * index){\
    if (node == NULL)                                                       \
        return;                                                             \
                                                                            \
    if ( *index > 0 ){                                                      \
        *index = (*index) - 1;                                              \
        _##scl_name##_replacenode_recursive(node->next, node, new, index );  \
    } else {                                                                \
        new->next = node->next;                                                   \
        prev->next = new;                                                   \
        if (is_pointer(node->info) && (void *)node->info != NULL )                  \
            free((void *)node->info);                                               \
        free(node);                                                         \
                                                                            \
    }                                                                       \
    return;                                                                 \
}                                                                           \
                                                                            \
int scl_name##_replacenode_rec(scl_name ** head, scl_name * new, unsigned int index) {\
    unsigned int i = index;                                                 \
    scl_name * prev = *head;                                                 \
    if (*head == NULL || head == NULL)                                      \
        return False;                                                       \
                                                                            \
    if (index == 0){                                                        \
        new->next = (*head)->next;                                                  \
        if (is_pointer((*head)->info) && (*head)->info != NULL )                  \
            free((*head)->info);                                               \
        free(*head);                                                        \
        *head = new;                                                        \
    }else                                                                   \
        _##scl_name##_replacenode_recursive(*head, prev, new, &i);           \
                                                                            \
    return True;                                                            \
}                                                                           \
                                                                            \
int scl_name##_replace_rec(scl_name ** head, type_t data, unsigned int index) {\
    if (*head == NULL || head == NULL)                                      \
        return False;                                                       \
                                                                            \
    scl_name * new = scl_name##_create_node(data);                          \
    if (new == NULL)                                                        \
        return False;                                                       \
    return scl_name##_replacenode_rec(head, new, index);                     \
}                                                                           \

#define REGISTER_DELETE(type_t, scl_name)\
void _##scl_name##_deletenode_recursive(scl_name * node, scl_name * prev, unsigned int * index) {\
                                                                            \
    if (node == NULL)                                                       \
        return;                                                             \
                                                                            \
    if ( *index > 0 ){                                                      \
        *index = (*index) - 1;                                              \
        _##scl_name##_deletenode_recursive(node->next, node, index );       \
    } else {                                                                \
        prev->next = node->next;                                            \
        if (is_pointer(node->info) && (void *)node->info != NULL )                  \
            free((void *)node->info);                                               \
        free(node);                                                         \
                                                                            \
    }                                                                       \
    return;                                                                 \
}                                                                           \
                                                                            \
int scl_name##_delete_rec(scl_name ** head, unsigned int index) {           \
    if (*head == NULL || head == NULL)                                      \
        return False;                                                       \
                                                                            \
    if (index == 0) {                                                       \
        *head = (*head)->next;                                              \
        if (is_pointer((*head)->info) && (*head)->info != NULL )            \
            free((*head)->info);                                            \
        free(*head);                                                        \
    } else {                                                                \
        scl_name * prev = *head;                                            \
        unsigned int i = index;                                             \
        _##scl_name##_deletenode_recursive(*head, prev, &i );               \
    }                                                                       \
                                                                            \
    return True;                                                            \
}                                                                           \

#define REGISTER_INFO(type_t, scl_name)\
size_t scl_name##_len(scl_name ** head) {                                   \
    if (head == NULL || *head == NULL)                                      \
        return -1;                                                          \
                                                                            \
    scl_name * node = *head;                                                \
    int index = 0;                                                          \
                                                                            \
    for (; node != NULL ; index++){                                                               \
        node = node->next;                                                  \
    }                                                                       \
    return index;                                                           \
}                                                                           \
                                                                            \
void scl_name##_print(scl_name ** head) {                                    \
    if (head == NULL || *head == NULL)                                      \
        return;                                                             \
                                                                            \
    scl_name * node = *head;                                                \
    int index = 0;                                                          \
                                                                            \
    printf("[ SCL ] Data type: `"#type_t "` Len: %d\n", scl_name##_len(head));\
    for (; node != NULL ; index++){                                         \
        printf("[ NODE %d ] (ME) %p -> %p (NEXT) \n", index, node, node->next); \
        node = node->next;                                                  \
    }                                                                       \
    putchar('\n');                                                          \
}                                                                           \

#define REGISTER_LOCATE(type_t, scl_name)   \
int scl_name##_find(scl_name ** head, type_t value) {                       \
    if (head == NULL || *head == NULL)                                      \
        return;                                                             \
                                                                            \
    scl_name * node = *head;                                                \
    int index = 0;                                                          \
    for (; node != NULL ; index++) {                                        \
        if (node->info == value)                                            \
            return index;                                                   \
        node = node->next;                                                  \
    }                                                                       \
    return -1;                                                              \
}                                                                           \
                                                                            \
int scl_name##_find_cb(scl_name ** head, type_t value, void (* callback)(scl_name *)) {                       \
    if (head == NULL || *head == NULL)                                      \
        return;                                                             \
                                                                            \
    scl_name * node = *head;                                                \
    int index = 0;                                                          \
    for (; node != NULL ; index++) {                                        \
        if (node->info == value){                                            \
            callback(node);                                                 \
            return index;                                                   \
        }                                                                   \
        node = node->next;                                                  \
    }                                                                       \
    return -1;                                                              \
}                                                                           \
                                                                            \
scl_name * scl_name##_get_node(scl_name ** head, unsigned int index) {      \
    if (head == NULL || *head == NULL)                                      \
        return;                                                             \
                                                                            \
    scl_name * node = *head;                                                \
    for (;node != NULL && index > 0; index--) {                             \
        node = node->next;                                                  \
    }                                                                       \
    return node;                                                            \
}                                                                           \
                                                                            \
type_t * scl_name##_get(scl_name ** head, scl_name ** buffer , unsigned int index) {             \
    if (head == NULL || *head == NULL)                                      \
        return;                                                             \
                                                                            \
    scl_name * node = *head;                                                \
    for (;node != NULL && index > 0; index--) {                             \
        node = node->next;                                                  \
    }                                                                       \
    if (buffer != NULL)                                                       \
        *buffer = node;                                               \
    return node->info;                                                             \
}                                                                           \

#define DEFINE_SCL_TYPE(type_t, name) \
struct scl_##name {         \
    type_t info;            \
    struct scl_##name * next;      \
};                          \
typedef struct scl_##name scl_##name;\
REGISTER_CREATE(type_t, scl_##name);       \
REGISTER_POP(type_t, scl_##name);       \
REGISTER_APPEND(type_t, scl_##name);       \
REGISTER_INSERT(type_t, scl_##name);\
REGISTER_REPLACE(type_t, scl_##name);\
REGISTER_DELETE(type_t, scl_##name);\
REGISTER_INFO(type_t, scl_##name);\
REGISTER_LOCATE(type_t, scl_##name);\

#endif