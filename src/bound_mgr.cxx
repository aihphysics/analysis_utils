#include <bound_mgr.hxx>

bound bound_mgr::get_bound( std::string name ){
  if ( bounds->find( name ) != bounds->end() ){
    return bounds->find( name )->second;
  } else {
    std::cout << "non-existent bound requested" << std::endl;
    return bound( "null" );
  }
}



std::string bound_mgr::get_var( std::string name ){
  return bounds->find( name )->second.get_var();
}

int bound_mgr::get_bins( std::string name ){
  return bounds->find( name )->second.get_bins();
}

double bound_mgr::get_min( std::string name ){
  return bounds->find( name )->second.get_min();
}

double bound_mgr::get_max( std::string name ){
  return bounds->find( name )->second.get_max();
}

std::string bound_mgr::get_units( std::string name ){
  return bounds->find( name )->second.get_units();
}

std::string bound_mgr::get_ltx( std::string name ){
  return bounds->find( name )->second.get_ltx();
}

double bound_mgr::get_width( std::string name ){
  double max  = ( bounds->find( name )->second ).get_max();
  double min  = ( bounds->find( name )->second ).get_min();
  return ( max - min );
}

double bound_mgr::get_bin_width( std::string name ){
  return ( this->get_width( name ) )/( (double) this->get_bins( name ) );
}


std::string bound_mgr::get_cut( std::string name ){
  double temp_min = this->get_min( name );
  double temp_max = this->get_max( name );
  std::string temp_name = this->get_var( name );
  return std::string( Form( "(%s>%.3f)&&(%s<%.5f)", temp_name.c_str(), temp_min,
                            temp_name.c_str(), temp_max ) );
}

std::vector< std::string > bound_mgr::get_cut_series( std::string name, int bins ){
  if ( bins == 0 ){
    bins = this->get_bins( name );
  }
  std::vector< std::string > cut_series;
  double min = this->get_min( name );
  double max = this->get_max( name );
  double width = ( max - min )/( (double) bins );
  std::string var = this->get_var( name );
  for ( int cut_idx = 0; cut_idx < bins; cut_idx++ ){
    double lower = min + width*cut_idx;
    double upper = lower + width;
    cut_series.push_back( Form( "%s>%.5f&&%s<%.5f", var.c_str(), lower, 
                                var.c_str(), upper ) );
  }
  return cut_series;
}

void bound_mgr::set_name( std::string name, std::string new_name ){
  auto bound = bounds->extract( name );
  bound.key() = new_name;
  bound.mapped().set_name( new_name );
  bounds->insert( std::move( bound ) );
}

void bound_mgr::set_var( std::string name, std::string var ){
  bounds->find( name )->second.set_var( var );
}

void bound_mgr::set_bins( std::string name, int bins ){
  bounds->find( name )->second.set_bins( bins );
}

void bound_mgr::set_min( std::string name, double min ){
  bounds->find( name )->second.set_min( min );
}

void bound_mgr::set_max( std::string name, double max ){
  bounds->find( name )->second.set_max( max );
}

void bound_mgr::set_units( std::string name, std::string units ){
  bounds->find( name )->second.set_units( units );
}

void bound_mgr::set_ltx( std::string name, std::string ltx ){
  bounds->find( name )->second.set_ltx( ltx );
}

void bound_mgr::add_bound( std::string name ){
  bounds->insert( std::pair< std::string, bound>( name, bound( name ) ) );
}

void bound_mgr::add_bound( std::string name, std::string var, int bins, double min, double max  ){
  bound temp_bound( name, var, bins, min, max );
  bounds->insert( std::pair< std::string, bound>( name, temp_bound ) );
}

void bound_mgr::add_bound( std::string name, std::string var, int bins, double min, double max, std::string units, std::string ltx ){
  bound temp_bound( name, var, bins, min, max );
  temp_bound.set_units( units );
  temp_bound.set_ltx( ltx );
  bounds->insert( std::pair< std::string, bound>( name, temp_bound ) );
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
    if ( bound_string.empty() ){ continue; }
		std::vector< std::string > bound_vec;
		split_strings( bound_vec, bound_string, "," );  
    int temp_bins = ( bound_vec.at( 2 ).empty() ) ? 100 : std::stoi( bound_vec.at( 2 ) );
    float temp_min = ( bound_vec.at( 3 ).empty() ) ? -FLT_MAX : std::stof( bound_vec.at( 3 ) );
    float temp_max = ( bound_vec.at( 4 ).empty() ) ? FLT_MAX : std::stof( bound_vec.at( 4 ) );
		bound temp_bound( bound_vec.at(0), bound_vec.at(1), temp_bins, temp_min, temp_max, 
                      bound_vec.at(5), bound_vec.at(6) );
    this->bounds->insert( std::pair<std::string, bound>(bound_vec.at(0), temp_bound ) );
	}	
  bound_mgr_file.close();
}

void bound_mgr::process_bounds_string( std::string bounds_string ){

  if ( bounds_string.empty() ){ return; }

  std::vector< std::string > single_bounds_vec;
  split_strings( single_bounds_vec, bounds_string, "#" );

  for ( std::string bound_str : single_bounds_vec ){

    std::vector< std::string > bound_vec;
    split_strings( bound_vec, bound_str, ":" );
    if ( bound_vec.size() == 4 ){
      if ( bounds->find( bound_vec.at(0) ) != bounds->end() ){
        std::cout << "replacing bound " << bound_vec.at(0) << std::endl;
        bounds->erase( bound_vec.at(0) );
      }
      int bound_bins = std::stoi( bound_vec.at(1) );
      float bound_min = std::stof( bound_vec.at(2) );
      float bound_max = std::stof( bound_vec.at(3) );
      this->add_bound( bound_vec.at( 0 ), bound_vec.at( 0 ), bound_bins, bound_min, bound_max );
    } else if ( bound_vec.size() == 6 ){
      if ( bounds->find( bound_vec.at(0) ) != bounds->end() ){
        std::cout << "replacing bound " << bound_vec.at(0) << std::endl;
        bounds->erase( bound_vec.at(0) );
      }
      int bound_bins = std::stoi( bound_vec.at(1) );
      float bound_min = std::stof( bound_vec.at(2) );
      float bound_max = std::stof( bound_vec.at(3) );
      this->add_bound( bound_vec.at( 0 ), bound_vec.at( 0 ), bound_bins, bound_min, bound_max,
                       bound_vec.at( 4 ), bound_vec.at( 5 ) );
    } else if ( bound_vec.size() == 6 ){
      if ( bounds->find( bound_vec.at(0) ) != bounds->end() ){
        std::cout << "replacing bound " << bound_vec.at(0) << std::endl;
        bounds->erase( bound_vec.at(0) );
        int bound_bins = std::stoi( bound_vec.at(2) );
        float bound_min = std::stof( bound_vec.at(3) );
        float bound_max = std::stof( bound_vec.at(4) );
        this->add_bound( bound_vec.at( 0 ), bound_vec.at( 1 ), bound_bins, bound_min, bound_max,
                       bound_vec.at( 5 ), bound_vec.at( 6 ) );
      }
    } else {
      for ( std::string str : bound_vec ){ std::cout << str << " ";}
      std::cout << std::endl;
      throw "ill formated bound, 4 or 6 elements";
    }

  }
  return;
}













