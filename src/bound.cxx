#include <bound.hxx>

// setters
void bound::set_name( std::string name ){ this->name = name; }
void bound::set_var( std::string var ){ this->var = var; }
void bound::set_bins( int bins ){ this->bins = bins; }
void bound::set_min( double min ){ this->min = min; }
void bound::set_max( double max ){ this->max = max; }
void bound::set_ltx( std::string ltx ){ this->ltx = ltx; }
void bound::set_units( std::string units){ this->units = units; }

// getters
std::string bound::get_name(){ return std::string( this->name ); } 
std::string bound::get_var(){ return std::string( this->var ); } 
int bound::get_bins(){ return int( this->bins ); }
double bound::get_min(){ return double( this->min ); }
double bound::get_max(){ return double( this->max); }
std::string bound::get_ltx(){ return std::string( this->ltx ); }
std::string bound::get_units(){ return std::string( this->units ); }

double bound::get_width(){
  return ( this->max - this->min );
}

double bound::get_bin_width( int bins ){
  return this->get_width()/( (double) bins );
}

double bound::get_bin_width(){
  return this->get_bin_width( this->bins );
}


// getter to produce the cut this bound would produce
std::string bound::get_cut(){
  double temp_min = this->min;
  double temp_max = this->max;
  std::string temp_name = this->get_var();
  return Form( "(%s>%.5f)&&(%s<%.5f)",
                temp_name.c_str(), temp_min,
                temp_name.c_str(), temp_max );
}

// getter to produce a series of cuts if this bound is subdivided by a number of bins
std::vector< std::string > bound::get_cut_series( int bins ){
  if ( bins == 0 ){ bins = this->bins; }
  std::vector< std::string > cut_series;
  double width = this->get_bin_width( bins );
  std::string var = this->get_var();
  for ( int cut_idx = 0; cut_idx < bins; cut_idx++ ){
    double lower = this->min + width*cut_idx;
    double upper = lower + width;
    cut_series.push_back( Form( "(%s>%.5f)&&(%s<%.5f)", 
                                var.c_str(), lower, 
                                var.c_str(), upper ) );
  }
  return cut_series;
}

std::vector< std::string > bound::get_series_names( int bins ){
  
  if ( bins == 0 ){ bins = this->get_bins(); }
  std::vector< std::string > series_names;
  for ( int cut_idx = 0; cut_idx < bins; cut_idx++ ){
    series_names.push_back( Form( "%s%i", name.c_str(), cut_idx ) ); 
  }
  return series_names;
}


TH1F * bound::get_hist(){
  int rndnum = std::rand() % 10000 + 1;
  return new TH1F( std::to_string(rndnum).c_str(), "", this->bins, this->min, this->max );
}

std::string bound::get_x_str( int bins ){
  return Form( "%s/%.3f [%s]", this->ltx.c_str(), 
              this->get_bin_width( bins ),
              this->units.c_str() );
}

std::string bound::get_y_str( int bins ){
  return Form("/%.3f [%s]", 
              this->get_bin_width( bins ),
              this->units.c_str() );
}

std::string bound::get_x_str(){
  return this->get_x_str( this->bins );
}

std::string bound::get_y_str(){
  return this->get_y_str( this->bins );
}

