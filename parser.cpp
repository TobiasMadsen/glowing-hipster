#include "parser.hpp"

namespace FP{

  template<typename Iterator>
  bool Parser<Iterator>::parse(Iterator begin, Iterator end, std::vector<double> & res){
    Iterator it = begin;
    while( it != end){
      if( *it == '(' or *it == ','){
	it++;
      }
      else if( *it == ':'){
	//Apply transition i.e. factor-to-node 
	Iterator d_start = ++it;
	while( isdigit(*++it) || *it == '.'){}
	double length = stod(std::string(d_start, it));
	transitionMessage(length);
      }
      else if( isalpha( *it ) ){
	//Lookup sequence
	Iterator s_start = it;
	while( isalpha(*++it) ){}
	std::string seq_name = std::string(s_start, it);
	createMessage( seq_name );
      }
      else if( *it == ')' ){
	mergeMessages();
	it++;
      }
      else if( *it == ';' )
	break;
      else{
	std::cerr << "Could not be parsed" << std::endl;
	return false;
      }
    }

    std::cout << std::endl;
    return true;
  }

  // Put a terminal on the stack
  template<typename Iterator>
  void Parser<Iterator>::createMessage(std::string const& s){
    if( sequences.find( s ) != sequences.end() ){
      // Message of first kind
      std::cout << "Putting message on stack : " << s << std::endl;
      ublas::vector<double> m(4,0);
      m( sequences.find( s )->second ) = 1;
      mu.push(m);

      // Message of second kind
      ublas::vector<double> la(4,0);
      lambda.push(la);
    }
    else{
      std::cout << "Sequence information not found" << std::endl;
    }
  }

  // Use transition matrix to create factor-to-node message
  // Takes top of stack and transforms it
  template<typename Iterator>
  void Parser<Iterator>::transitionMessage(double const& d){
    if( ! mu.empty() ){
      // Message of first kind
      ublas::matrix<double> transition(4, 4, 1./4 - exp(-d)/4 );
      transition = transition +exp(-d)*ublas::identity_matrix<double>(4);
      ublas::vector<double> v = mu.top();
      mu.pop();
      mu.push( prod( transition, v) );

      // Messages of second kind
      ublas::matrix<double> expected(4,4, 1);
      expected = expected - ublas::identity_matrix<double>(4);
      ublas::vector<double> la = lambda.top();
      lambda.pop();
      lambda.push( prod( element_prod( expected, transition), v) + prod( transition, la) );
    }
    else{
      //TODO Error message
    }
    std::cout << "Transition matrix being applied" << std::endl;
  }

  // Take two in-messages and calculate out-message
  template<typename Iterator>
  void Parser<Iterator>::mergeMessages(){
    //Message of first kind
    std::cout << "Merging messages" << std::endl;
    ublas::vector<double> mu1 = mu.top();
    mu.pop();
    ublas::vector<double> mu2 = mu.top();
    mu.pop();
    mu.push( element_prod(mu1, mu2) );

    //Message of second kind
    ublas::vector<double> la1 = lambda.top();
    lambda.pop();
    ublas::vector<double> la2 = lambda.top();
    lambda.pop();
    lambda.push( element_prod(mu1, la2) + element_prod(mu2, la1) );
  }

  // Explicit instantiation
  template class Parser<char const *>;
} //namespace FP


