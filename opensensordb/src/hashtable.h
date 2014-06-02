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
 * Demolished by Peter Hanappe [December 2002]
 *
 * - only string as key
 * - stores additional type info
 * - removed use of GLib types (gpointer, gint, ...)
 * - reduced the number of API functions
 * - changed names to fluid_hashtable_...
 */

/*
 * Copied from fluidsynth by Peter Hanappe [May 2014]
 *
 * - only id_t as key
 * - stores additional type info
 * - changed names to hashtable_...
 */

#ifndef _HASHTABLE_H
#define _HASHTABLE_H

typedef int (*hashtable_iter_t)(id_t id, void* value, void* data);
typedef void (*hashtable_delete_t)(void* value);

hashtable_t* new_hashtable(hashtable_delete_t delete);
void delete_hashtable(hashtable_t *table);

void hashtable_insert(hashtable_t *table, id_t id, void* value);

void hashtable_replace(hashtable_t *table, id_t id, void* value);

/* Returns non-zero if found, 0 if not found */
int hashtable_lookup(hashtable_t *table, id_t id, void** value);

/* Returns non-zero if removed, 0 if not removed */
int hashtable_remove(hashtable_t *table, id_t id);

void hashtable_foreach(hashtable_t *table, hashtable_iter_t fun, void* data);

unsigned int hashtable_size(hashtable_t *table);

#endif /* _HASHTABLE_H */

