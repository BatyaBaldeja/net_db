#ifndef STRING_H
#define STRING_H

#include <algorithm>
#include <memory>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include <errno.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define LEN 1000


enum cmd_type {
	CMD_NONE, QUIT, STOP, SELECT, INSERT, DELETE
	};
	
enum cond_type {
	COND_NONE, EQ, NE, LT, GT, LE, GE, LIKE
	};
	
enum op_type {
	OP_NONE, AND, OR
	};


class string {
	public:
		char current_string[LEN] = { 0 };
		char t[LEN] = {' ', '	', '\t', '\n', '\r', ',', '(', ')', '{', '}', ';'};
		char *words_current_string[LEN] = { 0 }; 
		int len_words = 0;

	public:
		string() {}
		~string() {}

		int words(FILE *fp = stdin) {
			char * tmp = nullptr;
			for (int i = 0; i < len_words; i++)
				words_current_string[i] = { 0 }; 
			len_words = 0;
			if ( !fgets(current_string, LEN, fp) )
				return 1;
			if( (words_current_string[0] = strtok(current_string, t)) != 0 ) {
				tmp = strtok(NULL, t); 
				len_words++;
				for (int i = 1; tmp; i++) {
					words_current_string[i] = tmp;
					len_words++;
					tmp = strtok(NULL, t);
				}
			} else 
				return 2;
			return 0;
		}
		
		int words(char * buf) {
			char * tmp = nullptr;
			for (int i = 0; i < len_words; i++)
				words_current_string[i] = { 0 }; 
			len_words = 0;

			if( (words_current_string[0] = strtok(buf, t)) != 0 ) {
				tmp = strtok(NULL, t); 
				len_words++;
				for (int i = 1; tmp; i++) {
					words_current_string[i] = tmp;
					len_words++;
					tmp = strtok(NULL, t);
				}
			} else 
				return 2;
			return 0;
		}
		
		void print_words(FILE *fp = stdout) {
			for (int i = 0; words_current_string[i]; i++)
				fprintf(fp, "\"%s\"\n", words_current_string[i]);
		}
};

#endif // !STRING_H
