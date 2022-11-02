

#ifndef bound_hxx
#define bound_hxx

#include "common.hxx"

// class to contain variables for defining variable bounds in jpsi+gamma analysis
// Could probably be replaced with a struct -> to come.
extern "C" class bound {

  public:
    int bins;
    double min, max;
    std::string name, var, ltx, units; 

    bound( std::string name ){
      this->name = name;
      this->var = var;
      this->bins = -99;
      this->min = 0;
      this->max = 0;
      this->units = "";
      this->ltx = "";
    };

    bound( std::string name, std::string var, int bins, double min, double max ){
      this->name = name;
      this->name = var;
      this->bins = bins;
      this->min = min;
      this->max = max;
      this->units = "";
      this->ltx = "";
    };

    bound( std::string name, std::string var, int bins, double min, double max, std::string units, std::string ltx ){
      this->name = name;
      this->var = var;
      this->bins = bins;
      this->min = min;
      this->max = max;
      this->units = units;
      this->ltx = ltx;
    }

    
    // setters
    void set_name( std::string name );
    void set_var( std::string var );
    void set_bins( int bins );
    void set_min( double min );
    void set_max( double max );
    void set_units( std::string units);
    void set_ltx( std::string ltx );

    // getters
    std::string get_name();
    std::string get_var();
    int get_bins();
    double get_min();
    double get_max();
    std::string get_ltx();
    std::string get_units();

};


#endif
