struct list_head {
	struct list_head *next, *prev;
};

static int list_insert(struct list_head *head, struct list_head *val)
{
	struct list_head *p;

	if (head == NULL || val == NULL)
		return 1;

	p = head;
	while (p->next != NULL)
		p = p->next;

	p->next = val;
	return 0;
}

static int list_node_exists(struct list_head *head, struct list_head *val)
{
	struct list_head *p;

	if (!head || !val)
		return 1;

	p = head;
	while (p != NULL) {
		if (p == val)
			return 0;
	}
	return 1;
}

static int list_insert_after(struct list_head *head, int pos, struct list_head *val)
{
	struct list_head *p, *q;
	int i = 0;

	if (!head || pos < 0 || !val)
		return 1;

	p = head;

	while (p != NULL) {
		if (i == pos) {
			q = p->next;
			p->next = val;
			val->prev = p;
			val->next = q;
			q->prev = val;
			break;
		}
		i++;
		p = p->next;
	}

	if (p == NULL)
		return 1;
	return 0;
}

static int ll_insert_before(struct list_head *head, int pos, struct list_head *val)
{
	struct list_head *p, *q;
	int i = 0;

	if (!head || pos < 0 || !val)
		return 1;

	p = head;

	while (p != NULL) {
		if (i == pos) {
			q = p->prev;
			q->next = val;
			val->prev = q;
			val->next = p;
			p->prev = val;
		}
		i++;
		p = p->next;
	}

	if (p == NULL)
		return 1;
	return 0;
}

static struct list_head* list_delete_node(struct list_head *head, int pos)
{
	struct list_head *p;
	struct ll_node *temp;
	int i = 0;

	if (!head || pos < 0)
		return NULL;

	p = head;
	while (p != NULL) {
		if (i == pos) {
			if (pos == 0)
				p->next->prev = NULL;
			else if (p->next == NULL)
				p->prev->next = NULL;
			else {
				p->prev->next = p->next;
				p->next->prev = p->prev;
			}
			break;
		}
		i++;
		p = p->next;
	}
	
	return p;
}
