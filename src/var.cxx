#include <var.hxx>

void var::set_bins( int bins ){ this->bins = bins; }
void var::set_min( double min ){ this->min = min; }
void var::set_max( double max ){ this->max = max; }
void var::set_ltx( std::string ltx ){ this->ltx = ltx; }
void var::set_name( std::string name ){ this->name = name; }
void var::set_units( std::string units){ this->units = units; }

int var::get_bins(){ return  int ( this->bins ); }
double var::get_min(){ return double( this->min ); }
double var::get_max(){ return double( this->max); }
std::string var::get_ltx(){ return std::string( this->ltx ); }
std::string var::get_name(){ return std::string( this->name); } 
std::string var::get_units(){ return std::string( this->units ); }
