test: test.cpp model.cpp sequence.cpp parser.cpp model.hpp sequence.hpp parser.hpp
	g++ -g --std=c++11 -lboost_unit_test_framework parser.cpp sequence.cpp model.cpp test.cpp -otest

main: main.cpp model.cpp sequence.cpp parser.cpp model.hpp sequence.hpp parser.hpp
	g++ -g --std=c++11 -lboost_program_options parser.cpp sequence.cpp model.cpp main.cpp -omain
