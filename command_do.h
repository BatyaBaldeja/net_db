#ifndef COMMAND_DO_H
#define COMMAND_DO_H

#include "record_list.h"

#define MAX_PRINT_COMMAND 100
#define MASSIVE_GROUP 498

class command_do {
	private:
		list *l_group;
		command *com;
		
		hash_fun *hash_global;
		tree_rb_phone *rb_tree_global;

	public:
		command_do() {
			com = new command;
			l_group = new list[MASSIVE_GROUP];
			hash_global = new hash_fun(0);
			rb_tree_global = new tree_rb_phone();
		}
		~command_do() {
			delete com;
			delete [] l_group;
			delete hash_global;
			delete rb_tree_global;
			l_group = 0; com = 0;
		}
		
		command * get_com() {return com;}
		
		
		/*void launch(FILE *fp) {
			int i = 0;
            //int k = 0;
			double t = clock();
			read_base(fp);
			printf("Time reading: %0.2lf", (clock() - t) / CLOCKS_PER_SEC);
			
			hash_global -> print_hash();
			rb_tree_global -> print_tree();
			
			
			for(int j = 0; j < 498; j++) {
				if(l_group[j].get_head()) l_group[j].print_list();
			}
			
			
			printf("Input command: \n");

			while(com -> command_read()) {
				if(i < MAX_PRINT_COMMAND) {com -> print_command(); i++;}
				
				apply(); 
				
			printf("/////////////////////////////////////////////////////");
			hash_global -> print_hash();
			rb_tree_global -> print_tree();
			
			for(int j = 0; j < 498; j++) {
				if(l_group[j].get_head()) l_group[j].print_list();
			}
			printf("/////////////////////////////////////////////////////");
			
				com -> free_command();
                //k++; printf("command done: %d\n", k);
			}
		}*/
		
		void only_cmd(int sock) {
			//printf("Input command: \n");
				apply(sock); 
				com -> free_command();
		}
		
		void read_base(FILE *fp) {
			list_node buf;
			list_node *tmp = 0;
			while (1) {
				switch (buf.read(fp)) {
					case 0: {
						tmp = l_group[buf.get_group() - 101].add_new(&buf);
						hash_global -> add_new(tmp);
						rb_tree_global -> insert(tmp);
						break;
					}
					case 1: return;
					case 2: break;
				}
			}
		}
		
		void apply(int sock) {
			if (com -> type == INSERT) insert();
			if (com -> type == SELECT || com -> type == DELETE) select_delete(sock);
		}
		
		void insert() {
			list_node buf;
			list_node *tmp = 0;
			buf.init(com -> get_name(), com -> get_phone(), com -> get_group());
			if (hash_global -> search(&buf) == 1) return;
			tmp = l_group[buf.get_group() - 101].add_new(&buf);
			hash_global -> add_new(tmp);
			rb_tree_global -> insert(tmp);
		}

		void select_delete(int sock) {
			if(com -> operation != OR && com -> c_group == EQ) {
				list_group_do(sock);
			} else if (com -> operation != OR && com -> c_phone != COND_NONE && com -> c_phone != NE) {
				tree_do(sock);
			} else if (com -> operation != OR && (com -> c_name == EQ || com -> c_name == LIKE)) {
				hash_do(sock);
			} else simple_do(sock);
			return;
		}
		
		void list_group_do(int sock) {
			if (com -> operation != OR && com -> c_phone != COND_NONE && com -> c_phone != NE) {
				tree_do_l_group(sock);
			} else if (com -> operation != OR && (com -> c_name == EQ || com -> c_name == LIKE)) {
				hash_do_l_group(sock);
			} else simple_do_l_group(sock);
		}
		
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
		
		void hash_do(int sock) {
        	int number = hash_global -> hash(com -> get_name()), t = 0;
			hash_fun_node * tmp = hash_global -> get_head() + number;
			while (tmp) {
				if (tmp -> get_list_node_hash()) {
					t = switch_do_name(tmp -> get_list_node_hash());
					if (t) {
						if (com -> operation == OP_NONE) {
                        	if(com -> type == DELETE) del_hash(tmp -> get_list_node_hash(), tmp);
                            else if(com -> type == SELECT) tmp -> get_list_node_hash() -> print_sock(sock);
                        } else if (com -> operation == AND) {
							int i1 = switch_do_phone(tmp -> get_list_node_hash()); 
							int i2 = switch_do_group(tmp -> get_list_node_hash()); 
                            if (i1 || i2) {
                            	if(com -> type == DELETE) del_hash(tmp -> get_list_node_hash(), tmp);
                       	    	else if(com -> type == SELECT) tmp -> get_list_node_hash() -> print_sock(sock);
							}
						}
					}
				}
				tmp = tmp -> get_next();
			}
			return;		
		}
		
		void del_hash(list_node *list_node_hash, hash_fun_node * tmp) {
			int t = list_node_hash -> get_group() - 101;
			
			tmp -> set_list_node_hash(0);
			rb_tree_global -> deletenode(list_node_hash);
			
			l_group[t].get_rb_tree() -> deletenode(list_node_hash);
			l_group[t].get_hash() -> search_and_delete(list_node_hash);
			l_group[t].delete_element(list_node_hash);
		}
		
///////////////////////////////////////////////////////////////////////////////

		void hash_do_l_group(int sock) {
			int t = com -> get_group() - 101;
        	int number = l_group[t].get_hash() -> hash(com -> get_name()), buf = 0;
			hash_fun_node * tmp = l_group[t].get_hash() -> get_head() + number;
			while (tmp) {
				if (tmp -> get_list_node_hash()) {
					buf = switch_do_name(tmp -> get_list_node_hash());
					if (buf) {
						if (com -> operation == OP_NONE) {
                        	if(com -> type == DELETE) del_hash_l_group(tmp -> get_list_node_hash(), tmp);
                            else if(com -> type == SELECT) tmp -> get_list_node_hash() -> print_sock(sock);
                        } else if (com -> operation == AND) {
							int i1, i2;
							if(com -> c_phone) i1 = switch_do_phone(tmp -> get_list_node_hash());  
							else i2 = 1;
                            if (i1 || i2) {
                            	if(com -> type == DELETE) del_hash_l_group(tmp -> get_list_node_hash(), tmp);
                       	    	else if(com -> type == SELECT) tmp -> get_list_node_hash() -> print_sock(sock);
							}
						}
					}
				}
				tmp = tmp -> get_next();
			}
			return;	
		}
		
		void del_hash_l_group(list_node *list_node_hash, hash_fun_node *tmp) {
			int t = list_node_hash -> get_group() - 101;
			
			hash_global -> search_and_delete(list_node_hash);
			rb_tree_global -> deletenode(list_node_hash);
			
			l_group[t].get_rb_tree() -> deletenode(list_node_hash);
			tmp -> set_list_node_hash(0);
			l_group[t].delete_element(list_node_hash);
		}
		
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////	

		void tree_do(int sock) {
			if (com -> operation == OP_NONE) {
				if (com -> type == SELECT) print_all_only(rb_tree_global -> get_root(), com -> get_phone(), sock);
				if (com -> type == DELETE) delete_all_only(rb_tree_global -> get_root(), com -> get_phone());
			} else if (com -> operation == AND) {
				if (com -> c_name) {
					if (com -> type == SELECT) print_all_name(rb_tree_global -> get_root(), com -> get_phone(), com -> get_name(), sock);
					if (com -> type == DELETE) delete_all_name(rb_tree_global -> get_root(), com -> get_phone(), com -> get_name());
				} else if (com -> c_group) {
					if (com -> type == SELECT) print_all_group(rb_tree_global -> get_root(), com -> get_phone(), com -> get_group(), sock);
					if (com -> type == DELETE) delete_all_group(rb_tree_global -> get_root(), com -> get_phone(), com -> get_group());
				}
			}
		}
		
///////////////////////////////////////////////////////////////////////////////

		void tree_do_l_group(int sock) {
			int t = com -> get_group() - 101;
			if (com -> operation == OP_NONE) {
				if (com -> type == SELECT) print_all_only(l_group[t].get_rb_tree() -> get_root(), com -> get_phone(), sock);
				if (com -> type == DELETE) delete_all_only(l_group[t].get_rb_tree() -> get_root(), com -> get_phone());
			} else if (com -> operation == AND) {
				if (com -> c_name) {
					if (com -> type == SELECT) print_all_name(l_group[t].get_rb_tree() -> get_root(), com -> get_phone(), com -> get_name(), sock);
					if (com -> type == DELETE) delete_all_name(l_group[t].get_rb_tree() -> get_root(), com -> get_phone(), com -> get_name());
				} else if (com -> c_group) {
					if (com -> type == SELECT) print_all_only(l_group[t].get_rb_tree() -> get_root(), com -> get_phone(), sock);
					if (com -> type == DELETE) delete_all_only(l_group[t].get_rb_tree() -> get_root(), com -> get_phone());
				}
			}
		}
		
///////////////////////////////////////////////////////////////////////////////
		
		void delete_all_only(node_rb *node, long long ph) {
			if (!node || node == rb_tree_global -> get_nil()) return;
			int i = 0, ifleft = 0, ifright = 0;
			node_rb *tmp = node -> get_parent();
			switch(com -> c_phone) {
				case 0: {break;}
				case 2: {break;}
				case 7: {break;}
				case 1: {
					if(node->get_key()->get_phone() == ph) {
						del(node); i = 0;
					} else {
						i = 1;
						if(node->get_key()->get_phone() < ph)
							{ifleft = 0; ifright = 1;}
						else
							{ifleft = 1; ifright = 0;}
					}
					break;
				}
				case 3: {if(node->get_key()->get_phone()  < ph) del(node); else {i = 1; ifleft = 1; ifright = 0;} break;}
				case 4: {if(node->get_key()->get_phone()  > ph) del(node); else {i = 1; ifleft = 0; ifright = 1;} break;}
				case 5: {if(node->get_key()->get_phone() <= ph) del(node); else {i = 1; ifleft = 1; ifright = 0;} break;}
				case 6: {if(node->get_key()->get_phone() >= ph) del(node); else {i = 1; ifleft = 0; ifright = 1;} break;}
			}
			if(!tmp) tmp = rb_tree_global -> get_root();
			if(i) {
				if(ifleft) delete_all_only (node -> get_left() , ph);
				if(ifright) delete_all_only (node -> get_right(), ph);
			} else {
				delete_all_only (tmp, ph);
			}
		}


		void delete_all_name(node_rb *node, long long ph, char *name) {
			if (!node || node == rb_tree_global -> get_nil()) return;
			int i = 0, ifleft = 0, ifright = 0;
			node_rb *tmp = node -> get_parent();
			switch(com -> c_phone) {
				case 0: {break;}
				case 2: {break;}
				case 7: {break;}
				case 1: {
					if(node->get_key()->get_phone() == ph) {
						if(name_switch(node, name))
							{del(node); i = 0;}
						else
							 i = 1;
						ifleft = ifright = 1;
					} else {
						i = 1;
						if(node->get_key()->get_phone() < ph)
							{ifleft = 0; ifright = 1;}
						else
							{ifleft = 1; ifright = 0;}
					}
					break;
				}
				case 3: {if(node->get_key()->get_phone()  < ph) {if(name_switch(node, name)) del(node);} else {i = 1; ifleft = 1; ifright = 0;} break;}
				case 4: {if(node->get_key()->get_phone()  > ph) {if(name_switch(node, name)) del(node);}  else {i = 1; ifleft = 0; ifright = 1;} break;}
				case 5: {if(node->get_key()->get_phone() <= ph) {if(name_switch(node, name)) del(node);}  else {i = 1; ifleft = 1; ifright = 0;} break;}
				case 6: {if(node->get_key()->get_phone() >= ph) {if(name_switch(node, name)) del(node);}  else {i = 1; ifleft = 0; ifright = 1;} break;}
			}
			if(!tmp) tmp = rb_tree_global -> get_root();
			if(i) {
				if(ifleft) delete_all_name (node -> get_left(), ph, name);
				if(ifright) delete_all_name (node -> get_right(), ph, name);
			} else {
				delete_all_name (tmp, ph, name);
			}
		}
		
		void delete_all_group(node_rb *node, long long ph, int group) {
			if (!node || node == rb_tree_global -> get_nil()) return;
			int i = 0, ifleft = 0, ifright = 0;
			node_rb *tmp = node -> get_parent();
			switch(com -> c_phone) {
				case 0: {break;}
				case 2: {break;}
				case 7: {break;}
				case 1: {
					if(node->get_key()->get_phone() == ph) {
						if(group_switch(node, group))
							{del(node); i = 0;}
						else
							 i = 1;
						ifleft = ifright = 1;
					} else {
						i = 1;
						if(node->get_key()->get_phone() < ph)
							{ifleft = 0; ifright = 1;}
						else
							{ifleft = 1; ifright = 0;}
					}
					break;
				}
				case 3: {if(node->get_key()->get_phone()  < ph) {if(group_switch(node, group)) del(node);} else {i = 1; ifleft = 1; ifright = 0;} break;}
				case 4: {if(node->get_key()->get_phone()  > ph) {if(group_switch(node, group)) del(node);} else {i = 1; ifleft = 0; ifright = 1;} break;}
				case 5: {if(node->get_key()->get_phone() <= ph) {if(group_switch(node, group)) del(node);} else {i = 1; ifleft = 1; ifright = 0;} break;}
				case 6: {if(node->get_key()->get_phone() >= ph) {if(group_switch(node, group)) del(node);} else {i = 1; ifleft = 0; ifright = 1;} break;}
			}
			if(!tmp) tmp = rb_tree_global -> get_root();
			if(i) {
				if(ifleft) delete_all_group (node -> get_left(), ph, group);
				if(ifright) delete_all_group (node -> get_right(), ph, group);
			} else {
				delete_all_group (tmp, ph, group);
			}
		}

		void print_all_only(node_rb *node, long long ph, int sock) {
			if (!node || node == rb_tree_global -> get_nil() || !node -> get_key()) return;
			int ifleft = 0, ifright = 0;
			switch(com -> c_phone) {
				case 0: {break;}
				case 2: {break;}
				case 7: {break;}
				case 1: {
					if(node->get_key()->get_phone() == ph) {
						node->get_key()->print_sock(sock);
						ifleft = ifright = 1;
					} else {
						if(node->get_key()->get_phone() < ph)
							{ifleft = 0; ifright = 1;}
						else
							{ifleft = 1; ifright = 0;}
					}
					break;
				}
				case 3: {if(node->get_key()->get_phone()  < ph) {node->get_key()->print_sock(sock); ifleft = ifright = 1;} else {ifleft = 1; ifright = 0;} break;}
				case 4: {if(node->get_key()->get_phone()  > ph) {node->get_key()->print_sock(sock); ifleft = ifright = 1;} else {ifleft = 0; ifright = 1;} break;}
				case 5: {if(node->get_key()->get_phone() <= ph) {node->get_key()->print_sock(sock); ifleft = ifright = 1;} else {ifleft = 1; ifright = 0;} break;}
				case 6: {if(node->get_key()->get_phone() >= ph) {node->get_key()->print_sock(sock); ifleft = ifright = 1;} else {ifleft = 0; ifright = 1;} break;}
			}
			if(ifleft) print_all_only (node -> get_left(), ph, sock);
			if(ifright) print_all_only (node -> get_right(), ph, sock);
			return;
		}

		void print_all_name(node_rb *node, long long ph, char *name, int sock) {
			if (!node || node == rb_tree_global -> get_nil()) return;
			int ifleft = 0, ifright = 0;
			switch(com -> c_phone) {
				case 0: {break;}
				case 2: {break;}
				case 7: {break;}
				case 1: {
					if(node->get_key()->get_phone() == ph) {
						if(name_switch(node, name))
							node->get_key()->print_sock(sock);
						ifleft = ifright = 1;
					} else {
						if(node->get_key()->get_phone() < ph)
							{ifleft = 0; ifright = 1;}
						else
							{ifleft = 1; ifright = 0;}
					}
					break;
				}
				case 3: {if(node->get_key()->get_phone()  < ph) {if(name_switch(node, name)) {node->get_key()->print_sock(sock); ifleft = ifright = 1;}} else {ifleft = 1; ifright = 0;} break;}
				case 4: {if(node->get_key()->get_phone()  > ph) {if(name_switch(node, name)) {node->get_key()->print_sock(sock); ifleft = ifright = 1;}} else {ifleft = 0; ifright = 1;} break;}
				case 5: {if(node->get_key()->get_phone() <= ph) {if(name_switch(node, name)) {node->get_key()->print_sock(sock); ifleft = ifright = 1;}} else {ifleft = 1; ifright = 0;} break;}
				case 6: {if(node->get_key()->get_phone() >= ph) {if(name_switch(node, name)) {node->get_key()->print_sock(sock); ifleft = ifright = 1;}} else {ifleft = 0; ifright = 1;} break;}
			}
			if(ifleft) print_all_name (node -> get_left(), ph, name, sock);
			if(ifright) print_all_name (node -> get_right(), ph, name, sock);

		}

		void print_all_group(node_rb *node, long long ph, int group, int sock) {
			if (!node || node == rb_tree_global -> get_nil()) return;
			int ifleft = 1, ifright = 1;
			switch(com -> c_phone) {
				case 0: {break;}
				case 2: {break;}
				case 7: {break;}
				case 1: {
					if(node->get_key()->get_phone() == ph) {
						if(group_switch(node, group))
							node->get_key()->print_sock(sock);
					} else {
						if(node->get_key()->get_phone() < ph)
							{ifleft = 0; ifright = 1;}
						else
							{ifleft = 1; ifright = 0;}
					}
					break;
				}
				case 3: {if(node->get_key()->get_phone()  < ph) {if(group_switch(node, group)) node->get_key()->print_sock(sock);} else {ifleft = 1; ifright = 0;} break;}
				case 4: {if(node->get_key()->get_phone()  > ph) {if(group_switch(node, group)) node->get_key()->print_sock(sock);} else {ifleft = 0; ifright = 1;} break;}
				case 5: {if(node->get_key()->get_phone() <= ph) {if(group_switch(node, group)) node->get_key()->print_sock(sock);} else {ifleft = 1; ifright = 0;} break;}
				case 6: {if(node->get_key()->get_phone() >= ph) {if(group_switch(node, group)) node->get_key()->print_sock(sock);} else {ifleft = 0; ifright = 1;} break;}
			}
			if(ifleft) print_all_group (node -> get_left(), ph, group, sock);
			if(ifright) print_all_group (node -> get_right(), ph, group, sock);
		}
		
		int name_switch(node_rb *node, char * name) {
			switch(com -> c_name) {
				case 0: {return 0;}
				case 7: {return 0;}
				case 1: {if(strcmp_(node->get_key()->get_name(), name) == 0) return 1; else return 0;}
				case 2: {if(strcmp_(node->get_key()->get_name(), name) == 1) return 1; else return 0;}
				case 3: {if(strcmp_(node->get_key()->get_name(), name)  < 0) return 1; else return 0;}
				case 4: {if(strcmp_(node->get_key()->get_name(), name)  > 0) return 1; else return 0;}
				case 5: {if(strcmp_(node->get_key()->get_name(), name) <= 0) return 1; else return 0;}
				case 6: {if(strcmp_(node->get_key()->get_name(), name) >= 0) return 1; else return 0;}
			}
			return 0;
		}

		int group_switch(node_rb *node, int group) {
			switch(com -> c_group) {
				case 0: {return 0;}
				case 7: {return 0;}
				case 1: {if(node->get_key()->get_group() == group) return 1; else return 0;}
				case 2: {if(node->get_key()->get_group() != group) return 1; else return 0;}
				case 3: {if(node->get_key()->get_group()  < group) return 1; else return 0;}
				case 4: {if(node->get_key()->get_group()  > group) return 1; else return 0;}
				case 5: {if(node->get_key()->get_group() <= group) return 1; else return 0;}
				case 6: {if(node->get_key()->get_group() >= group) return 1; else return 0;}
			}
			return 0;
		}

		void del(node_rb *node) {
			int t = node -> get_key() -> get_group() - 101;
			list_node *tmp = node -> get_key();
			
			hash_global -> search_and_delete(tmp);
			rb_tree_global -> deletenode(tmp);
			
			l_group[t].get_rb_tree() -> deletenode(tmp);
			l_group[t].get_hash() -> search_and_delete(tmp);
			l_group[t].delete_element(tmp);
		}
		
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

		void simple_do(int sock) {
			int i = 0;
			for(int j = 0; j < 498; j++) {
				list_node * buf = l_group[j].get_head(), *tmp = 0;						
				while (buf) {
					if (com -> operation == OP_NONE) {
						if (i == 0 && switch_do_name(buf)) i = 1;
						else if (i == 0 && switch_do_phone(buf)) i = 1;
						else if (i == 0 && switch_do_group(buf)) i = 1;
						else if (i == 0 && com -> c_name == COND_NONE && com -> c_phone == COND_NONE && com -> c_group == COND_NONE) i = 1;
					} else if(com -> operation == OR) {
						if (i == 0 && (switch_do_name(buf) || switch_do_phone(buf))) i = 1;
						else if (i == 0 && (switch_do_name(buf) || switch_do_group(buf))) i = 1;
						else if (i == 0 && (switch_do_phone(buf) || switch_do_group(buf))) i = 1;
					} else if(com -> operation == AND) {
						if (i == 0 && (switch_do_name(buf) && switch_do_phone(buf))) i = 1;
						else if (i == 0 && (switch_do_name(buf) && switch_do_group(buf))) i = 1;
						else if (i == 0 && (switch_do_phone(buf) && switch_do_group(buf))) i = 1;
					}
					if (i == 1) {
						if (com -> type == SELECT) {
							buf -> print_sock(sock);
							buf = buf -> get_next();
						} else if (com -> type == DELETE) {
							if (buf -> get_next()) tmp = buf -> get_next(); else tmp = 0;
							simple_delete(buf);
							buf = tmp;
						}
					} else if (i == 0) {
						buf = buf -> get_next();
					}
					i = 0;
				}
			}
		}
		
///////////////////////////////////////////////////////////////////////////////

		void simple_do_l_group(int sock) {
			int t = com -> get_group() - 101;
			int i = 0;
			list_node * buf = l_group[t].get_head(), *tmp = 0;					
			while (buf) {
				if (com -> operation == OP_NONE) {
					i = 1;
				} else if(com -> operation == AND) {
					if (i == 0 && (switch_do_phone(buf))) i = 1;
					else if (i == 0 && (switch_do_name(buf))) i = 1;
				}
				if (i == 1) {
					if (com -> type == SELECT) {
						buf -> print_sock(sock);
						buf = buf -> get_next();
					} else if (com -> type == DELETE) {
						if (buf -> get_next()) tmp = buf -> get_next(); else tmp = 0;
						simple_delete(buf);
						buf = tmp;
					}
				} else if (i == 0) {
					buf = buf -> get_next();
				}
				i = 0;
			}
		}

		void simple_delete(list_node *buf) {
			int t = buf -> get_group() - 101;
			
			hash_global -> search_and_delete(buf);
			rb_tree_global -> deletenode(buf);
			//rb_tree_global -> print_tree();
			
			l_group[t].get_rb_tree() -> deletenode(buf);
			l_group[t].get_hash() -> search_and_delete(buf);
			l_group[t].delete_element(buf);
		}

		
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

		int switch_do_name(list_node *buf) {
			if (com -> c_name == COND_NONE && !buf -> get_name()) return 0;
			switch (com -> c_name) {
				case 0: return 0;
				case 1: if (strcmp_(com -> get_name(), buf -> get_name()) == 0) return 1; else return 0;
				case 2: if (strcmp_(com -> get_name(), buf -> get_name()) == 1) return 1; else return 0;
				case 4: if (strcmp_(com -> get_name(), buf -> get_name())  < 0) return 1; else return 0;
				case 3: if (strcmp_(com -> get_name(), buf -> get_name())  > 0) return 1; else return 0;
				case 6: if (strcmp_(com -> get_name(), buf -> get_name()) <= 0) return 1; else return 0;
				case 5: if (strcmp_(com -> get_name(), buf -> get_name()) >= 0) return 1; else return 0;
				case 7: if (strcmp_(com -> get_name(), buf -> get_name()) == 0) return 1; else return 0;
			}
			return 0;
		}

		int switch_do_phone(list_node *buf) {
			if (com -> c_phone == COND_NONE && !buf -> get_phone()) return 0;
			switch (com -> c_phone) {
				case 0: return 0;
				case 1: if ( com -> get_phone() == buf -> get_phone() ) 	return 1; else return 0;
				case 2: if ( com -> get_phone() != buf -> get_phone() ) 	return 1; else return 0;
				case 4: if ( com -> get_phone()  < buf -> get_phone() ) 	return 1; else return 0;
				case 3: if ( com -> get_phone()  > buf -> get_phone() ) 	return 1; else return 0;
				case 6: if ( com -> get_phone() <= buf -> get_phone() ) 	return 1; else return 0;
				case 5: if ( com -> get_phone() >= buf -> get_phone() ) 	return 1; else return 0;
				case 7: if ( com -> get_phone() == buf -> get_phone() ) 	return 1; else return 0;
			}
			return 0;
		}

		int switch_do_group(list_node *buf) {
			if(com -> c_group == COND_NONE && !buf -> get_group()) return 0;
			switch(com -> c_group) {
				case 0: return 0;
				case 1: if ( com -> get_group() == buf -> get_group() ) 	return 1; else return 0;
				case 2: if ( com -> get_group() != buf -> get_group() ) 	return 1; else return 0;
				case 4: if ( com -> get_group()  < buf -> get_group() ) 	return 1; else return 0;
				case 3: if ( com -> get_group()  > buf -> get_group() ) 	return 1; else return 0;
				case 6: if ( com -> get_group() <= buf -> get_group() ) 	return 1; else return 0;
				case 5: if ( com -> get_group() >= buf -> get_group() ) 	return 1; else return 0;
				case 7: if ( com -> get_group() == buf -> get_group() ) 	return 1; else return 0;
			}
			return 0;
		}
		
		int strcmp_(char name1[], char name2[]) {
			if ( !strchr(name1, '%') && !strchr(name1, '_') ) {
				return strcmp(name1, name2);
			} else {
				for (int i = 0; name1[i] && name2[i]; i++) {
					if (name1[i] == '_') {
			
					} else if(name1[i] == '%') {
						return 0;
					} else if (name1[i] != name2[i]) 
						return strcmp(name1, name2);				
				}
			}
			return 0;
		}
};

#endif // !COMMAND_DO_H

