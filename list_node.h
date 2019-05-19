#ifndef LIST_NODE_H
#define LIST_NODE_H

#include "command.h"

class hash_fun_node;
class node_rb;
class list;

class list_node: public record {
	private:
		list_node *next;
		list_node *prev;
		
		friend class list;

	public:
		list_node() {next = prev = 0;}
		~list_node() {next = prev = 0;}
		
		list_node * get_next() {return next;}
		list_node * get_prev() {return prev;}
		void set_next(list_node *n) {next = n; return;}
		void set_prev(list_node *p) {prev = p; return;}
		
		list_node & move(list_node & rhs) {
			if (this == &rhs) return *this;
			record::move(rhs);
			next = rhs.next;
			prev = rhs.prev;
			rhs.prev = rhs.next = 0;
			return *this;
		}
		
		list_node & copy(list_node & rhs) {
			if (this == &rhs) return *this;
			record::copy(rhs);
			next = rhs.next;
			prev = rhs.prev;
			return *this;
		}
};

#endif // !LIST_NODE
