#include "rb_tree.h"

void
tree_rb::print(const node_rb *root, int isLeft, int level) {
	if(!root || root == nil) return;
	int i = 0;
	if (level > MAX_PRINT_TREE)    
		return;
	for (i = 0; i < level; i++) printf("| ");
	if (isLeft == 1) printf("L: " );
	if (isLeft == 0) printf("R: " );
	if(root -> key) root -> key -> print_();
	if(root -> red) printf (" \"RED\"\n");
	else printf (" \"BLACK\"\n");
	if (root -> left)	print (root -> left, 1, level + 1);
	if (root -> right)	print (root -> right, 0, level + 1);
}


void
tree_rb::print_tree() {
	printf("Print RB_tree:\n");
	print(root, -1, 0);
	printf("\n");
}


void
tree_rb::delete_tree(node_rb *a) {
	if (a -> left && a -> left != nil)
		delete_tree (a -> left);
	if (a -> right && a -> right != nil)
		delete_tree (a -> right);
	if (a == root)
		root = 0;
	delete (a);
}


void
tree_rb::rotateleft(node_rb *a) {
	node_rb *b = a -> right;
	node_rb *p = a -> parent;
	if (b == 0 || b == nil)
		return;
	b -> parent = p;
	if (p != 0) {
		if (a == p -> left)
			p -> left = b;
		else
			p -> right = b;
	} else {
		root = b;
	}
	a -> right = b -> left;
	if (b -> left != 0 && b -> left != nil)
		b -> left -> parent = a;
	b -> left = a;
	a -> parent = b;
}


void
tree_rb::rotateright(node_rb *a) {
	node_rb *b = a -> left;
	node_rb *p = a -> parent;
	if (b == 0 || b == nil)
		return;
	b -> parent = p;
	if (p != 0) {
		if (a == p -> left)
			p -> left = b;
		else
			p -> right = b;
	} else {
		root = b;
	}
	a -> left = b -> right;
	if (b -> right != 0 && b -> right != nil)
		b -> right -> parent = a;
	b -> right = a;
	a -> parent = b;
}


void
tree_rb::fix(node_rb *a) {
	while(a != root && a -> parent -> red) {
		if (a->parent == a->parent->parent->left) {
			node_rb *b = a->parent->parent->right;
			if (b != 0 && b!= nil && b->red) {
				a->parent->red = false;
				b->red = false;
				a = a->parent->parent;
				a->red = true;
			} else {
				if (a == a->parent->right) {
					a = a->parent;
					rotateleft(a);
				}
				a->parent->red = false;
				a->parent->parent->red = true;
				rotateright(a->parent->parent);
			}
		} else {

			node_rb *b = a->parent->parent->left;
			if (b != 0 && b != nil && b->red) {
				a->parent->red = false;
				b->red = false;
				a = a->parent->parent;
				a->red = true;
			} else {
				if (a == a->parent->left) {
					a = a->parent;
					rotateright(a);
				}
				a->parent->red = false;
				a->parent->parent->red = true;
				rotateleft(a->parent->parent);
			}
		}
	}
	if (a == root)
		a->red = false;
}


void
tree_rb_phone::insert(list_node *key) {
    node_rb *a, *p = root, *t;
	if (root == 0) {
		nil = new node_rb(0, false, nil);
		root = new node_rb(key, false, nil);
		root -> red = false;
		return;
	} else if (root == nil) {
		root = new node_rb(key, false, nil);
		return;
	} else {
		a = new node_rb(key, true, nil);
		while (p != nil) {
			t = p;
			if (p -> key -> get_phone() < a -> key -> get_phone()) p = p -> right;
			else p = p -> left;
		}
		a -> parent = t;
		if (t -> key -> get_phone() < a -> key -> get_phone()) t -> right = a;
		else t -> left = a;
	}
	fix(a);
}


void
tree_rb::deletefix(node_rb *a) {
	while (a != root && a -> red == false) {
		if (a == a->parent->left) {
			node_rb *b = a->parent->right;
			if (b->red == true) {
				b->red = false;
				a->parent->red = true;
				rotateleft(a->parent);
				b = a->parent->right;
			}
			if (b -> left && b->left->red == false && b -> right && b->right->red == false) {
				b->red = true;
				a = a->parent;
			} else {
				if (b -> right && b->right->red == false) {
					if(b -> left) b->left->red = false;
					b->red = true;
					rotateright(b);
					b = a->parent->right;
				}
				b->red = a->parent->red;
				a->parent->red = false;
				if(b -> right) b->right->red = false;
				rotateleft(a->parent);
				a = root;
			}
		} else {
			node_rb *b = a->parent->left;
			if (b->red == true) {
				b->red = false;
				a->parent->red = true;
				rotateright(a->parent);
				b = a->parent->left;
			}
			if (b -> right && b->right->red == false && b-> left && b->left->red == false) {
				b->red = true;
				a = a->parent;
			} else {
				if (b-> left && b->left->red == false) {
					if(b -> right) b->right->red = false;
					b->red = true;
					rotateleft(b);
					b = a->parent->left;
				}
				b->red = a->parent->red;
				a->parent->red = false;
				if(b -> left) b->left->red = false;
				rotateright(a->parent);
				a = root;
			}
		}
	}
	a->red = false;
}


node_rb * tree_rb_phone::search(node_rb *node, list_node *del) {
	if (!node || node == nil) return 0;
	int ifleft = 0, ifright = 0;
	node_rb * tmp = 0;
	if (del == node -> key)
		return node;
		
	if (del -> get_phone() <= node -> get_key() -> get_phone()) ifleft = 1;
	if (del -> get_phone() >= node -> get_key() -> get_phone()) ifright = 1;
	if(ifleft) tmp = search(node -> left, del);
	if(tmp) return tmp;
	if(ifright) tmp = search(node -> right, del);
	if(tmp) return tmp;
	return 0;
}


void tree_rb_phone::deletenode(list_node *del) {

	/* for (curr = root; curr != nil; ) {
		if (del == curr -> key)
			{break;}

		if (del -> get_phone() < curr -> get_key() -> get_phone())
			goto_left();
		else
			goto_right();
	}
	
	if (curr == nil || !curr)
		return; */
		
	

	node_rb *a = search(root, del), *b = 0, *c = 0;
	
	//a -> get_key() -> print();

	if (!a || a == nil) return;

	if (a->left == nil || a->right == nil)
		c = a;
	else 
	{
		c = a->right;
		while (c->left != nil)
			c = c->left;
	}

	if (c->left != nil)
	b = c->left;
	else
		b = c->right;

	b->parent = c->get_parent();

	if (c->parent)
	{
		if (c == c->parent->left)
			c->parent->left = b;
		else
			c->parent->right = b;
	}
	else
		root = b;

	if (c != a)
	{ 
		a->key = c->key;
	}

	if (c->red == false && b != nil)
		deletefix (b);
		
	delete c;
}

