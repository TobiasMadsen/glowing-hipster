#ifndef __parser_h
#define __parser_h

#include <stack>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>

#include "model.hpp"

namespace FP{
  namespace ublas = boost::numeric::ublas;

  class Parser{
  public:
    template<typename Iterator>
    bool parse(Iterator begin, Iterator end, std::vector<double> & res);
    
  private:
    // Parsing actions
    void transitionMessage(double const& d);
    void mergeMessages();
    void createMessage(std::string const& s);

    // Stacks for messages
  public: //Only for Dev
    std::stack< ublas::vector<double> > mu;
    std::stack< ublas::vector<double> > lambda;

    // Sequence information // Consider refactoring
    std::map<std::string, int> sequences;

    //Pointer to model
    ModelPtr model;
  };
}

#endif  // __parser_h
