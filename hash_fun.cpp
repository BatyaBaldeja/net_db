#include "hash_fun.h"

hash_fun::hash_fun(int t) {
	type = t;
	switch(type) {
		case 0: { hash_table_size = HASH_TABLE_SIZE_NAME; break; }
		case 1: { hash_table_size = HASH_TABLE_SIZE_PHONE; break; }
		case 2: { hash_table_size = HASH_TABLE_SIZE_GROUP; break; }
	}
	head = new hash_fun_node[hash_table_size];
	for (int i = 0; i < hash_table_size; i++) {
		head[i].list_node_hash = 0;
		head[i].next = 0;
	}
	return;
}

void
hash_fun::delete_hash_fun() {
	if(!head) return;
	for (int i = 0; i < hash_table_size; i++) {
		while (head[i].next) {
			hash_fun_node * tmp = head[i].next;
			head[i].next = tmp -> next;
			tmp -> next = 0;
			tmp -> list_node_hash = 0;
			delete tmp;
		}
	}
	delete [] head;
	return;
}

int
hash_fun::search(list_node * buf) {
	if (!buf) return 0;
	int number = 0;
	switch (type) {
		case 0: { number = hash(buf -> get_name()); break; }
		case 1: { number = hash(buf -> get_phone()); break; }
		case 2: { number = hash(buf -> get_group()); break; }
	}
	for (hash_fun_node * h = head + number; h; h = h -> next) {
		if (h -> list_node_hash && h -> list_node_hash -> get_phone() == buf -> get_phone() && h -> list_node_hash -> get_group() == buf -> get_group())
			return 1;
	}
	return 0;
}

int
hash_fun::search_and_delete(list_node * buf) {
	if (!buf) return 0;
	int number = 0;
	switch (type) {
		case 0: { number = hash(buf -> get_name()); break; }
		case 1: { number = hash(buf -> get_phone()); break; }
		case 2: { number = hash(buf -> get_group()); break; }
	}
	for (hash_fun_node * h = head + number; h; h = h -> next) {
		if (h -> list_node_hash && h -> list_node_hash -> get_phone() == buf -> get_phone() && h -> list_node_hash -> get_group() == buf -> get_group())
			h -> delete_from_hash();
	}
	return 0;
}

int
hash_fun::add_new(list_node * buf) {
	if (!buf) return 0;
	int number = 0;
	switch (type) {
		case 0: { number = hash(buf -> get_name()); break; }
		case 1: { number = hash(buf -> get_phone()); break; }
		case 2: { number = hash(buf -> get_group()); break; }
	}
	if (number > hash_table_size) return 0;
	if (!head[number].list_node_hash) {
		head[number].list_node_hash = buf;
	} else if (!cmp_(head[number].list_node_hash, buf)) {
		hash_fun_node * tmp = new hash_fun_node;
		if ((head + number) -> next)
			tmp -> next = (head + number) -> next;
		else
			tmp -> next = 0;
		(head + number) -> next = tmp;
		tmp -> list_node_hash = (head + number) -> list_node_hash;
		(head + number) -> list_node_hash = buf;
	}
	return 1;
}

int
hash_fun::cmp_(list_node *tmp1, list_node *tmp2) {
	if(!tmp1 || !tmp2) return 1;
	switch(type) {
		case 0: { return name_cmp(tmp1-> get_name(), tmp2 -> get_name()); }
		case 1: { return phone_cmp(tmp1 -> get_phone(), tmp2 -> get_phone()); }
		case 2: { return group_cmp(tmp1 ->  get_group(), tmp2 ->  get_group()); }
	}
	return 1;
}

void
hash_fun::print_hash() {
	if(!head) return;
	printf("\nPrint Hash: \n");
	int k = 0;
	for (int i = 0; i < hash_table_size; i++) {
		if (k >= MAX_PRINT_HASH) {printf("\n"); return;}
		hash_fun_node * buf = head[i].next;
		int l = 0, m = 0;
		if (head[i].list_node_hash) {
			head[i].list_node_hash -> print_();
			k++; l = 1; m = 1;
		}
		for (int j = m; buf && j < MAX_PRINT_HASH_L; j++) {
			if (buf -> list_node_hash) {
				if (l == 1) {
					printf("-> ");
					buf -> list_node_hash -> print_();
				} else if(l == 0) {
					k++; l = 1;  
					buf -> list_node_hash -> print_();
				}
			} else j--;
		buf = buf -> next;
		}
		if(l == 1) printf("\n");
	}
	printf("\n");
}
	
