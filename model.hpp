#ifndef __model_hpp
#define __model_hpp

#include <memory> // shared_ptr c++11
#include <boost/numeric/ublas/matrix.hpp>

namespace FP{
  namespace ublas = boost::numeric::ublas;

  class Model; // forward declaration

  /** Pointer to Model */
  typedef std::shared_ptr<Model> ModelPtr;
  
  class Model{
  public:
    virtual ublas::matrix<double> transitionMatrix(double const& t) const = 0;
    virtual ublas::matrix<double> summaryStatistic(double const& t) const = 0;
  };


  class JukesCantor : public Model {
  public:
    virtual ublas::matrix<double> transitionMatrix(double const& t) const;
    virtual ublas::matrix<double> summaryStatistic(double const& t) const;
  };
}

#endif // __model_hpp
