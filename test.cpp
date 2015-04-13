#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TEST

// Boost test
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

// IO
#include <boost/numeric/ublas/io.hpp>
#include <iostream>
#include <fstream>

#include "model.hpp"
#include "parser.hpp"
#include "sequence.hpp"

namespace ublas = boost::numeric::ublas;

BOOST_AUTO_TEST_CASE(JukesCantor_1){
  FP::JukesCantor jc;

  // Test Jukes Cantor transition probabilities
  ublas::matrix<double> mat = jc.transitionMatrix(0);
  BOOST_CHECK_CLOSE( mat(0,0), 1, 0.0001);

  // Test Jukes Cantor expected substitutions
  ublas::matrix<double> matExp = jc.summaryStatistic(1);
  BOOST_CHECK_CLOSE( matExp(0,1), 3.074629, 0.0001);
  BOOST_CHECK_CLOSE( matExp(0,0), 2.79166 , 0.0001);
  std::cout << matExp << std::endl;
}

BOOST_AUTO_TEST_CASE(SequenceMap_1){
  std::shared_ptr<FP::SequenceMap> sm(new FP::SequenceMap());
  sm->addSeq("A", 1);
  sm->addSeq("B", 2);

  BOOST_CHECK_EQUAL( sm->next("A"), 1);
  BOOST_CHECK_EQUAL( sm->next("B"), 2);
}

BOOST_AUTO_TEST_CASE(SequenceStream_1){
  std::shared_ptr<FP::SequenceStream> ss(new FP::SequenceStream("testcases/seq1.txt"));
  BOOST_CHECK_EQUAL( ss->next("A"), 1);
  BOOST_CHECK_EQUAL( ss->next("A"), 2);
  BOOST_CHECK_EQUAL( ss->next("A"), 0);

}

BOOST_AUTO_TEST_CASE(Parser_JC_1){
  // Setup parser
  FP::Parser parser;
  parser.model = std::make_shared<FP::JukesCantor>();

  // Read newick from file stream
  std::fstream file("testcases/test1.nw");
  std::istream_iterator<char> fileIterator(file);

  // Setup sequences
  std::shared_ptr<FP::SequenceMap> sm(new FP::SequenceMap());
  sm->addSeq("A", 1);
  parser.sequences = sm;

  // Parse
  std::vector<double> summary;
  parser.parse<std::istream_iterator<char> >(fileIterator, std::istream_iterator<char>(), summary);

  // Test
  BOOST_CHECK_CLOSE( summary.at(0) , 2.25, 0.0001);
}


BOOST_AUTO_TEST_CASE(Parser_JC_2){
  // Setup parser
  FP::Parser parser;
  parser.model = std::make_shared<FP::JukesCantor>();

  // Read newick from file stream
  std::fstream file("testcases/test2.nw");
  std::istream_iterator<char> fileIterator(file);

  // Setup sequences
  std::shared_ptr<FP::SequenceMap> sm(new FP::SequenceMap());
  sm->addSeq("A", 1);
  sm->addSeq("B", 2);
  parser.sequences = sm;

  // Parse
  std::vector<double> summary;
  parser.parse<std::istream_iterator<char> >(fileIterator, std::istream_iterator<char>(), summary);

  // Test
  std::cout << summary.at(0) << std::endl;
  BOOST_CHECK_CLOSE(summary.at(0), 1.813035285, 0.0001);
}

BOOST_AUTO_TEST_CASE(Parser_JC_3){
  // Check results independent of order
  // Setup parser
  FP::Parser parser;
  parser.model = std::make_shared<FP::JukesCantor>();

  // Read newick from file stream
  std::fstream file1("testcases/test3.nw");
  std::fstream file2("testcases/test4.nw");

  std::istream_iterator<char> fileIterator1(file1);
  std::istream_iterator<char> fileIterator2(file2);

  // Setup sequences
  std::shared_ptr<FP::SequenceMap> sm(new FP::SequenceMap());
  sm->addSeq("A", 1);
  sm->addSeq("B", 2);
  sm->addSeq("C", 0);
  parser.sequences = sm;

  // Parse
  std::vector<double> summary1;
  parser.parse<std::istream_iterator<char> >(fileIterator1, std::istream_iterator<char>(), summary1);

  std::vector<double> summary2;
  parser.parse<std::istream_iterator<char> >(fileIterator2, std::istream_iterator<char>(), summary2);

  // Test
  BOOST_CHECK_CLOSE( summary1.at(0) , summary2.at(0), 0.0001);
}

