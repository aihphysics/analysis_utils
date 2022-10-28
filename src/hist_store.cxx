#include <hist_store.hxx>

void hist_store::set_output_file( std::string file_path ){

  if ( this->output_file != nullptr ){
    output_file = new TFile( file_path.c_str(), "UPDATE" );
  } else {
    std::cout << "Store "  << output_file->GetName() << " is already initialised." << std::endl;
    std::cout << "No action taken." << std::endl;
  }
}

void hist_store::write( std::string file_path ){
  this->set_output_file( file_path );
  this->write();
}


void hist_store::write(){

  output_file->cd();
  for ( int hist_idx = 0; hist_idx < (int) hists->size(); hist_idx++ ){
    hists->at( hist_idx )->Write( hist_names->at( hist_idx ).c_str() );
  }
  for ( int canv_idx = 0; canv_idx < (int) canvases->size(); canv_idx++ ){
    canvases->at( canv_idx )->Write( canvas_names->at( canv_idx ).c_str() );
  }
}

void hist_store::add_hist( TH1 * hist, std::string name ){
  hists->push_back( hist );
  hist_names->push_back( name );
}

void hist_store::add_canvas( TCanvas * canvas, std::string name ){
  canvases->push_back( canvas );
  canvas_names->push_back( name );
}

void hist_store::add_hist( TH1 * hist ){
  this->add_hist( hist, hist->GetName() ); 
}
  
void hist_store::add_canvas( TCanvas * canvas ){
  this->add_canvas( canvas, canvas->GetName() ); 
}
