test: test.cpp model.cpp parser.cpp model.hpp parser.hpp
	g++ --std=c++11 -lboost_unit_test_framework parser.cpp model.cpp test.cpp -otest

main: main.cpp model.cpp parser.cpp model.hpp parser.hpp
	g++ --std=c++11 -ggdb parser.cpp model.cpp main.cpp -omain
