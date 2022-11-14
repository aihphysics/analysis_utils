#include <bound_mgr.hxx>

// getters

// start of basic getters
bound bound_mgr::get_bound( std::string name ){
  if ( bounds->find( name ) != bounds->end() ){
    return bounds->find( name )->second;
  } else {
    std::cout << "non-existent bound requested" << std::endl;
    return bound( "null" );
  }
}

// add a blank bound
void bound_mgr::add_bound( std::string name ){
  bounds->insert( std::pair< std::string, bound>( name, bound( name ) ) );
}

// add a bound with the basics
void bound_mgr::add_bound( std::string name, std::string var, int bins, double min, double max  ){
  bound temp_bound( name, var, bins, min, max );
  bounds->insert( std::pair< std::string, bound>( name, temp_bound ) );
}

// adds a bound with the decorations
void bound_mgr::add_bound( std::string name, std::string var, int bins, double min, double max, std::string units, std::string ltx ){
  bound temp_bound( name, var, bins, min, max );
  temp_bound.set_units( units );
  temp_bound.set_ltx( ltx );
  bounds->insert( std::pair< std::string, bound>( name, temp_bound ) );
}

// load the bound manager
void bound_mgr::load_bound_mgr(){
	this->load_bound_mgr( this->bound_mgr_filename );
}

// load bounds into the manager
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
  std::cout << "loaded " << this->bounds->size() << " different bounds" << std::endl;
}

// process additional bounds in string form
// bounds manually supplied in string form overwrite the bounds already stored
// string supplied bounds do not contain ltx, or units.
void bound_mgr::process_bounds_string( std::string bounds_string ){

  if ( bounds_string.empty() ){ return; }

//  std::vector< std::string > single_bounds_vec;
//  split_strings( single_bounds_vec, bounds_string, "#" );
//
//
//  for ( std::string bound_str : single_bounds_vec ){
//
//    std::vector< std::string > bound_vec;
//    split_strings( bound_vec, bound_str, ":" );
//    //std::string_view name = bound_vec.at( 0 );
//    std::string name = bound_vec.at(0); 
//    int bins = ( !bound_vec.at(1).empty() ) ? : ; 
//    double min = ( !bound_vec.at(2).empty() ) ? : ; 
//    double max = ( !bound_vec.at(3).empty() ) ? : ; 
//
//
//  }
//
  

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




std::string bound_mgr::merge_cut( std::string & first, std::string & second ){

  return std::string( (this->bounds->find(first))->second.get_cut() + "&&" + (this->bounds->find(second))->second.get_cut() );

}
