#include <stdio.h>
#include <stdlib.h>

struct ll_node {
	void		*data;
	struct ll_node	*left, *right;
};

struct list_head {
	int		len;
	struct ll_node 	*start, *end;
};

static int ll_insert_node(struct list_head *head, void *val)
{
	struct ll_node *p;

	p = (struct ll_node *)malloc(sizeof(*p));
	if (p == NULL)
		return 1;
	
	p->data = val;
	p->right = p->left = NULL;

	if (head->len == 0)
		head->start = head->end = p;
	else {
		head->end->right = p;
		p->left = head->end;
		head->end = p;
	}

	head->len++;
	return 0;
}

static int ll_node_exists(struct list_head *head, struct ll_node *n)
{
	struct ll_node *p;

	if (!head || !n || head->len == 0)
		return 1;

	p = head->start;
	while (p != NULL) {
		if (p == n)
			return 0;
	}
	return 1;
}

static int ll_insert_node_after(struct list_head *head, int pos, void *val)
{
	struct ll_node *p, *q, *r;
	int i = 0;

	if (!head || head->len == 0 || pos > head->len || pos < 0 || !val)
		return 1;

	p = head->start;

	while (p != NULL) {
		if (i == pos) {
			q = head->end;
			r = p->right;
			head->end = p;
			if (ll_insert_node(head, val)) {
				head->end = p;
				return 1;
			}
			head->end->right = r;
			r->left = head->end;
			head->end = q;
			break;
		}
		i++;
		p = p->right;
	}

	if (p == NULL)
		return 1;
	return 0;
}

static int ll_insert_node_before(struct list_head *head, int pos, void *val)
{
	struct ll_node *p, *q;
	int i = 0;

	if (!head || head->len == 0 || pos > head->len || pos < 0 || !val)
		return 1;

	p = head->start;

	while (p != NULL) {
		if (i == pos) {
			q = head->end;
			head->end = p->left;
			if (ll_insert_node(head, val)) {
				head->end = p;
				return 1;
			}
			head->end->right = p;
			p->left = head->end;
			head->end = q;
		}
		i++;
		p = p->right;
	}

	if (p == NULL)
		return 1;
	return 0;
}

static int ll_delete_node(struct list_head *head, int pos)
{
	struct ll_node *p;
	struct ll_node *temp;
	int i = 0;

	if (!head || head->len == 0 || pos < 0 || pos > head->len)
		return 1;

	p = head->start;
	while (p != NULL) {
		if (i == pos) {
			temp = p;
			p->left->right = p->right;
			p->right->left = p->left;
			break;
		}
		i++;
		p = p->right;
	}
	
	if (p == NULL)
		return 1;
	
	free(temp);
	return 0;
}

static int ll_delete(struct list_head *head)
{
	struct ll_node *p, *q;

	if (!head || head->len == 0)
		return 1;

	p = head->start;

	while (p != NULL) {
		q = p;
		p = p->right;
		free(q);
	}

	head->len = 0;
	head->start = head->end = 0;
	return 0;
}

static struct list_head * ll_create()
{
	struct list_head *lh;

	lh = (struct list_head *)malloc(sizeof(*lh));
	if (lh == NULL)
		return NULL;

	lh->len = 0;
	lh->start = lh->end = NULL;

	return lh;
}
