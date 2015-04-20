#include "parser.hpp"

namespace FP{

  template<typename Iterator>
  bool Parser::parse(Iterator begin, Iterator end, std::vector<double> & res){
    Iterator it = begin;
    while( it != end){
      if( *it == '(' or *it == ','){
	it++;
      }
      else if( *it == ':'){
	//Apply transition i.e. factor-to-node 
	std::stringstream buf;
	while( isdigit(*++it) || *it == '.'){
	  buf << *it;
	}
	double length = stod(buf.str());
	transitionMessage(length);
      }
      else if( isalpha( *it ) ){
	//Lookup sequence
	std::stringstream buf;
	//	Iterator s_start = it;
	buf << *it;
	while( isalpha(*++it) ){
	  buf << *it;
	}
	std::string seq_name = buf.str();
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


    // Calculate summaries 
    if( mu.size() == 1 && lambda.size() == 1){
      res.push_back( sum(lambda.top()) );
    }
    else{
      std::cerr << "ERROR: parser.cpp::parse: stacks have incorrect number of messages" << std::endl;
    }

    // Clear stacks
    while( ! mu.empty()) mu.pop();
    while( ! lambda.empty()) lambda.pop();
    
    return true;
  }

  // Put a terminal on the stack
  void Parser::createMessage(std::string const& s){
    // Message of first kind + scaling
    ublas::vector<double> mu_new(4,0);
    mu_new( sequences->next(s) ) = 1;
    mu.push( mu_new );
    scaling.push( 1 );

    // Message of second kind
    ublas::vector<double> lambda_new(4,0);
    lambda.push( lambda_new );
  }

  // Use transition matrix to create factor-to-node message
  // Takes top of stack and transforms it
  void Parser::transitionMessage(double const& d){
    if( ! mu.empty() ){
      // Message of first kind + scaling
      ublas::matrix<double> transition = model->transitionMatrix(d);
      ublas::vector<double> mu_old = mu.top();
      double scaling_old = scaling.top();
      mu.pop();
      scaling.pop();
      
      // Calculate new message and normalize
      ublas::vector<double> mu_new = prod( transition, mu_old);
      double scale_factor = 1/sum(mu_new);
      double scaling_new = scaling_old + log(scale_factor);
      mu_new = mu_new * scale_factor;
      mu.push( mu_new );
      scaling.push( scaling_new );

      // Messages of second kind
      ublas::matrix<double> expected = model->summaryStatistic(d);
      ublas::vector<double> lambda_old = lambda.top();
      lambda.pop();

      // Calculate new message and normalize
      ublas::vector<double> lambda_new = prod( element_prod( expected, transition), mu_old) + prod( transition, lambda_old);
      lambda_new = lambda_new * scale_factor;
      lambda.push( lambda_new );
    }
    else{
      //TODO Error message
    }
  }

  // Take two in-messages and calculate out-message
  void Parser::mergeMessages(){
    //Message of first kind
    ublas::vector<double> mu1_old = mu.top();
    double scaling_old_1 = scaling.top();
    mu.pop();
    scaling.pop();
    ublas::vector<double> mu2_old = mu.top();
    double scaling_old_2 = scaling.top();
    mu.pop();
    scaling.pop();

    // Calculate new message and normalize
    ublas::vector<double> mu_new = element_prod(mu1_old, mu2_old);
    double scale_factor = 1/sum(mu_new);
    double scaling_new = scaling_old_1 + scaling_old_2 + log(scale_factor);
    mu_new = mu_new * scale_factor;
    mu.push( mu_new );
    scaling.push( scaling_new );

    // Message of second kind
    ublas::vector<double> lambda1_old = lambda.top();
    lambda.pop();
    ublas::vector<double> lambda2_old = lambda.top();
    lambda.pop();

    // Calcuate new message and normalize
    ublas::vector<double> lambda_new = element_prod(mu1_old, lambda2_old) + element_prod(mu2_old, lambda1_old);
    lambda_new = lambda_new * scale_factor;
    lambda.push( lambda_new );
  }

  //Instantiation
  template bool Parser::parse(char const*, char const*, std::vector<double> &);
  template bool Parser::parse(std::istream_iterator<char>, std::istream_iterator<char>, std::vector<double> & );
} //namespace FP


