#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <map>
#include <string>

#include <boost/program_options.hpp>
#include "parser.hpp"
#include "model.hpp"
#include "sequence.hpp"

namespace ublas = boost::numeric::ublas;

int main(int argc, char* argv[]){
  //Setup parser
  // Program options
  namespace po = boost::program_options;
  std::string treeFilename, seqFilename;
  po::options_description desc("Options:");
  desc.add_options()
    ("help,h", "produce help message")
    ("tree,t", po::value<std::string>(& treeFilename)->default_value(""), "treefile in newick format")
    ("seq,s", po::value<std::string>(& seqFilename)->default_value(""), "sequence file, if not specified dummy sequences will be used");

  // setting up options parser
  po::options_description cmdline_options;
  cmdline_options.add(desc);

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(cmdline_options).run(), vm);
  po::notify(vm);

  // print help message
  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return 1;
  }

  // Setup parser
  FP::Parser parser;
  parser.model = std::make_shared<FP::JukesCantor>();

  // Read newick from file stream
  std::fstream file(treeFilename);
  if(!file){
    //No filestream
    std::cerr << "Newick file could not be read" << std::endl;
    return 1;
  }
  std::istream_iterator<char> fileIterator(file);
  

  // Setup sequence information
  if( seqFilename != ""){
    std::shared_ptr<FP::SequenceStream> ss(new FP::SequenceStream(seqFilename));
    parser.sequences = ss;
  } else{
    std::shared_ptr<FP::SequenceDummy> sd(new FP::SequenceDummy());
    parser.sequences = sd;
  }

  // Calculate summaries
  std::vector<double> summary;
  parser.parse<std::istream_iterator<char> >(fileIterator, std::istream_iterator<char>(), summary);


  return 0;
}
