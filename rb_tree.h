#ifndef RB_TREE_H
#define RB_TREE_H

#include "list_node.h"

#define MAX_PRINT_TREE 3

class list;

class node_rb {
	private:
		node_rb *left = 0;
		node_rb *right = 0;
		node_rb *parent = 0;
		list_node *key = 0;
		bool red = false;
		
	public:
		node_rb(list_node *a = 0, bool red = false, node_rb *nil = 0) {key = a; this -> red = red; this -> left = nil, this -> right = nil;}
		~node_rb() {left = 0; right = 0; key = 0; parent = 0;}
		
		list_node * get_key() {return key;}
		node_rb * get_left() {return left;}
		node_rb * get_right() {return right;}
		node_rb * get_parent() {return parent;}

		friend class tree_rb;
		friend class tree_rb_phone;
};

class tree_rb {
	private:
		node_rb *root;
		node_rb *curr;
		node_rb *nil;
		
		void delete_tree(node_rb *subtree);
		
	public:
		tree_rb() {root = curr = nil = 0;}
		~tree_rb() {if (root && root != nil) delete_tree(root); root = 0; curr = 0; delete nil;}

		void print_tree();
		void print(const node_rb *root, int isLeft = -1, int level = 0);

		void insert(list_node *key);
		void fix(node_rb *a);
		void rotateleft(node_rb *a);
		void rotateright(node_rb *a);

		void deletefix(node_rb *a);
		void deletenode(node_rb *a);

		void goto_root() {curr = root;}
		void goto_left() {curr = curr -> left;}
		void goto_right() {curr = curr -> right;}

		node_rb * get_curr() {return curr;}
		node_rb * get_root() {return root;}
		node_rb * get_nil() {return nil;}
		void set_curr(node_rb *tmp) {curr = tmp; return;}
		void set_root(node_rb *tmp) {root = tmp; return;}

		friend class list;
		friend class tree_rb_phone;
};

class tree_rb_phone : public tree_rb {
	public:
		tree_rb_phone() : tree_rb () {}
		~tree_rb_phone() {}
		
		node_rb * search(node_rb *node, list_node *del);
		void insert(list_node *key);
		void deletenode(list_node *del);
		
		friend class list;
};

#endif // !RB_TREE_H
