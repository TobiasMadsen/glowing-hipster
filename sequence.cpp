#include <iostream>

#include "sequence.hpp"

namespace FP{
  // Implement sequence map
  int SequenceMap::next(std::string s){
    if( sequences.find( s ) != sequences.end() ){
      return sequences.find(s)->second;
    }
    else{
      std::cerr << "Error: SequenceMap::next: Sequence not found" << std::endl;
      return 0;
    }
  }

  void SequenceMap::addSeq(std::string name, int value){
    sequences.insert( std::pair<std::string, int>(name, value) );
  }

  // Implement sequence stream
  SequenceStream::SequenceStream(std::string filename) : file(filename), fileIterator(file){ }
  int SequenceStream::next(std::string){
    return (*fileIterator++)-48;
  }
}
