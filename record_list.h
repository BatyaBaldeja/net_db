#ifndef RECORD_LIST_H
#define RECORD_LIST_H

#include "hash_fun.h"
#include "rb_tree.h"

#define MAX_PRINT_LIST 15

class list {
	private:
		list_node *head;
		list_node *curr;
		
		hash_fun *hash;
		tree_rb_phone *rb_tree;

	public:
		list() {hash = 0; head = curr = 0; hash = new hash_fun(0); rb_tree = new tree_rb_phone();}
		~list() {delete_list(); head = curr = 0; delete hash; delete rb_tree;}
		
		list_node * get_head() {return head;}
		list_node * get_curr() {return curr;}
		void set_head(list_node *h) {head = h; return;}
		void set_curr(list_node *c) {curr = c; return;}
		
		hash_fun * get_hash() {return hash;}
		tree_rb_phone * get_rb_tree() {return rb_tree;}
		
		void delete_list();
		void delete_list_node(list_node *p_head);
		
		int read_list(FILE *fp = stdin);
		list_node * add_new(list_node *buf);
		
		void delete_node(list_node *a);
		
		list_node * push(list_node *rhs);
		void print_list(FILE *fp = stdout, list_node *node = 0);
		void print_list_inverse(FILE *fp = stdout, list_node *node = 0);
		
		void delete_element(list_node *element) {
			if (element) {
				if (element -> get_next() && element-> get_prev()) {
					element -> get_next() -> set_prev(element -> get_prev());
					element -> get_prev() -> set_next(element -> get_next());
					delete element;
					return;
				} else if (element -> get_next() == 0 && element -> get_prev()) {
					element -> get_prev() -> set_next(0);
					delete element;
					return;
				} else if (element -> get_prev() == 0 && element -> get_next()) {
					element -> get_next() -> set_prev(0);
					set_head(element -> get_next()); 
					delete element;
					return;
				} else if (element -> get_next() == 0 && element -> get_prev() == 0) {
					delete element;
					set_curr(0); set_head(0);
					return;
				}
			}
			return;
		}
};

#endif // !RECORD_LIST_H
