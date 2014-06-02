/* GLIB - Library of useful routines for C programming
 * Copyright (C) 1995-1997  Peter Mattis, Spencer Kimball and Josh MacDonald
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02110-1301, USA.
 */

/*
 * Modified by the GLib Team and others 1997-1999.  See the AUTHORS
 * file for a list of people on the GLib Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GLib at ftp://ftp.gtk.org/pub/gtk/.
 */

#include "osdb_priv.h"

list_t* new_list(void* data)
{
        list_t* list = (list_t*) malloc(sizeof(list_t));
        if (list == NULL) {
                log_err("new_list: out of memory");
                return NULL;
        }
        list->data = data;
        list->next = NULL;
        return list;
}

void delete_list(list_t *list)
{
        list_t *next;
        while (list) {
                next = list->next;
                free(list);
                list = next;
        }
}

void delete1_list(list_t *list)
{
        if (list) {
                free(list);
        }
}

list_t* list_append(list_t *list, void*  data)
{
        list_t *newlist;
        list_t *last;

        newlist = new_list(NULL);
        if (newlist == NULL)
                return NULL;
        newlist->data = data;

        if (list) {
                last = list_last(list);
                /* g_assert (last != NULL); */
                last->next = newlist;

                return list;
        }
        else
                return newlist;
}

list_t* list_prepend(list_t *list, void* data)
{
        list_t *newlist;

        newlist = new_list(data);
        newlist->next = list;

        return newlist;
}

list_t* list_nth(list_t *list, int n)
{
        while ((n-- > 0) && list) {
                list = list->next;
        }

        return list;
}

list_t* list_remove(list_t *list, void* data)
{
        list_t *tmp;
        list_t *prev;

        prev = NULL;
        tmp = list;

        while (tmp) {
                if (tmp->data == data) {
                        if (prev) {
                                prev->next = tmp->next;
                        }
                        if (list == tmp) {
                                list = list->next;
                        }
                        tmp->next = NULL;
                        delete_list(tmp);

                        break;
                }

                prev = tmp;
                tmp = tmp->next;
        }

        return list;
}

list_t* list_remove_link(list_t *list, list_t *link)
{
        list_t *tmp;
        list_t *prev;

        prev = NULL;
        tmp = list;

        while (tmp) {
                if (tmp == link) {
                        if (prev) {
                                prev->next = tmp->next;
                        }
                        if (list == tmp) {
                                list = list->next;
                        }
                        tmp->next = NULL;
                        break;
                }

                prev = tmp;
                tmp = tmp->next;
        }

        return list;
}

static list_t* list_sort_merge(list_t *l1, list_t *l2, compare_func_t compare_func)
{
        list_t list, *l;

        l = &list;

        while (l1 && l2) {
                if (compare_func(l1->data,l2->data) < 0) {
                        l = l->next = l1;
                        l1 = l1->next;
                } else {
                        l = l->next = l2;
                        l2 = l2->next;
                }
        }
        l->next= l1 ? l1 : l2;

        return list.next;
}

list_t* list_sort(list_t *list, compare_func_t compare_func)
{
        list_t *l1, *l2;

        if (!list) {
                return NULL;
        }
        if (!list->next) {
                return list;
        }

        l1 = list;
        l2 = list->next;

        while ((l2 = l2->next) != NULL) {
                if ((l2 = l2->next) == NULL)
                        break;
                l1=l1->next;
        }
        l2 = l1->next;
        l1->next = NULL;

        return list_sort_merge(list_sort(list, compare_func),
                               list_sort(l2, compare_func),
                               compare_func);
}


list_t* list_last(list_t *list)
{
        if (list) {
                while (list->next)
                        list = list->next;
        }

        return list;
}

int list_size(list_t *list)
{
        int n = 0;
        while (list) {
                n++;
                list = list->next;
        }
        return n;
}

list_t* list_insert_at(list_t *list, int n, void* data)
{
        list_t *newlist;
        list_t *cur;
        list_t *prev = NULL;

        newlist = new_list(data);

        cur = list;
        while ((n-- > 0) && cur) {
                prev = cur;
                cur = cur->next;
        }

        newlist->next = cur;

        if (prev) {
                prev->next = newlist;
                return list;
        } else {
                return newlist;
        }
}

/* Compare function to sort strings alphabetically,
 * for use with list_sort(). */
int list_str_compare_func (void *a, void *b)
{
        if (a && b) return strcmp((char *)a, (char *)b);
        if (!a && !b) return 0;
        if (a) return -1;
        return 1;
}

list_t* list_insert_ordered(list_t *list, void* data, compare_func_t compare_func)
{
        list_t* newlist = new_list(data);
        if (newlist == NULL)
                return NULL;

        if (list == NULL)
                return newlist;
  
        if (compare_func(data, list->data) < 0) {
                newlist->next = list;
                return newlist;
        }
        
        list_t* prev = list;
        list_t* cur = list->next;

        while (cur != NULL) {
                if (compare_func(data, cur->data) < 0) {
                        prev->next = newlist;
                        newlist->next = cur;
                        return list;
                }
                prev = cur;
                cur = cur->next;
        }  

        prev->next = newlist;
        return list;
}

