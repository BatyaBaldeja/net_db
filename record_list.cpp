#include "record_list.h"

void
list::delete_list() {
	if (head)
		delete_list_node(head);
		head = curr = 0;
		return;
}

void
list::delete_list_node(list_node *p_head) {
		list_node *tmp;
		while (p_head) {
			tmp = p_head;
			if (tmp -> next)
				p_head = tmp -> next;
			else
				p_head = nullptr;
			delete tmp;
		}
}

list_node *
list::push(list_node *rhs) {
	if (rhs) {
		if (head) {
			rhs -> next = head;
			rhs -> next -> prev = rhs;
			rhs -> prev = 0;
			head = curr = rhs;
			return curr;
		} else {
			curr = head = rhs; 
			head -> next = head -> prev = 0; 
			return curr;
		}
	}
	return curr;
}
	
list_node *
list::add_new(list_node *buf) {
	buf = push(&((new list_node) -> move(*buf)));
	hash -> add_new(buf);
	rb_tree -> insert(buf);
	return buf;
}

int
list::read_list(FILE *fp) {
	list_node buf;
	switch (buf.read(fp)) {
		case 0: add_new(&buf); break;
		case 1: curr = head = NULL; return 1;
		case 2: break;
	}
	while (1) {
		switch (buf.read(fp)) {
			case 0: add_new(&buf); break;
			case 1: curr = head; return 1;
			case 2: break;
		}
	}
	curr = head;
	return 0;
}


void
list::print_list(FILE *fp, list_node *node) {
	printf("\nPrint List:\n");
	if (!head) return;
	if (!node) node = head;
	if (node) {
		int i = 0;
		for (node = head; node; node = node -> next) {
			if (i < MAX_PRINT_LIST) {
			node -> print(fp); 
			} else
				break; 
			i++;
		}
	}
}

void
list::print_list_inverse(FILE * fp, list_node *node) {
	printf("\nPrint Inverse List:\n");
	if (!head) return;
	if (!node) node = head;
	for (; node -> next; node = node -> next) {}
	if(node) {
		int i = 0;
		for (; node; node = node -> prev) {
			if(i < MAX_PRINT_LIST) {
				node -> print(fp);
			} else
				break;
			i++;
		}
	}
}

void
list::delete_node (list_node *a) {
	if (a == head) {
		head = a->get_next();
		head -> set_prev(0);
		delete a;
		return;
	} if (a -> get_next() == 0) {
		a -> get_prev() -> set_next(0);
		delete a;
		return;
	}
	a -> get_next() -> set_prev(a -> get_prev());
	a -> get_prev() -> set_next(a -> get_next());
	delete a;
}

