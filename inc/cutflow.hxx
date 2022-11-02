
#ifndef cutflow_hxx
#define cutflow_hxx

#include "common.hxx"
#include "utils.hxx"

extern "C" class cutflow {

  public:
    
    // member variables
    int cuts;
    std::string var_str, output_str;
    std::vector< std::string > cut_names;
    TH1F * cutflow_hist;
    double var;
    bool distribution;
    std::vector< TH1F* > cut_distributions;

    //constructors
    cutflow(){};
    cutflow( int cuts, std::string cut_names, bool distribution=false, std::string var_name="", double var=0 ){
        
      this->cuts = cuts;
      split_strings( this->cut_names, cut_names, ":" );
      cutflow_hist = new TH1F("Cutflow", "", cuts, 0, cuts);

      if ( distribution ){
        this->var = var;
        for ( int cut_idx = 1; cut_idx <= cuts; cut_idx++ ){
          cut_distributions.push_back( new TH1F( Form( "%s_cut_%i", var_name.c_str(), cut_idx ), "", 50, -100, 100 ) );
        }
      }
    }

    // Fill a cutflow step
    void fill( int cut ){
      cutflow_hist->Fill( cut-1 );
      if ( distribution ){ cut_distributions.at( cut-1 )->Fill( var ); }
    }

    // set the variable to produce with cutflow  
    void set_var( double var ){ this->var = var; }

    // write out the cutflow
    void write( std::string unique );
    
};

#endif
