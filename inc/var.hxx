

#ifndef var_hxx
#define var_hxx

#include "common.hxx"

extern "C" class var {

  public:
    int bins;
    double min, max;
    std::string name, ltx, units; 

    var( std::string name ){
      this->name = name;
      this->bins = -99;
      this->min = 0;
      this->max = 0;
      this->units = "";
      this->ltx = "";
    };

    var( std::string name, int bins, double min, double max ){
      this->name = name;
      this->bins = bins;
      this->min = min;
      this->max = max;
      this->units = "";
      this->ltx = "";
    };

    var( std::string name, int bins, double min, double max, std::string units, std::string ltx ){
      this->name = name;
      this->bins = bins;
      this->min = min;
      this->max = max;
      this->units = units;
      this->ltx = ltx;
    }
    
    void set_name( std::string name );
    void set_bins( int bins );
    void set_min( double min );
    void set_max( double max );
    void set_units( std::string units);
    void set_ltx( std::string ltx );



    std::string get_name();
    int get_bins();
    double get_min();
    double get_max();
    std::string get_ltx();

    std::string get_units();

};


#endif
