#include "record.h"

record &
record::move(record &rhs) {
	if (this == &rhs)
		return *this;
	name = std::move(rhs.name);
	phone = rhs.phone; group = rhs.group;
	rhs.phone = rhs.group = 0;
	return *this;
}


record &
record::copy(record &rhs) {
	if (this == &rhs)
		return *this;
	init(rhs.get_name(), rhs.get_phone(), rhs.get_group());
	return *this;
}

int
record::init(char *name, long long phone, int group) {
	if(name) {
		this -> name = std::unique_ptr <char[]> (new char[strlen(name) + 1]);
		if(!this -> name) {
			printf("Error init name!\n"); 
			return 1;
		}
		strcpy((this -> name).get(), name);
	} else
		name = nullptr;
	this -> phone = phone;
	this -> group = group;
	return 0;
}

int
record::read(FILE *fp) {
	string str;
	switch (str.words(fp)) {
		case 0: return init(str.words_current_string[0], (long long)atof(str.words_current_string[1]), atoi(str.words_current_string[2]));
		case 1: return 1;
		case 2: return 2;
	}
	return 2;
}

int
record::cmp (const record &b){
	int res;
	if (name && b.name) {
		res = strcmp (name.get(), b.name.get());
		if (res) return res;
		res = group - b.group;
		if (res) return res;
		return phone - b.phone;
	}
	if (!name && b.name) return -1;
	if (name && !b.name) return 1;
	res = group - b.group;
	if (res) return res;
	return phone - b.phone;
}

record &
record::operator = (const record &rhs){
	init (rhs.name.get(), rhs.phone, rhs.group);
	return *this;
}

bool
record::operator < (const record &b) {
	if (this -> cmp(b) < 0)
		return true;
	else
		return false;
}

bool
record::operator > (const record &b) {
	if (this -> cmp(b) > 0)
		return true;
	else
		return false;
}

bool
record::operator <= (const record &b) {
	if (this -> cmp(b) <= 0)
		return true;
	else
		return false;
}

bool
record::operator >= (const record &b) {
	if (this -> cmp(b) >= 0)
		return true;
	else
		return false;
}

bool
record::operator == (const record &b) {
	if (this -> cmp(b) == 0)
		return true;
	else
		return false;
}

