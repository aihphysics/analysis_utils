
#ifndef bounds_hxx
#define bounds_hxx

#include "common.hxx"
#include "utils.hxx"
#include "var.hxx"

extern "C" class bounds final{

  private:
    std::string bounds_filename;
    std::map< std::string, var  > vars;

  public:
    void load_bounds( );
    void load_bounds( std::string bounds_file );
    void set_bounds( std::string bounds_file );

    std::string get_cut( std::string bound );

    int get_bins( std::string name );
    double get_min( std::string name );
    double get_max( std::string name );
    std::string get_units( std::string name );
    std::string get_ltx( std::string name );

    void set_bins( std::string name, int bins );
    void set_min( std::string name, double min );
    void set_max( std::string name, double max );
    void set_units( std::string name, std::string units );
    void set_ltx( std::string ltx, std::string new_ltx );
    void set_name( std::string name, std::string new_name );
    
    void add_variable( std::string name );
    void add_variable( std::string name, int bins, double min, double max );
    void add_variable( std::string name, int bins, double min, double max, std::string units, std::string ltx );

    void set_variable( std::string name, int bins, double min, double max );
    void set_variable( std::string name, int bins, double min, double max, std::string units, std::string ltx );

    bounds();
    bounds( std::string bounds_file );

};


#endif
