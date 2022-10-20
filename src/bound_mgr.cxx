#include <bound_mgr.hxx>

bound bound_mgr::get_bound( std::string name ){
  return bound(bounds.find( name )->second);
}

int bound_mgr::get_bins( std::string name ){
  return bounds.find( name )->second.get_bins();
}

double bound_mgr::get_min( std::string name ){
  return bounds.find( name )->second.get_min();
}

double bound_mgr::get_max( std::string name ){
  return bounds.find( name )->second.get_max();
}

std::string bound_mgr::get_units( std::string name ){
  return bounds.find( name )->second.get_units();
}

std::string bound_mgr::get_ltx( std::string name ){
  return bounds.find( name )->second.get_ltx();
}

std::string bound_mgr::get_cut( std::string name ){
  double temp_min = this->get_min( name );
  double temp_max = this->get_max( name );
  const char * temp_name = name.c_str();
  return std::string( Form( "%s>%.5f&&%s<%.5f", temp_name, temp_min,
                            temp_name, temp_max) );
}

void bound_mgr::set_name( std::string name, std::string new_name ){
  auto bound = bounds.extract( name );
  bound.key() = new_name;
  bound.mapped().set_name( new_name );
  bounds.insert( std::move( bound ) );
}

void bound_mgr::set_var( std::string name, std::string var ){
  bounds.find( name )->second.set_var( var );
}

void bound_mgr::set_bins( std::string name, int bins ){
  bounds.find( name )->second.set_bins( bins );
}

void bound_mgr::set_min( std::string name, double min ){
  bounds.find( name )->second.set_min( min );
}

void bound_mgr::set_max( std::string name, double max ){
  bounds.find( name )->second.set_max( max );
}

void bound_mgr::set_units( std::string name, std::string units ){
  bounds.find( name )->second.set_units( units );
}

void bound_mgr::set_ltx( std::string name, std::string ltx ){
  bounds.find( name )->second.set_ltx( ltx );
}

void bound_mgr::add_bound( std::string name ){
  bounds.insert( std::pair< std::string, bound>( name, bound( name ) ) );
}

void bound_mgr::add_bound( std::string name, std::string var, int bins, double min, double max  ){
  bound temp_bound( name, var, bins, min, max );
  bounds.insert( std::pair< std::string, bound>( name, temp_bound ) );
}

void bound_mgr::add_bound( std::string name, std::string var, int bins, double min, double max, std::string units, std::string ltx ){
  bound temp_bound( name, var, bins, min, max );
  temp_bound.set_units( units );
  temp_bound.set_ltx( ltx );
  bounds.insert( std::pair< std::string, bound>( name, temp_bound ) );
}

void bound_mgr::set_bound( std::string name, std::string var, int bins, double min, double max ){
  this->set_bound( name, var, bins, min, max, this->get_units( name ), this->get_ltx( name ) );
}

void bound_mgr::set_bound( std::string name, std::string var, int bins, double min, double max, std::string units, std::string ltx ){
  this->set_var( name, var );
  this->set_bins( name, bins );
  this->set_min( name, min );
  this->set_max( name, max );
  this->set_units( name, units );
  this->set_ltx( name, ltx );
}

void bound_mgr::load_bound_mgr(){
	this->load_bound_mgr( this->bound_mgr_filename );
}

void bound_mgr::load_bound_mgr( std::string bound_mgr_filename ){
  std::ifstream bound_mgr_file( bound_mgr_filename );
	std::string bound_string;
	while ( std::getline( bound_mgr_file, bound_string ) ){
		std::vector< std::string > bound_vec;
		split_strings( bound_vec, bound_string, "," );
    int temp_bins = std::stoi( bound_vec.at( 2 ) );
    int temp_min = std::stof( bound_vec.at( 3 ) );
    int temp_max = std::stof( bound_vec.at( 4 ) );
		bound temp_bound( bound_vec.at(0), bound_vec.at(1), temp_bins, temp_min, temp_max, bound_vec.at(4), bound_vec.at(5) );
    this->bounds.insert( std::pair<std::string, bound>(bound_vec.at(0), temp_bound ) );
	}	
  bound_mgr_file.close();
}
