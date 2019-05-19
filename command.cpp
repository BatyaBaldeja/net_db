#include "command.h"
		
int command::command_read() {	
	string str;
	if (!str.words())
        return parse(str.words_current_string);
    return 1;
}

int command::command_read(char * buf) {	
	string str;
	if (!str.words(buf))
        return parse(str.words_current_string);
    return 1;
}
	
int command::parse(char *str_words[]) {
	for (int i = 0; str_words[i]; i++) {
		if ( !strcmp(str_words[0], "quit") ) 	{type = QUIT; return 0;}
        if ( !strcmp(str_words[0], "stop") ) 	{type = STOP; return 0;}
		if ( !strcmp(str_words[0], "insert") ) 	{type = INSERT; type_insert(str_words); return 1;}
		if ( !strcmp(str_words[0], "select") ) 	{type = SELECT; type_select_delete(str_words); return 1;}
		if ( !strcmp(str_words[0], "delete") ) 	{type = DELETE; type_select_delete(str_words); return 1;}
		if ( str_words[0] ) 					{type = CMD_NONE; return 1;}
	}
	return 1;
}

void command::type_insert(char *str_words[]) {
	if (str_words[1]) {
		this -> name = std::unique_ptr<char[]> (new char[strlen(str_words[1]) + 1]);
		if ( !this -> name ) {printf("Error init name!\n"); return;}
		strcpy((this -> name).get(), str_words[1]);
	}
	if (str_words[2]) phone = (long long)atof(str_words[2]);
	if (str_words[3]) group = atoi(str_words[3]);
	return;			
}

void command::type_select_delete(char *str_words[]) {
	int i = 0;
	if ( !strcmp(str_words[0], "delete") ) i = -1; else i = 0;
	if ( strcmp(str_words[0], "delete") && str_words[1] && strcmp(str_words[1], "*") ) return;	
	if ( str_words[2 + i] && strcmp(str_words[2 + i], "where") ) return;
	if ( str_words[3 + i] && !strcmp(str_words[3 + i], "name") ) {
		if (str_words[4 + i]) c_name = cond_type_f(str_words[4 + i]);
		if (str_words[5 + i]) {
			if (name.get()) strcpy(name.get(), str_words[5 + i]); 
			else {
				this -> name = std::unique_ptr<char[]> (new char[strlen(str_words[5 + i]) + 1]);
				if ( !this -> name ) {printf("Error init name!\n"); return;}
				strcpy((this -> name).get(), str_words[5 + i]);
			}
		}
	} else if (str_words[3 + i] && !strcmp(str_words[3 + i], "phone")) {
		if (str_words[4 + i]) c_phone = cond_type_f(str_words[4 + i]); 	
		if (str_words[5 + i]) phone = (long long)atof(str_words[5 + i]);
	} else if (str_words[3 + i] && !strcmp(str_words[3 + i], "group")) {
		if (str_words[4 + i]) c_group = cond_type_f(str_words[4 + i]); 	
		if (str_words[5 + i]) group = atoi(str_words[5 + i]);
	}
	if (str_words[6 + i]) {
		if (!strcmp(str_words[6 + i], "and")) {operation = AND;} 
		else if (!strcmp(str_words[6 + i], "or")) {operation = OR;}
	}
	if (str_words[7 + i] && !strcmp(str_words[7 + i], "name")) {
		if (str_words[8 + i]) c_name = cond_type_f(str_words[8 + i]);
		if (str_words[9 + i]) {
			if (name.get()) strcpy(name.get(), str_words[9 + i]); 
			else {
				this -> name = std::unique_ptr<char[]> (new char[strlen(str_words[9 + i]) + 1]);
				if (!this -> name) {printf("Error init name!\n"); return;}
				strcpy((this -> name).get(), str_words[9 + i]);
			}
		}
	} else if (str_words[7 + i] && !strcmp(str_words[7 + i], "phone")) {
		if(str_words[8 + i]) c_phone = cond_type_f(str_words[8 + i]); 	
		if(str_words[9 + i]) phone = (long long)atof(str_words[9 + i]);
	} else if (str_words[7 + i] && !strcmp(str_words[7 + i], "group")) {
		if(str_words[8 + i]) c_group = cond_type_f(str_words[8 + i]); 	
		if(str_words[9 + i]) group = atoi(str_words[9 + i]);
	}
}
		
cond_type command::cond_type_f(char *str_words) {
	if ( !strcmp(str_words, "=") ) 		return EQ;
	if ( !strcmp(str_words, "<>") ) 	return NE;
	if ( !strcmp(str_words, ">") ) 		return GT;
	if ( !strcmp(str_words, "<") ) 		return LT;
	if ( !strcmp(str_words, ">=") ) 	return GE;
	if ( !strcmp(str_words, "<=") ) 	return LE;
	if ( !strcmp(str_words, "like") ) 	return LIKE; 
		return COND_NONE;
}

void command::print_command() {
	int tmp = 3;
	if (type == QUIT) {
		printf("quit;\n\n");
	} else if (type == INSERT) {
		printf("insert {%s, %.0lld, %d};\n", name.get(), phone, group);
	} else if (type == SELECT) {
		printf("select * where "); 
		tmp = take_print_arg(3);
		if (operation != OP_NONE) {
		  if (operation == AND) printf(" and "); 
		  else if (operation == OR) printf(" or ");
		} else {
			printf(";\n"); 
			return;
		}
		take_print_arg(tmp);
		printf(";\n");
	} else if (type == DELETE) {
		printf("delete where "); 
		tmp = take_print_arg(3);
		if (operation != OP_NONE) {
		if (operation == AND) printf(" and ");
		else if (operation == OR) printf(" or ");
		} else {
			printf(";\n");
			return;
		}
		take_print_arg(tmp);
		printf(";\n");
	}
}
		
int command::take_print_arg(int type) {
	if(c_name != COND_NONE && type != 0) {
		printf("name ");
		args_switch(c_name);
		printf("%s", name.get());
		return 0;
	} else if (c_phone != COND_NONE && type != 1) {
		printf("phone "); 
		args_switch(c_phone); 
		printf("%lld", phone); 
		return 1;
	} else if (c_group != COND_NONE && type != 2) {
		printf("group ");
		args_switch(c_group);
		printf("%d", group);
		return 2;
	}
	return 3;
}
		
void command::args_switch(cond_type t) {	
	if (t == COND_NONE) return;
	switch (t) {
		case 0: return;
		case 1: printf("= "); 		return;
		case 2: printf("<> "); 		return;
		case 3: printf("< "); 		return;
		case 4: printf("> "); 		return;
		case 5: printf("<= "); 		return;
		case 6: printf(">= "); 		return;
		case 7: printf("like "); 	return;	
	}
	return;
}
		
void command::free_command() {
	type 		= CMD_NONE;
	c_name 		= COND_NONE;
	c_phone 	= COND_NONE;
	c_group 	= COND_NONE;
	operation	= OP_NONE;
	phone = 0; 
	group = 0;
	name = nullptr;
}
	
