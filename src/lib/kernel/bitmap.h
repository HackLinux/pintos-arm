#ifndef __LIB_KERNEL_BITMAP_H
#define __LIB_KERNEL_BITMAP_H

#include <stdbool.h>
#include <stddef.h>

/* Bitmap abstract data type. */

struct bitmap *bitmap_create (size_t bit_cnt);
void bitmap_destroy (struct bitmap *);

size_t bitmap_size (const struct bitmap *);

void bitmap_set (struct bitmap *, size_t idx, bool);
void bitmap_set_all (struct bitmap *, bool);

void bitmap_mark (struct bitmap *, size_t idx);
void bitmap_reset (struct bitmap *, size_t idx);
void bitmap_flip (struct bitmap *, size_t idx);

bool bitmap_test (const struct bitmap *, size_t idx);

#define BITMAP_ERROR ((size_t) -1)
size_t bitmap_scan (const struct bitmap *, bool);
size_t bitmap_find_and_set (struct bitmap *);
size_t bitmap_find_and_clear (struct bitmap *);

size_t bitmap_set_cnt (const struct bitmap *);
bool bitmap_clear_cnt (const struct bitmap *);

bool bitmap_any (const struct bitmap *);
bool bitmap_none (const struct bitmap *);
bool bitmap_all (const struct bitmap *);

#ifdef FILESYS
struct file;
size_t bitmap_file_size (const struct bitmap *);
void bitmap_read (struct bitmap *, struct file *);
void bitmap_write (const struct bitmap *, struct file *);
#endif

void bitmap_dump (const struct bitmap *);

#endif /* lib/kernel/bitmap.h */