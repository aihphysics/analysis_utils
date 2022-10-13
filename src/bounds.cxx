#include <bounds.hxx>


int bounds::get_bins( std::string name ){
  return vars.find( name )->second.get_bins();
}

double bounds::get_min( std::string name ){
  return vars.find( name )->second.get_min();
}

double bounds::get_max( std::string name ){
  return vars.find( name )->second.get_max();
}

std::string bounds::get_units( std::string name ){
  return vars.find( name )->second.get_units();
}

std::string bounds::get_ltx( std::string name ){
  return vars.find( name )->second.get_ltx();
}

std::string bounds::get_cut( std::string name ){
  double temp_min = this->get_min( name );
  double temp_max = this->get_max( name );
  const char * temp_name = name.c_str();
  return std::string( Form( "%s>%.5f&&%s<%.5f", temp_name, temp_min,
                            temp_name, temp_max) );
}

void bounds::set_name( std::string name, std::string new_name ){
  auto bound = vars.extract( name );
  bound.key() = new_name;
  bound.mapped().set_name( new_name );
  vars.insert( std::move( bound ) );
}

void bounds::set_bins( std::string name, int bins ){
  vars.find( name )->second.set_bins( bins );
}

void bounds::set_min( std::string name, double min ){
  vars.find( name )->second.set_min( min );
}

void bounds::set_max( std::string name, double max ){
  vars.find( name )->second.set_max( max );
}

void bounds::set_units( std::string name, std::string units ){
  vars.find( name )->second.set_units( units );
}

void bounds::set_ltx( std::string name, std::string ltx ){
  vars.find( name )->second.set_ltx( ltx );
}

void bounds::add_variable( std::string name ){
  vars.insert( std::pair< std::string, var>( name, var( name ) ) );
}

void bounds::add_variable( std::string name, int bins, double min, double max  ){
  var temp_var( name, bins, min, max );
  vars.insert( std::pair< std::string, var>( name, temp_var ) );
}

void bounds::add_variable( std::string name, int bins, double min, double max, std::string units, std::string ltx ){
  var temp_var( name, bins, min, max );
  temp_var.set_units( units );
  temp_var.set_ltx( ltx );
  vars.insert( std::pair< std::string, var>( name, temp_var ) );
}

void bounds::set_variable( std::string name, int bins, double min, double max ){
  this->set_variable( name, bins, min, max, this->get_units( name ), this->get_ltx( name ) );
}

void bounds::set_variable( std::string name, int bins, double min, double max, std::string units, std::string ltx ){
  this->set_bins( name, bins );
  this->set_min( name, min );
  this->set_max( name, max );
  this->set_units( name, units );
  this->set_ltx( name, ltx );
}

void bounds::load_bounds(){
	this->load_bounds( this->bounds_filename );
}

void bounds::load_bounds( std::string bounds_filename ){
  std::ifstream bounds_file( bounds_filename );
	std::string bound_string;
	while ( std::getline( bounds_file, bound_string ) ){
		std::vector< std::string > bound_vec;
		split_strings( bound_vec, bound_string, "," );
    int temp_bins = std::stoi( bound_vec.at( 1 ) );
    int temp_min = std::stof( bound_vec.at( 2 ) );
    int temp_max = std::stof( bound_vec.at( 3 ) );
		var temp_var( bound_vec.at(0), temp_bins, temp_min, temp_max, bound_vec.at(4), bound_vec.at(5) );
    this->vars.insert( std::pair<std::string, var>(bound_vec.at(0), temp_var) );
	}	
  bounds_file.close();
}
