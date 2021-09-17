#include "main.h"

#include <stdio.h>
#include <stdlib.h>

void copy_string(char **dest, char *src)
{
    int i = 0;

    *dest = (char *) malloc(sizeof(char));

    while (*src)
    {
        (*dest)[i] = *src;

        src++;
        i++;

        *dest = (char *) realloc(*dest, sizeof(char) * (i + 1));
    }

    (*dest)[i] = '\0';
}

Apartment *find_node_at_ith_index_apt(Apartment *head, int index)
{
    while (index)
    {
        head = head->next;

        index--;
    }

    return head;
}

Apartment *find_last_node_apt(Apartment *head)
{
    Apartment *real_head = head;

    while (head->next != real_head)
    {
        head = head->next;
    }

    return head;
}

int find_string_length(char *str)
{
    int i = 0;

    while (*str)
    {
        i++;
        str++;
    }

    return i;
}

Flat *find_node_at_ith_index_flat(Flat *head, int index)
{
    while (index)
    {
        head = head->next;

        index--;
    }

    return head;
}

int compare_string(char *str1, char *str2)
{
    if (find_string_length(str1) != find_string_length(str2))
    {
        return 0;
    }

    while (*str1)
    {
        if (*str1 != *str2)
        {
            return 0;
        }

        str1++;
        str2++;
    }

    return 1;
}

int calculate_used_bandwidth_apt(Apartment *head)
{
    int used_bandwidth = 0;

    Flat *flat_list = head->flat_list;

    while (flat_list)
    {
        used_bandwidth += flat_list->initial_bandwidth;

        flat_list = flat_list->next;
    }

    return used_bandwidth;
}

Apartment *find_apt_with_name(Apartment *head, char *name)
{
    while (head)
    {
        if (compare_string(head->name, name))
        {
            return head;
        }

        head = head->next;
    }

    return NULL;
}

void free_memory_flat(Flat *flat_list)
{
    Flat *temp;

    while (flat_list)
    {
        temp = flat_list;
        flat_list = flat_list->next;
        free(temp);
    }
}

void free_memory_apt(Apartment *apt)
{
    free_memory_flat(apt->flat_list);
    free(apt);   
}

Flat *find_flat_with_id(Flat *flat_list, int id)
{
    while (flat_list)
    {
        if (flat_list->id == id)
        {
            return flat_list;
        }

        flat_list = flat_list->next;
    }

    return NULL;
}

Flat *find_last_node_flat(Flat *head)
{
    while (head->next)
    {
        head = head->next;
    }

    return head;
}

Apartment *find_apt_with_flat_id(Apartment *head, int flat_id)
{
    while (head)
    {
        if (find_flat_with_id(head->flat_list, flat_id))
        {
            return head;
        }

        head = head->next;
    }

    return NULL;
}

int find_flat_index(Flat *flat_list, int flat_id)
{
    int i = 0;

    while (flat_list)
    {
        if (flat_list->id == flat_id)
        {
            break;
        }

        i++;
        flat_list = flat_list->next;
    }

    return i;
}

void insert_flat(Apartment *head, Flat *new_flat, int index)
{
    Flat *prev_flat;

    if (!head->flat_list)
    {
        head->flat_list = new_flat;
        head->flat_list->next = NULL;
        head->flat_list->prev = NULL;
    }
    else if (index == 0)
    {
        new_flat->prev = NULL;
        new_flat->next = head->flat_list;
        head->flat_list->prev = new_flat;
        head->flat_list = new_flat;
    }
    else
    {
        prev_flat = find_node_at_ith_index_flat(head->flat_list, index - 1);
        new_flat->next = prev_flat->next;
        new_flat->prev = prev_flat;

        if (prev_flat->next)
        {
            prev_flat->next->prev = new_flat;
        }

        prev_flat->next = new_flat;
    }
}

Apartment* add_apartment(Apartment* head, int index, char* apartment_name, int max_bandwidth)
{
    Apartment *new_apt = (Apartment *) malloc(sizeof(Apartment)), *prev_apt;

    new_apt->flat_list = NULL;
    new_apt->max_bandwidth = max_bandwidth;
    copy_string(&new_apt->name, apartment_name);

    if (!head)
    {
        head = new_apt;
        head->next = head;

        return head;
    }
    else if (index == 0)
    {
        prev_apt = find_last_node_apt(head);

        new_apt->next = head;

        prev_apt->next = new_apt;

        return new_apt;
    }
    else
    {
        prev_apt = find_node_at_ith_index_apt(head, index - 1);

        new_apt->next = prev_apt->next;
        prev_apt->next = new_apt;

        return head;        
    }
}

void add_flat(Apartment* head, char* apartment_name, int index, int flat_id, int initial_bandwidth)
{
    int available_bandwidth;
    Flat *new_flat;

    head = find_apt_with_name(head, apartment_name);

    available_bandwidth = head->max_bandwidth - calculate_used_bandwidth_apt(head);

    if (initial_bandwidth > available_bandwidth)
    {
        initial_bandwidth = available_bandwidth;
    }

    new_flat = (Flat *) malloc(sizeof(Flat));
    new_flat->is_empty = 0;
    new_flat->id = flat_id;
    new_flat->initial_bandwidth = initial_bandwidth;

    insert_flat(head, new_flat, index);
}

Apartment* remove_apartment(Apartment* head, char* apartment_name)
{
    Apartment *apt_to_be_deleted = find_apt_with_name(head, apartment_name), *apt_to_be_freed;
    Flat *flat_list_to_be_freed;

    if (apt_to_be_deleted->next == apt_to_be_deleted)
    {
        head = NULL;
    }
    else
    {
        if (apt_to_be_deleted->next == head)
        {
            head = apt_to_be_deleted;
        }

        flat_list_to_be_freed = apt_to_be_deleted->flat_list;
        apt_to_be_freed = apt_to_be_deleted->next;
        *apt_to_be_deleted = *apt_to_be_deleted->next;
        apt_to_be_freed->flat_list = flat_list_to_be_freed;
    }

    free_memory_apt(apt_to_be_freed);

    return head;
}

void make_flat_empty(Apartment* head, char* apartment_name, int flat_id)
{
    Flat *flat;
    
    head = find_apt_with_name(head, apartment_name);
    flat = find_flat_with_id(head->flat_list, flat_id);

    flat->is_empty = 1;
    flat->initial_bandwidth = 0;
}

int find_sum_of_max_bandwidths(Apartment* head)
{
    int sum = 0;
    Apartment *curr_apt = head;

    if (head)
    {
        do
        {
            sum += curr_apt->max_bandwidth;

            curr_apt = curr_apt->next;
        }
        while (curr_apt != head);
    }

    return sum;
}

Apartment* merge_two_apartments(Apartment* head, char* apartment_name_1, char* apartment_name_2)
{
    Apartment *apt1 = find_apt_with_name(head, apartment_name_1);
    Apartment *apt2 = find_apt_with_name(head, apartment_name_2);
    Apartment *apt_to_be_freed;
    Flat *last_flat_of_apt1;

    apt1->max_bandwidth += apt2->max_bandwidth;

    if (!apt1->flat_list && apt2->flat_list)
    {
        apt1->flat_list = apt2->flat_list;
    }
    else if (apt1->flat_list && apt2->flat_list)
    {
        last_flat_of_apt1 = find_last_node_flat(apt1->flat_list);
        last_flat_of_apt1->next = apt2->flat_list;
        apt2->flat_list->prev = last_flat_of_apt1;
    }

    if (apt2->next == apt2)
    {
        head = NULL;
    }
    else
    {
        if (apt2->next == head)
        {
            head = apt2;
        }

        apt_to_be_freed = apt2->next;
        *apt2 = *apt2->next;
    }

    free(apt_to_be_freed);

    return head;
}

void relocate_flats_to_same_apartment(Apartment* head, char* new_apartment_name, int flat_id_to_shift, int* flat_id_list)
{
    Apartment *new_apartment = find_apt_with_name(head, new_apartment_name);
    Apartment *old_apartment;
    Flat *flat;
    int i = 0, flat_index = find_flat_index(new_apartment->flat_list, flat_id_to_shift);

    while (flat_id_list[i])
    {
        old_apartment = find_apt_with_flat_id(head, flat_id_list[i]);
        flat = find_flat_with_id(old_apartment->flat_list, flat_id_list[i]);

        old_apartment->max_bandwidth -= flat->initial_bandwidth;
        new_apartment->max_bandwidth += flat->initial_bandwidth;

        if (flat->prev)
        {
            flat->prev->next = flat->next;
        }
        else
		{
			old_apartment->flat_list = flat->next;
		}

        if (flat->next)
        {
            flat->next->prev = flat->prev;
        }

        insert_flat(new_apartment, flat, flat_index + i);

        i++;
    }
}
