
#ifndef hist_store_hxx
#define hist_store_hxx

#include "common.hxx"
#include "utils.hxx"

extern "C" class hist_store{

  private:
    TFile * output_file;
    std::vector< TH1 * > * hists;
    std::vector< std::string > * hist_names;
    std::vector< TCanvas * > * canvases;
    std::vector< std::string > * canvas_names;
  
  public:
    hist_store(){
      this->hists = new std::vector< TH1 * >();
      this->hist_names = new std::vector< std::string >();
      output_file = nullptr;
    }
  
    hist_store( std::string file_path ) : hist_store() {
      hist_store();
      output_file = new TFile( file_path.c_str(), "RECREATE" );
    }
  
    void set_output_file( std::string file_path );
    void add_hist( TH1 * hist, std::string name );
    void add_canvas( TCanvas * canvas, std::string name );
    void add_hist( TH1 * hist );
    void add_canvas( TCanvas * canvas );


    void write();
    void write( std::string file_path );
  

};


#endif
