#ifndef fit_mgr_hxx
#define fit_mgr_hxx

#include "common.hxx"
#include "utils.hxx"

struct fit_param {
  int param_count;
  double func_start;
  double func_end;
  std::vector< double > param_starts;
  std::vector< std::pair<double, double> > param_limits;
};

extern "C" class fit_mgr final{

  private:

    // members, config location and map; 
    std::string fit_mgr_filename;
    std::unordered_map< std::string, fit_param > * fits;

  public:
    
    // constructors
    fit_mgr(){
      fits = new std::unordered_map< std::string, fit_param >();
    };

    fit_mgr( std::string fit_mgr_filename ){
      fits = new std::unordered_map< std::string, fit_param >();
      this->fit_mgr_filename = fit_mgr_filename;
    };

    // prepare the manager
    void load_fit_mgr( );
    void load_fit_mgr( std::string fit_mgr_file );
    void set_fit_path( std::string fit_mgr_file );

    // function modifiers;
    void align_func( TF1 * func, const std::string & key );
    void range_func( TF1 * func, const std::string & key );
    void limit_func( TF1 * func, const std::string & key );
    void parameterise_func( TF1 * func, const std::string & key );

    // get upper and lower ranges
    double get_lower_range( const std::string & key );
    double get_upper_range( const std::string & key );

};

#endif
