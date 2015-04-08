#include "model.hpp"


namespace FP{
  ublas::matrix<double> JukesCantor::transitionMatrix(double const& t) const{
    ublas::matrix<double> ret(4, 4, 1./4 - exp(-4*t)/4 );
    ret = ret +exp(-4*t)*ublas::identity_matrix<double>(4);

    return ret;
  }

  ublas::matrix<double> JukesCantor::summaryStatistic(double const& t) const{
    ublas::matrix<double> ret(4,4, (t/4*exp(-4*t)+t*3/4)/(1./4-exp(-4*t)/4) );
    ret = ret + (-ret(1,1)+(-t*3/4*exp(-4*t) + t*3/4)/(1./4+3*exp(-4*t)/4) )*ublas::identity_matrix<double>(4);

    return ret;
  }
}
