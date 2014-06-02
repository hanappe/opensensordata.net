/* GLIB - Library of useful routines for C programming
 * Copyright (C) 1995-1997  Peter Mattis, Spencer Kimball and Josh MacDonald
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/*
 * Modified by the GLib Team and others 1997-2000.  See the AUTHORS
 * file for a list of people on the GLib Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GLib at ftp://ftp.gtk.org/pub/gtk/. 
 */

/* 
 * MT safe
 */

#include "osdb_priv.h"

#define HASHTABLE_MIN_SIZE 7
#define HASHTABLE_MAX_SIZE 13845163

typedef struct _hashnode_t hashnode_t;

struct _hashnode_t {
        id_t id;
        void* value;
        int type;
        hashnode_t *next;
};

static hashnode_t* new_hashnode(id_t id, void* value);
static void delete_hashnode(hashnode_t *hashnode, hashtable_delete_t del);
static void delete_hashnodes(hashnode_t *hashnode, hashtable_delete_t del);

struct _hashtable_t {
        unsigned int size;
        unsigned int nnodes;
        hashnode_t **nodes;
        hashtable_delete_t del;
};

#define HASHTABLE_RESIZE(table)			\
        if ((3 * table->size <= table->nnodes) 	        \
            && (table->size < HASHTABLE_MAX_SIZE)) {        \
                hashtable_resize(table);          \
        } 

static void hashtable_resize(hashtable_t *table);
static hashnode_t** hashtable_lookup_node(hashtable_t *table, id_t id);

hashtable_t* new_hashtable(hashtable_delete_t del)
{
        hashtable_t *table;
        unsigned int i;

        table = (hashtable_t *) malloc(sizeof(hashtable_t));
        if (table == NULL) {
                log_err("new_hashtable: out of memory");
                return NULL;
        }
        table->size = HASHTABLE_MIN_SIZE;
        table->nnodes = 0;
        table->nodes = (hashnode_t**) malloc(table->size * sizeof(hashnode_t*));
        if (table->nodes == NULL) {
                log_err("new_hashtable: out of memory");
                free(table);
                return NULL;
        }
        table->del = del;

        for (i = 0; i < table->size; i++)
                table->nodes[i] = NULL;

        return table;
}

void delete_hashtable(hashtable_t *table)
{
        unsigned int i;
  
        if (table == NULL)
                return;
  
        for (i = 0; i < table->size; i++)
                delete_hashnodes(table->nodes[i], table->del);

        free(table->nodes);
        free(table);
}

static hashnode_t** hashtable_lookup_node(hashtable_t* table, id_t id)
{
        hashnode_t **node;
  
        node = &table->nodes[id % table->size];
  
        while (*node && ((*node)->id != id))
                node = &(*node)->next;
  
        return node;
}

int hashtable_lookup(hashtable_t *table, id_t id, void** value)
{
        hashnode_t *node;
  
        node = *hashtable_lookup_node(table, id);
        if (node) {
                if (value)
                        *value = node->value;
                return 1;
        } else {
                return 0;
        }
}

void hashtable_insert(hashtable_t *table, id_t id, void* value)
{
        hashnode_t **node;
  
        node = hashtable_lookup_node(table, id);
  
        if (*node) {
                (*node)->value = value;
        } else {
                *node = new_hashnode(id, value);
                table->nnodes++;
                HASHTABLE_RESIZE(table);
        }
}

void hashtable_replace(hashtable_t *table, id_t id, void* value)
{
        hashnode_t **node;
  
        node = hashtable_lookup_node(table, id);
  
        if (*node) {
                if (table->del) {
                        table->del((*node)->value);
                }
                (*node)->value = value;

        } else {
                *node = new_hashnode(id, value);
                table->nnodes++;
                HASHTABLE_RESIZE(table);
        }
}

int hashtable_remove(hashtable_t *table, id_t id)
{
        hashnode_t **node, *dest;
  
        node = hashtable_lookup_node(table, id);
        if (*node) {
                dest = *node;
                (*node) = dest->next;
                delete_hashnode(dest, table->del);
                table->nnodes--;
                HASHTABLE_RESIZE(table);
                return 1;
        }

        return 0;
}

void hashtable_foreach(hashtable_t *table, hashtable_iter_t func, void* data)
{
        hashnode_t *node = NULL;
        unsigned int i;

        for (i = 0; i < table->size; i++) {
                for (node = table->nodes[i]; node != NULL; node = node->next) {
                        (*func)(node->id, node->value, data);
                }
        }
}

unsigned int hashtable_size(hashtable_t *table)
{
        return table->nnodes;
}

static void hashtable_resize(hashtable_t *table)
{
        hashnode_t **new_nodes;
        hashnode_t *node;
        hashnode_t *next;
        unsigned int hash_val;
        int new_size;
        unsigned int i;

        new_size = 3 * table->size + 1;
        new_size = (new_size > HASHTABLE_MAX_SIZE)? HASHTABLE_MAX_SIZE : new_size;

        /*   printf("%s: %d: resizing, new size = %d\n", __FILE__, __LINE__, new_size); */
 
        new_nodes = (hashnode_t**) malloc(new_size * sizeof(hashnode_t*));
        if (new_nodes == NULL) {
                log_err("hashtable_resize: out of memory");
                return;
        }
        memset(new_nodes, 0, new_size * sizeof(hashnode_t*));

        for (i = 0; i < table->size; i++) {
                for (node = table->nodes[i]; node; node = next) {
                        next = node->next;
                        hash_val = node->id % new_size;      
                        node->next = new_nodes[hash_val];
                        new_nodes[hash_val] = node;
                }
        }

        free(table->nodes);
        table->nodes = new_nodes;
        table->size = new_size;
}

static hashnode_t* new_hashnode(id_t id, void* value)
{
        hashnode_t *hashnode;
  
        hashnode = (hashnode_t*) malloc(sizeof(hashnode_t));
        if (hashnode == NULL) {
                log_err("new_hashnode: out of memory");
                return NULL;
        }

        hashnode->id = id;
        hashnode->value = value;
        hashnode->next = NULL;
  
        return hashnode;
}

static void delete_hashnode(hashnode_t *hashnode, hashtable_delete_t del)
{
        if (del)
                (*del)(hashnode->value);
        free(hashnode);
}

static void delete_hashnodes(hashnode_t *hashnode, hashtable_delete_t del)
{
        while (hashnode) {
                hashnode_t *next = hashnode->next;
                delete_hashnode(hashnode, del);
                hashnode = next;
        }  
}

