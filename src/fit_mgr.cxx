#include <fit_mgr.hxx>

// load fit manager, parse config file
void fit_mgr::load_fit_mgr(){
	this->load_fit_mgr( this->fit_mgr_filename );
}

// load fit manager, parse config file
void fit_mgr::load_fit_mgr( std::string fit_mgr_filename ){

  // iterate over defined file
  std::ifstream fit_mgr_file( fit_mgr_filename );
	std::string fit_string;
	while ( std::getline( fit_mgr_file, fit_string ) ){
    if ( fit_string.empty() ){ continue; }

    // parse line, 10 , separated entries
		std::vector< std::string > fit_vec, param_starts, param_lowers, param_uppers;
		split_strings( fit_vec, fit_string, "," );  
		split_strings( param_starts, fit_vec.at(7), "#" );  
		split_strings( param_lowers, fit_vec.at(8), "#" );  
		split_strings( param_uppers, fit_vec.at(9), "#" );  

    // define key
    std::string key = fit_vec.at(0) + "_" + fit_vec.at(1) + "_" + fit_vec.at(2)  + "_" + fit_vec.at(3);
    //if (std::adjacent_find( { param_starts.size(), param_lowers.size(), param_uppers.size() }, std::not_equal_to<>() ){
    //  std::cout << "param count mismatch" << std::endl;
    //  throw "ill formatted config"
    //}
  
    // parse limits
    std::vector< std::pair< double, double> > param_limits;
    for ( int par_idx = 0; par_idx < std::stoi( fit_vec.at(4) ); par_idx++ ){
      double lower = ( param_lowers.at( par_idx ).empty() ) ? -DBL_MAX : std::stod( param_lowers.at( par_idx ) );
      double upper = ( param_uppers.at( par_idx ).empty() ) ?  DBL_MAX : std::stod( param_uppers.at( par_idx ) );
      param_limits.push_back( std::pair< double, double >( lower, upper ) );
    }
    
    // parse parameter intial conditions;
    std::vector< double > param_num_starts ;
    for ( int par_idx = 0; par_idx < std::stoi( fit_vec.at(4) ); par_idx++ ){

      if ( !( param_starts.at( par_idx) ).empty() ){ 
        param_num_starts.push_back( std::stod( param_starts.at( par_idx ) ) );
      } else {
        std::pair< double, double > limits = param_limits.at( par_idx );
        param_num_starts.push_back( ( limits.first + limits.second )/2.0 );
      }

    } 

    // construct the parameter struct
    fit_param current;
    current.param_count = std::stoi( fit_vec.at(4) );
    current.func_start = std::stod( fit_vec.at(5) );
    current.func_end = std::stod( fit_vec.at(6) );
    current.param_starts = param_num_starts;
    current.param_limits = param_limits;

    // insert into the map
    (this->fits)->insert( std::pair<std::string, fit_param>( key, current ) );

	}	

  //close out
  fit_mgr_file.close();
}


// align function with parameters attached to key
void fit_mgr::align_func( TF1 * func, const std::string & key ){

  fit_param params = fits->at( key );
  for ( int par_idx = 0; par_idx < params.param_count; par_idx++ ){
    func->SetParameter( par_idx, ( params.param_starts ).at( par_idx ) );
  }

}

// 
void fit_mgr::range_func( TF1 * func, const std::string & key ){

  fit_param params = fits->at( key );
  func->SetRange( params.func_start, params.func_end );

}

// apply limits to parameters
void fit_mgr::limit_func( TF1 * func, const std::string & key ){

  fit_param params = fits->at( key );
  for ( int par_idx = 0; par_idx < params.param_count; par_idx++ ){
    func->SetParLimits( par_idx, (params.param_limits).at( par_idx ).first, 
                                 (params.param_limits).at( par_idx ).second );
  }

}

void fit_mgr::parameterise_func( TF1 * func, const std::string & key ){
  this->range_func( func, key );
  this->align_func( func, key );
  this->limit_func( func, key );
}

double fit_mgr::get_lower_range( const std::string & key ){
  return ( fits->at( key ) ).func_start;
}

double fit_mgr::get_upper_range( const std::string & key ){
  return ( fits->at( key ) ).func_end;
}

