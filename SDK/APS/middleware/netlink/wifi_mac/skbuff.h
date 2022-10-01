#ifndef __SKBUFF_H__
#define __SKBUFF_H__

#include <stdlib.h>
#include <stdint.h>


#define ETH_ALEN    6       /* Octets in one ethernet addr   */
#define ETH_HLEN    14      /* Total octets in header.   */


struct ethhdr {
    unsigned char   h_dest[ETH_ALEN];   /* destination eth addr */
    unsigned char   h_source[ETH_ALEN]; /* source ether addr    */
    u16             h_proto;            /* packet type ID field ,  big-endian */
} __attribute__((packed));


struct sk_buff;

struct sk_buff {
    /* These two members must be first. */
    struct sk_buff      *next;
    struct sk_buff      *prev;

    unsigned char *head;
    unsigned char *data;
    unsigned char *tail;
    unsigned char *end;
    unsigned int  len;          /*total buffer len */
    unsigned int  data_len;
    unsigned int  ref;
};


/**
 * compare_ether_addr - Compare two Ethernet addresses
 * @addr1: Pointer to a six-byte array containing the Ethernet address
 * @addr2: Pointer other six-byte array containing the Ethernet address
 *
 * Compare two ethernet addresses, returns 0 if equal
 */
struct sk_buff_head {
    /* These two members must be first. */
    struct sk_buff *next;
    struct sk_buff *prev;
    u32 qlen;
};


/*
 * Add data to an sk_buff
 */

static inline unsigned char *__skb_put(struct sk_buff *skb, unsigned int len)
{
    unsigned char *tmp = skb->tail;
    skb->tail += len;
    skb->len += len;
    return tmp;
}

/**
 * skb_put - add data to a buffer
 * @skb: buffer to use
 * @len: amount of data to add
 *
 * This function extends the used data area of the buffer. If this would
 * exceed the total buffer size the kernel will panic. A pointer to the
 * first byte of the extra data is returned.
 */
static inline unsigned char *skb_put(struct sk_buff *skb, unsigned int len)
{
    unsigned char *tmp = skb->tail;
    skb->tail += len;
    skb->len += len;
    if (skb->tail > skb->end) {
        //skb_over_panic(skb, len, current_text_addr());
    }
    return tmp;
}

static inline unsigned char *__skb_push(struct sk_buff *skb, unsigned int len)
{
    skb->data -= len;
    skb->len += len;
    return skb->data;
}

/**
 * skb_push - add data to the start of a buffer
 * @skb: buffer to use
 * @len: amount of data to add
 *
 * This function extends the used data area of the buffer at the buffer
 * start. If this would exceed the total buffer headroom the kernel will
 * panic. A pointer to the first byte of the extra data is returned.
 */

/**
 * skb_push - add data to the start of a buffer
 * @skb: buffer to use
 * @len: amount of data to add
 *
 * This function extends the used data area of the buffer at the buffer
 * start. If this would exceed the total buffer headroom the kernel will
 * panic. A pointer to the first byte of the extra data is returned.
 */

static inline unsigned char *skb_push(struct sk_buff *skb, unsigned int len)
{
    skb->data -= len;
    skb->len += len;
    //if(skb->datahead) {
    //   skb_under_panic(skb, len, current_text_addr());
    //}
    return skb->data;
}


/**
 * skb_pull - remove data from the start of a buffer
 * @skb: buffer to use
 * @len: amount of data to remove
 *
 * This function removes data from the start of a buffer, returning
 * the memory to the headroom. A pointer to the next data in the buffer
 * is returned. Once the data has been pulled future pushes will overwrite
 * the old data.
 */

static inline unsigned char *__skb_pull(struct sk_buff *skb, unsigned int len)
{
    skb->len -= len;
    return  skb->data += len;
}

static inline unsigned char *skb_pull(struct sk_buff *skb, unsigned int len)
{
    if (len > skb->len) {
        return NULL;
    }
    return __skb_pull(skb, len);
}

/**
 * skb_headroom - bytes at buffer head
 * @skb: buffer to check
 *
 * Return the number of bytes of free space at the head of an &sk_buff.
 */

static inline int skb_headroom(const struct sk_buff *skb)
{
    return skb->data - skb->head;
}

/**
 * skb_tailroom - bytes at buffer end
 * @skb: buffer to check
 *
 * Return the number of bytes of free space at the tail of an sk_buff
 */

static inline int skb_tailroom(const struct sk_buff *skb)
{
    return skb->end - skb->tail;
}

/**
 * skb_reserve - adjust headroom
 * @skb: buffer to alter
 * @len: bytes to move
 *
 * Increase the headroom of an empty &sk_buff by reducing the tail
 * room. This is only allowed for an empty buffer.
 */

static inline void skb_reserve(struct sk_buff *skb, unsigned int len)
{
    skb->data += len;
    skb->tail += len;
}

static inline void __skb_trim(struct sk_buff *skb, unsigned int len)
{
    skb->len = len;
    skb->tail = skb->data + len;
}


/**
 * skb_trim - remove end from a buffer
 * @skb: buffer to alter
 * @len: new length
 *
 * Cut the length of a buffer down by removing data from the tail. If
 * the buffer is already under the length specified it is not modified.
 */

static inline void skb_trim(struct sk_buff *skb, unsigned int len)
{
    if (skb->len > len) {
        __skb_trim(skb, len);
    }
}


void __init_skb(struct sk_buff *skb, unsigned int size, char *data)
{
    skb->head = (unsigned char *)data;
    skb->data = (unsigned char *)data;
    skb->tail = (unsigned char *)data;
    skb->end = skb->tail + size;
    skb->len = size;
    skb->ref = 1;
}

struct sk_buff *__alloc_skb(unsigned int size)
{
    struct sk_buff *skb;
    u8 *data;

    skb = (struct sk_buff *)malloc(sizeof(*skb));
    if (!skb) {
        goto out;
    }

    //size = SKB_DATA_ALIGN(size);
    data = malloc(size);
    if (!data) {
        goto nodata;
    }

    /*
     * Only clear those fields we need to clear, not those that we will
     * actually initialise below. Hence, don't put any more fields after
     * the tail pointer in struct
     */
    //memset(skb, 0, offsetof(struct sk_buff, tail));
    //skb->truesize = size + sizeof(struct sk_buff);
    //atomic_set(&skb->users, 1);
    skb->head = data;
    skb->data = data;
    skb->tail = data;
    skb->end = skb->tail + size;
    skb->len = 0;
    skb->data_len = 0;
    skb->ref = 0;
out:
    return skb;
nodata:
    free(skb);
    skb = NULL;
    return skb;
}




void __kfree_skb(struct sk_buff *skb)
{
    if (skb && !skb->ref) {
        if (skb->head) {
            free(skb->head);
        }
        free(skb);
    }
}

static inline struct sk_buff *alloc_skb(unsigned int size)
{
    return __alloc_skb(size);
}


/**
 *  skb_queue_empty - check if a queue is empty
 *  @list: queue head
 *
 *  Returns true if the queue is empty, false otherwise.
 */
static inline int skb_queue_empty(const struct sk_buff_head *list)
{
    int rc = 0;
    if (list->next == (struct sk_buff *)list) {
        rc = 1;
    }
    return rc;
}

/**
 *  skb_queue_is_last - check if skb is the last entry in the queue
 *  @list: queue head
 *  @skb: buffer
 *
 *  Returns true if @skb is the last buffer on the list.
 */
static inline bool skb_queue_is_last(const struct sk_buff_head *list,
                                     const struct sk_buff *skb)
{
    return (skb->next == (struct sk_buff *) list);
}

/**
 *  skb_queue_is_first - check if skb is the first entry in the queue
 *  @list: queue head
 *  @skb: buffer
 *
 *  Returns true if @skb is the first buffer on the list.
 */
static inline bool skb_queue_is_first(const struct sk_buff_head *list,
                                      const struct sk_buff *skb)
{
    return (skb->prev == (struct sk_buff *) list);
}

/**
 *  skb_peek - peek at the head of an &sk_buff_head
 *  @list_: list to peek at
 *
 *  Peek an &sk_buff. Unlike most other operations you _MUST_
 *  be careful with this one. A peek leaves the buffer on the
 *  list and someone else may run off with it. You must hold
 *  the appropriate locks or have a private queue to do this.
 *
 *  Returns %NULL for an empty list or a pointer to the head element.
 *  The reference count is not incremented and the reference is therefore
 *  volatile. Use with caution.
 */
static inline struct sk_buff *skb_peek(struct sk_buff_head *list_)
{
    struct sk_buff *list = ((struct sk_buff *)list_)->next;
    if (list == (struct sk_buff *)list_) {
        list = NULL;
    }
    return list;
}

/**
 *  skb_peek_tail - peek at the tail of an &sk_buff_head
 *  @list_: list to peek at
 *
 *  Peek an &sk_buff. Unlike most other operations you _MUST_
 *  be careful with this one. A peek leaves the buffer on the
 *  list and someone else may run off with it. You must hold
 *  the appropriate locks or have a private queue to do this.
 *
 *  Returns %NULL for an empty list or a pointer to the tail element.
 *  The reference count is not incremented and the reference is therefore
 *  volatile. Use with caution.
 */
static inline struct sk_buff *skb_peek_tail(struct sk_buff_head *list_)
{
    struct sk_buff *list = ((struct sk_buff *)list_)->prev;
    if (list == (struct sk_buff *)list_) {
        list = NULL;
    }
    return list;
}

/**
 *  skb_queue_len   - get queue length
 *  @list_: list to measure
 *
 *  Return the length of an &sk_buff queue.
 */
static inline u32 skb_queue_len(const struct sk_buff_head *list_)
{
    return list_->qlen;
}

static inline void __skb_queue_head_init(struct sk_buff_head *list)
{
    list->prev = list->next = (struct sk_buff *)list;
    list->qlen = 0;
}

/*
 * This function creates a split out lock class for each invocation;
 * this is needed for now since a whole lot of users of the skb-queue
 * infrastructure in drivers have different locking usage (in hardirq)
 * than the networking core (in softirq only). In the long run either the
 * network layer or drivers should need annotation to consolidate the
 * main types of usage into 3 classes.
 */
static inline void skb_queue_head_init(struct sk_buff_head *list)
{
    //spin_lock_init(&list->lock);
    __skb_queue_head_init(list);
}

static inline void __skb_insert(struct sk_buff *newsk,
                                struct sk_buff *prev, struct sk_buff *next,
                                struct sk_buff_head *list)
{
    newsk->next = next;
    newsk->prev = prev;
    next->prev  = prev->next = newsk;
    list->qlen++;
}

/**
 *  __skb_queue_after - queue a buffer at the list head
 *  @list: list to use
 *  @prev: place after this buffer
 *  @newsk: buffer to queue
 *
 *  Queue a buffer int the middle of a list. This function takes no locks
 *  and you must therefore hold required locks before calling it.
 *
 *  A buffer cannot be placed on two lists at the same time.
 */
static inline void __skb_queue_after(struct sk_buff_head *list,
                                     struct sk_buff *prev,
                                     struct sk_buff *newsk)
{
    __skb_insert(newsk, prev, prev->next, list);
}

extern void skb_append(struct sk_buff *old, struct sk_buff *newsk,
                       struct sk_buff_head *list);

static inline void __skb_queue_before(struct sk_buff_head *list,
                                      struct sk_buff *next,
                                      struct sk_buff *newsk)
{
    __skb_insert(newsk, next->prev, next, list);
}

/**
 *  __skb_queue_head - queue a buffer at the list head
 *  @list: list to use
 *  @newsk: buffer to queue
 *
 *  Queue a buffer at the start of a list. This function takes no locks
 *  and you must therefore hold required locks before calling it.
 *
 *  A buffer cannot be placed on two lists at the same time.
 */
extern void skb_queue_head(struct sk_buff_head *list, struct sk_buff *newsk);
static inline void __skb_queue_head(struct sk_buff_head *list,
                                    struct sk_buff *newsk)
{
    __skb_queue_after(list, (struct sk_buff *)list, newsk);
}

/**
 *  __skb_queue_tail - queue a buffer at the list tail
 *  @list: list to use
 *  @newsk: buffer to queue
 *
 *  Queue a buffer at the end of a list. This function takes no locks
 *  and you must therefore hold required locks before calling it.
 *
 *  A buffer cannot be placed on two lists at the same time.
 */
extern void skb_queue_tail(struct sk_buff_head *list, struct sk_buff *newsk);
static inline void __skb_queue_tail(struct sk_buff_head *list,
                                    struct sk_buff *newsk)
{
    __skb_queue_before(list, (struct sk_buff *)list, newsk);
}



/*
 * remove sk_buff from list. _Must_ be called atomically, and with
 * the list known..
 */
extern void    skb_unlink(struct sk_buff *skb, struct sk_buff_head *list);
static inline void __skb_unlink(struct sk_buff *skb, struct sk_buff_head *list)
{
    struct sk_buff *next, *prev;

    list->qlen--;
    next       = skb->next;
    prev       = skb->prev;
    skb->next  = skb->prev = NULL;
    next->prev = prev;
    prev->next = next;
}

/**
 *  __skb_dequeue - remove from the head of the queue
 *  @list: list to dequeue from
 *
 *  Remove the head of the list. This function does not take any locks
 *  so must be used with appropriate locks held only. The head item is
 *  returned or %NULL if the list is empty.
 */
extern struct sk_buff *skb_dequeue(struct sk_buff_head *list);
static inline struct sk_buff *__skb_dequeue(struct sk_buff_head *list)
{
    struct sk_buff *skb = skb_peek(list);
    if (skb) {
        __skb_unlink(skb, list);
    }
    return skb;
}



/**
 *  __skb_queue_purge - empty a list
 *  @list: list to empty
 *
 *  Delete all buffers on an &sk_buff list. Each buffer is removed from
 *  the list and one reference dropped. This function does not take the
 *  list lock and the caller must hold the relevant locks to use it.
 */
extern void skb_queue_purge(struct sk_buff_head *list);
static inline void __skb_queue_purge(struct sk_buff_head *list)
{
    struct sk_buff *skb;
    while ((skb = __skb_dequeue(list)) != NULL) {
        __kfree_skb(skb);
    }

}

#endif /* __SKBUFF_H__*/
