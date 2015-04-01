#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_char_class.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>

#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <map>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace ublas = boost::numeric::ublas;

template<typename Iterator>
struct Newick : qi::grammar<Iterator, double()>
{
  Newick() : Newick::base_type(start){
    using qi::eps;
    using qi::lit;
    using qi::double_;
    using qi::_val;
    using qi::_1;
    using ascii::char_;
    using ascii::alpha;

    start = *(lit('(') 
	      | lit(',')
	      | lit(':') >> double_[ &transitionMessage ] 
	      | lit(')') [ &mergeMessages ] 
	      | (+alpha) [ &createMessage] )
      >> lit(';')
      ;
  }

  // Parser
  qi::rule<Iterator, double()> start;

  // Parser function
  static int count;

  // Put a terminal on the stack
  static void createMessage(std::vector<char> const& v){
    std::string str(v.begin(),v.end());

    if( sequences.find( str ) != sequences.end() ){
      std::cout << "Putting message on stack : " << str << std::endl;
      ublas::vector<double> m(4,0);
      m( sequences.find( str )->second ) = 1;
      messages.push(m);
    }
    else{
      std::cout << "Sequence information not found" << std::endl;
    }
  }

  // Use transition matrix to create factor-to-node message
  // Takes top of stack and transforms it
  static void transitionMessage(double const& d){
    if( ! messages.empty() ){
      ublas::matrix<double> transition(4, 4, 1./4 - exp(-d)/4 );
      transition = transition +exp(-d)*ublas::identity_matrix<double>(4);
      ublas::vector<double> v = messages.top();
      messages.pop();
      messages.push( prod( transition, v) );
    }
    else{
      //TODO Error message
    }
    std::cout << "Transition matrix being applied" << std::endl;
  }

  // Take two in-messages and calculate out-message
  static void mergeMessages(){
    if( messages.size() >= 2){
      std::cout << "Merging messages" << std::endl;
      ublas::vector<double> v1 = messages.top();
      messages.pop();
      ublas::vector<double> v2 = messages.top();
      messages.pop();
      messages.push( element_prod(v1, v2) );
    } else{
      //TODO Error message
    }    
  }

  // Stack for messages
  static std::stack< ublas::vector<double> > messages;

  // Sequence information // Could be replaced by a stream
  static std::map<std::string, int> sequences;

};

//Initialize static members
template<typename Iterator>
int Newick<Iterator>::count = 0;
template<typename Iterator>
std::stack< ublas::vector<double> > Newick<Iterator>::messages;
template<typename Iterator>
std::map<std::string, int> Newick<Iterator>::sequences;


int main(int argc, char* argv[]){
  //Setup parser
  using boost::spirit::qi::parse;
  typedef Newick<char const *> NewickChar;
  NewickChar newick_parser;

  //Setup newick string
  char const *first = "((A:0.15,C:0.50):0.9,BEBE:1);", *last = first + std::strlen(first);

  //Setup sequence information
  NewickChar::sequences.insert( std::pair<std::string, int>("A", 1) );
  NewickChar::sequences.insert( std::pair<std::string, int>("C", 1) );
  NewickChar::sequences.insert( std::pair<std::string, int>("BEBE", 3) );

  //Parse and calculate
  double res = 0;
  bool r = parse(first, last, newick_parser, res);

  //Report
  if(r) 
    std::cout << "Success\n";
  else
    std::cout << "Failure\n";
  
  //See stack
  while( ! NewickChar::messages.empty() ){
    std::cout << "Top: " << NewickChar::messages.top() << std::endl;
    NewickChar::messages.pop();
  }
  return 0;
}
