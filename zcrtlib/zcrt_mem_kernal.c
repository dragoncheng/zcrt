/****************************************************************************
COPYRIGHT NOTICE  
Copyright (c) 2012 - 2014  Alex Cheng
All rights reserved.  
Author: Alex Cheng

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifdef __KERNEL__

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/list.h>                 /* for list_head */
#include <linux/slab.h>


typedef struct memblock_s
{
	struct list_head        list;
	void                        *ptr;
	size_t                        size;
} memblock_t;


/*
* The list of memory blocks (one simple list)
*/
static LIST_HEAD(memblock_list);
static DEFINE_RWLOCK(memblock_lock);

static int memblock_add(void *ptr, size_t size)
{
	memblock_t *m;

	if (!(m = kmalloc(sizeof(memblock_t), GFP_KERNEL)))
		return -ENOMEM;
	m->ptr = ptr;
	m->size = size;

	write_lock(&memblock_lock);
	list_add(&m->list, &memblock_list);
	write_unlock(&memblock_lock);

	return 0;
}

static void memblock_del(memblock_t *m)
{
	write_lock(&memblock_lock);
	list_del(&m->list);
	write_unlock(&memblock_lock);
	kfree(m);
}

static memblock_t *memblock_get(void *ptr)
{
	struct list_head *l;
	memblock_t *m;

	read_lock(&memblock_lock);
	list_for_each(l, &memblock_list) {
		m = list_entry(l, memblock_t, list);
		if (m->ptr == ptr) {
			/* HIT */
			read_unlock(&memblock_lock);
			return m;
		}
	}
	read_unlock(&memblock_lock);
	return NULL;
}


void* zcrt_malloc(uint32_t size, uint32_t tag)
{
	void *ptr;
	tag=tag;

	if (!(ptr = kmalloc(size, GFP_KERNEL)))
		return NULL;
	if (memblock_add(ptr, size)) {
		kfree(ptr);
		return NULL;
	}
	return ptr;
}

void zcrt_free(void *ptr)
{
	memblock_t *m;

	if (!ptr)
		return;
	if (!(m = memblock_get(ptr))) {
		printk(KERN_ERR "bug: free non-exist memory\n");
		return;
	}
	memblock_del(m);
	kfree(ptr);
}

void *zcrt_realloc(void *ptr, size_t size, uint32_t tag)
{
	memblock_t *m;
	void *nbuf = NULL;
	tag=tag;

	if (ptr) {
		if (!(m = memblock_get(ptr))) {
			printk(KERN_ERR "bug: realloc non-exist memory\n");
			return NULL;
		}

		if (size == m->size)
			return ptr;
		if (size != 0) {
			if (!(nbuf = kmalloc(size, GFP_KERNEL)))
				return NULL;
			memmove(nbuf, ptr, m->size);
			if (memblock_add(nbuf, size)) {
				kfree(nbuf);
				return NULL;
			}
		}

		memblock_del(m);
		kfree(ptr);
	} else {
		if (size != 0) {
			if (!(nbuf = kmalloc(size, GFP_KERNEL)))
				return NULL;
			if (memblock_add(nbuf, size)) {
				kfree(nbuf);
				return NULL;
			}
		}
	}

	return nbuf;
}

#endif
