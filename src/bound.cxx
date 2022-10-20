#include <bound.hxx>

void bound::set_name( std::string name ){ this->name = name; }
void bound::set_var( std::string var ){ this->var = var; }
void bound::set_bins( int bins ){ this->bins = bins; }
void bound::set_min( double min ){ this->min = min; }
void bound::set_max( double max ){ this->max = max; }
void bound::set_ltx( std::string ltx ){ this->ltx = ltx; }
void bound::set_units( std::string units){ this->units = units; }

std::string bound::get_name(){ return std::string( this->name ); } 
std::string bound::get_var(){ return std::string( this->var ); } 
int bound::get_bins(){ return int( this->bins ); }
double bound::get_min(){ return double( this->min ); }
double bound::get_max(){ return double( this->max); }
std::string bound::get_ltx(){ return std::string( this->ltx ); }
std::string bound::get_units(){ return std::string( this->units ); }
