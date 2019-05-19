all: server client

server: command_do.h group_db.h hash_fun.h list_node.h rb_tree.h record.h record_list.h command.cpp command.h hash_fun.cpp main.cpp rb_tree.cpp record.cpp record_list.cpp string.h

	g++  -std=c++11 -o server command.cpp main.cpp rb_tree.cpp record.cpp record_list.cpp hash_fun.cpp
	

client: client.cpp
	g++  -std=c++11 -o client client.cpp
