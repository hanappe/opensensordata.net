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

#ifndef _LIST_H
#define _LIST_H

typedef struct _list_t list_t;

struct _list_t
{
        void* data;
        list_t* next;
};

typedef int (*compare_func_t)(void* a, void* b);

list_t* new_list(void* data);
void delete_list(list_t *list);
void delete1_list(list_t *list);
list_t* list_sort(list_t *list, compare_func_t compare_func);
list_t* list_append(list_t *list, void* data);
list_t* list_prepend(list_t *list, void* data);
list_t* list_remove(list_t *list, void* data);
list_t* list_remove_link(list_t *list, list_t *llink);
list_t* list_nth(list_t *list, int n);
list_t* list_last(list_t *list);
list_t* list_insert_at(list_t *list, int n, void* data);
list_t* list_insert_ordered(list_t *list, void* data, compare_func_t compare_func);
int list_size(list_t *list);

#define list_next(slist)	((slist) ? (((list_t *)(slist))->next) : NULL)
#define list_get(slist)	((slist) ? ((slist)->data) : NULL)

int list_str_compare_func (void *a, void *b);

#endif  /* _LIST_H */
