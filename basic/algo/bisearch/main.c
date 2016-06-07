#include <stdio.h>
#include <stdlib.h>

#define QUEUE_SIZE	256

struct qs {
	int d[QUEUE_SIZE];
	int n;
} head;

static inline int getbyte()
{
	return random() & 0xffff;
}

void print(struct qs *q)
{
	int i;

	printf("queue num %d\n", q->n);
	for(i = 0; i < q->n; i++)
		printf("%d: %d\n", i, q->d[i]);
}

int find(struct qs *q, int x)
{
	int i, n, m;

	n = 0;
	m = q->n - 1;

	for(i = m / 2; n <= m; i = (n + m) / 2) {
		printf("[%d %d %d]\n", n, i, m);
		if(x == q->d[i])
			break;
		else if(x > q->d[i])
			n = i + 1;
		else
			m = i - 1;
	}

	return i;
}

int insert(struct qs *q, int x)
{
	int i, j;

	if(q->n >= QUEUE_SIZE) {
		printf("queue overflow!\n");
		return -1;
	} else if(q->n <= 0) {
		q->d[0] = x;
		q->n = 1;
		return 0;
	}

	i = find(q, x);

	/* if c < found, mv current... -> 
	 * else if c > found, mv current + 1... ->
	 */
	if(x > q->d[i])
		i++;
	//printf("num = %d, insert to %d\n", q->n, i);
	for(j = q->n; j > i; j--) {
		q->d[j] = q->d[j - 1];
	}

	q->d[i] = x;
	q->n++;

	return 0;
}

int main(void)
{
	int i;

	for(i = 0; i < QUEUE_SIZE; i++) {
		int x = getbyte();
		printf("insert: %d\n", x);
		insert(&head, x);
	}
	print(&head);
	return 0;
}
