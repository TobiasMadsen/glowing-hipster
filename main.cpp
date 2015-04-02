#include <iostream>
#include <vector>

#include "parser.hpp"

namespace ublas = boost::numeric::ublas;

int main(int argc, char* argv[]){
  //Setup parser
  FP::Parser<char const *> parser;

  //Setup newick string
  char const *first = "(A:0.75,C:0.75);", *last = first + std::strlen(first);

  //Setup sequence information
  parser.sequences.insert( std::pair<std::string, int>("A", 1) );
  parser.sequences.insert( std::pair<std::string, int>("C", 1) );
  parser.sequences.insert( std::pair<std::string, int>("BEBE", 3) );

  std::vector<double> summary;
  parser.parse(first, last, summary);

  std::cout << parser.mu.top() << std::endl;
  std::cout << parser.lambda.top() << std::endl;

  return 0;
}
