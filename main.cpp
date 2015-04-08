#include <iostream>
#include <vector>
#include <memory>
#include <fstream>

#include "parser.hpp"
#include "model.hpp"

namespace ublas = boost::numeric::ublas;

int main(int argc, char* argv[]){
  //Setup parser
  FP::Parser parser;
  parser.model = std::make_shared<FP::JukesCantor>();

  // Read newick from file stream
  std::fstream file("testcases/test1.nw");
  std::istream_iterator<char> fileIterator(file);

  //Setup sequence information
  parser.sequences.insert( std::pair<std::string, int>("A", 1) );
  parser.sequences.insert( std::pair<std::string, int>("C", 1) );
  parser.sequences.insert( std::pair<std::string, int>("BEBE", 3) );

  // Test
  std::vector<double> summary;
  parser.parse<std::istream_iterator<char> >(fileIterator, std::istream_iterator<char>(), summary);

  std::cout << parser.mu.top() << std::endl;
  std::cout << parser.lambda.top() << std::endl;

  return 0;
}
