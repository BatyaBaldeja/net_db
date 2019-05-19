#ifndef RECORD_H
#define RECORD_H

#include "string.h"


int write_to_socket(int fd, const char* buf);

class record {
	private:
		std::unique_ptr <char[]> name = nullptr;
		long long phone = 0;
		int group = 0;
		
		friend class command;

	public:
		record(char * name = nullptr, long long phone = 0, int group = 0) {init(name, phone, group);}
		record(record & rhs) {this -> init(rhs.name.get(), rhs.phone, rhs.group);}
		~record() {}
		
		char * get_name() {return name.get();}
		long long get_phone() {return phone;}
		int get_group() {return group;}
		
		record & move(record & rhs);
		record & copy(record &rhs); 
		void swap(record & rhs) {std::swap(name, rhs.name); std::swap(phone, rhs.phone); std::swap(group, rhs.group);}
		int read(FILE * fp = stdin);
		int init(char *name, long long phone, int group);
		
		void print(FILE * fp = stdout) {fprintf(fp, "%15s %10.1lld %6d\n", name.get(), phone, group);}
		void print_(FILE * fp = stdout) {fprintf(fp, "%s %.1lld %d ", name.get(), phone, group);}
		void print_name(FILE * fp = stdout) {fprintf(fp, "%15s\n", name.get());}
		void print_phone(FILE * fp = stdout) {fprintf(fp, "%10.1lld\n", phone);}
		void print_group(FILE * fp = stdout) {fprintf(fp, "%6d\n", group);}
		
		void print_sock(int sock) {
			char buf[LEN];
			sprintf(buf, "%15s %10.1lld %6d\n", name.get(), phone, group);
			write_to_socket(sock, buf);
		}
		
		int cmp (const record &b);
		record& operator = (const record &rhs);
		bool operator < (const record &b);
		bool operator > (const record &b);
		bool operator == (const record &b);
		bool operator <= (const record &b);
		bool operator >= (const record &b);
};

#endif // !RECORD_H
