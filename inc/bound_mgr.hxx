
#ifndef bound_mgr_hxx
#define bound_mgr_hxx

#include "common.hxx"
#include "utils.hxx"
#include "bound.hxx"

extern "C" class bound_mgr final{

  private:

    // members, config location and map, should probably be unordered set. 
    std::string bound_mgr_filename;
    std::unordered_map< std::string, bound > * bounds;

  public:
    
    // constructors
    bound_mgr(){
      bounds = new std::unordered_map< std::string, bound >();
    };
    bound_mgr( std::string bound_mgr_filename ){
      bounds = new std::unordered_map< std::string, bound>();
      this->bound_mgr_filename = bound_mgr_filename;
    };

    // prepare the manager
    void load_bound_mgr( );
    void load_bound_mgr( std::string bound_mgr_file );
    void set_bound_path( std::string bound_mgr_file );

    // getters
    bound get_bound( std::string name );
    void add_bound( std::string name );
    void add_bound( std::string name, std::string var, int bins, double min, double max );
    void add_bound( std::string name, std::string var, int bins, double min, double max, std::string units, std::string ltx );
    void process_bounds_string( std::string bounds_string );

};


#endif
