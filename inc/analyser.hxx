
#ifndef analyser_hxx
#define analyser_hxx

#include "common.hxx"
#include "utils.hxx"

// Class for analysing histograms with numerous styles of fit.
class analyser{

  private:
    TH1 * histogram;
    std::vector< TF1 * > functions

  public:

    analyser(){
      functions = new std::vector< TF1 * >;
    }

    analyser( TH1 * histogram ) : analyser(){
      this->histogram = ( TH1 * ) histogram->Clone();
    }





};



#endif

