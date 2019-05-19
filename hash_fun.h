#ifndef HASH_FUN_H
#define HASH_FUN_H

#include "list_node.h"

#define CHARACTER_NAME 52
//#define HASH_TABLE_SIZE_NAME 27556
#define HASH_TABLE_SIZE_NAME 140608

#define CHARACTER_PHONE 10
#define HASH_TABLE_SIZE_PHONE 100

#define CHARACTER_GROUP 10
#define HASH_TABLE_SIZE_GROUP 100

#define MAX_PRINT_HASH 10
#define MAX_PRINT_HASH_L 3

class hash_fun_node {
	private:
		list_node *list_node_hash;
		hash_fun_node *next;
		friend class hash_fun;
		
	public:
		hash_fun_node() {list_node_hash = 0; next = 0;}
		~hash_fun_node() {list_node_hash = 0; next = 0;}
		
		void set_list_node_hash(list_node * lnh) {list_node_hash = lnh; return;}
		list_node * get_list_node_hash() {return list_node_hash;}
		hash_fun_node * get_next() {return next;}
		
		void print_node() {if (list_node_hash) list_node_hash -> print(); return;}
		void delete_from_hash() {list_node_hash = 0; return;}
};


class hash_fun {
	private:
		hash_fun_node * head;
		int hash_table_size;
		int type;

	public:
		hash_fun(int type);
		~hash_fun() {delete_hash_fun(); head = 0; type = hash_table_size = 0;}
		
		hash_fun_node * get_head() {return head;}
		
		void delete_hash_fun();

        /*int hash(char * name) {
			int number = 0;
            if (name[0] && name[1])
				number = (int) (CHARACTER_NAME * name[0] + name[1]);
			else if(name[0])
				number = (int) name[0];
			return number;
        }*/
        int hash(char * name) {
                    int number = 0;
                    if (name[0] && name[1] && name[2])
                        number = (int) (CHARACTER_NAME * CHARACTER_NAME * (name[0] - 65) + CHARACTER_NAME * (name[1] - 65) + (name[2] - 65));
                    else if(name[0] && name[1])
                        number = (int) (CHARACTER_NAME * (name[0] - 52) + (name[1] - 52));
                    else if(name[0] && name[1])
                        number = (int) (name[0] - 52);
                    //printf("%d ", number);
                    return number;
                }

		int hash(long long phone) {return phone % 100;}
		int hash(int group) {return group % 100;}
		
		int search(list_node * buf);
		int search_and_delete(list_node * buf);
		int add_new(list_node * buf);
		
		int cmp_(list_node *tmp1, list_node *tmp2);
		int name_cmp(char *name1, char *name2) {return hash(name1) - hash(name2);}
		int phone_cmp(long long phone1, long long phone2) {return hash(phone1) - hash(phone2);}
		int group_cmp(int group1, int group2) {return hash(group1) - hash(group2);}
		
		void print_hash();
};

#endif // !HASH_FUN_H

