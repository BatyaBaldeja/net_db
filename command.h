#ifndef COMMAND_H
#define COMMAND_H

#include "record.h"
	

class command: public record {
	private:
		cmd_type 	type 		= CMD_NONE;
		cond_type 	c_name 		= COND_NONE;
		cond_type 	c_phone 	= COND_NONE;
		cond_type 	c_group 	= COND_NONE;
		op_type 	operation	= OP_NONE;
		friend class command_do;

	public:
		command() : record() {}
		~command() {}
		cmd_type get_type() {return type;}
		void free_command();
		int command_read();
		int command_read(char * buf);
		int parse(char *str_words[]);
		void type_insert(char *str_words[]);
		void type_select_delete(char *str_words[]);
		cond_type cond_type_f(char *str_words);
		void print_command();
		int take_print_arg(int type);
		void args_switch(cond_type t);
};

#endif // !COMMAND_H
