#ifndef __LIST_H_
#define __LIST_H_

struct list_head {
	struct list_head *prev, *next;
};

#define LIST_HEAD_INIT(head)	struct list_head (head) = { .prev = &(head), .next = &(head)}
#define INIT_LIST_HEAD(head)	do { (head)->prev = (head); (head)->next = (head); } while(0)

#define container_of(ptr, type, list)	((type *)((char *)(ptr) - ((char *)(&((type *)0)->list))))

static inline void list_add(struct list_head *head, struct list_head *list)
{
	list->prev = head;
	list->next = head->next;
	head->next->prev = list;
	head->next = list;
}

static inline void list_add_tail(struct list_head *head, struct list_head *list)
{
	list->prev = head->prev;
	list->next = head;
	head->prev->next = list;
	head->prev = list;
}

static inline struct list_head * list_del(struct list_head *list)
{
	list->prev->next = list->next;
	list->next->prev = list->prev;
	list->prev = 0;
	list->next = 0;

	return list;
}

static inline int list_is_empty(struct list_head *head)
{
	return head->next == head;
}

#endif
