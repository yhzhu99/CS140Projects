#include "threads/synch.h"
#include <stdbool.h>
struct frame
{
    struct lock frame_lock;
    void *base;
    struct page *page;
};

void frame_init(void);

struct frame * alloc_frame(struct page*);

void lock_frame(struct page*);
void unlock_frame(struct frame*);
void free_frame(struct frame*);