#include <utils.hxx>

// things you can do
// get error bars from a hist and turn them into their own hist, relative or absolute
// get an error hist, turn those into error bars;
// calculate the absolute or relative error of a systematic ( one-sided )
// combine systematic groups

void split_strings(std::vector<std::string> & vec_split, std::string str_split, std::string delim){

  std::istringstream str_stream{str_split};
  std::string token;
  while( getline( str_stream, token, *(delim.c_str()) ) ){
    vec_split.push_back(token);
  }

}

void align_sg( TF1 * sg_func, TH1F * hist, bool limit ){
  sg_func->SetParameter( 0, hist->GetMaximum() );
  sg_func->SetParameter( 1, hist->GetMean() );
  sg_func->SetParameter( 2, hist->GetStdDev() );
  if ( limit ){
    sg_func->SetParLimits( 2, hist->GetStdDev()/10.0, hist->GetStdDev()*2.0 );
  }

}

void align_dg( TF1 * dg_func, TH1F * hist, bool limit){
  dg_func->SetParameter( 0, hist->GetMaximum()*(2./3.) );
  dg_func->SetParameter( 1, hist->GetMean() );
  dg_func->SetParameter( 2, hist->GetStdDev() );
  dg_func->SetParameter( 3, hist->GetMaximum()*(1./3.) );
  dg_func->SetParameter( 4, hist->GetStdDev()*3 );
  if ( limit ){
    dg_func->SetParLimits( 2, hist->GetStdDev()/3.0, hist->GetStdDev() );
    dg_func->SetParLimits( 4, hist->GetStdDev(), hist->GetStdDev()*4.0 );
  }
}

void hist_prep_axes( TH1 * hist, bool zero ){
  //hist->GetXaxis()->SetRange( hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax() );
  hist->GetYaxis()->SetRangeUser( !zero * hist->GetMinimum() * 1.75, hist->GetMaximum() * 1.75 );
  hist->GetYaxis()->SetLabelSize( 0.035 );
  hist->GetYaxis()->SetTitleSize( 0.035 );
  hist->GetXaxis()->SetLabelSize( 0.035 );
  hist->GetXaxis()->SetTitleSize( 0.035 );
  hist->GetYaxis()->SetMaxDigits( 3 );
  //hist->GetXaxis()->SetRange( 1, hist->GetNbinsX() );
}

void error_prep_axes( TGraphAsymmErrors * err ){

  double max = std::max( { err->GetMaximum(), abs( err->GetMinimum() ) } );
  max = std::ceil( max );
  err->GetYaxis()->SetRangeUser( max, max );
  err->GetYaxis()->SetLabelSize( 0.035 );
  err->GetYaxis()->SetTitleSize( 0.035 );
  err->GetXaxis()->SetLabelSize( 0.035 );
  err->GetXaxis()->SetTitleSize( 0.035 );
  err->GetYaxis()->SetMaxDigits( 3 );
  //hist->GetXaxis()->SetRange( 1, hist->GetNbinsX() );
}

std::map< std::string, std::vector< double > > prep_binnings( std::string input_range ){

  std::map< std::string, std::vector<double> > bins;
  bins["qtA"]           =  std::vector<double>{ 15,  -10,  20};
  bins["BDT"]           =  std::vector<double>{ 10,  -1.0, 1.0};
  bins["abs(qtB)"]      =  std::vector<double>{ 10,  0,    20};
  bins["qtB"]           =  std::vector<double>{ 10,  0,    20};
  bins["Phi"]           =  std::vector<double>{ 8,   0,    M_PI};
  bins["DPhi"]          =  std::vector<double>{ 8,   0,    M_PI};
  bins["DiMuonPt"]      =  std::vector<double>{ 10,  5,    30};
  bins["PhotonPt"]      =  std::vector<double>{ 10,  5,    25};

  if ( !input_range.empty() ){
    std::vector< std::string > arg_bin;
    std::vector< std::string > set_bins;
    split_strings( arg_bin, input_range, "_" );
    split_strings( set_bins, arg_bin.at(1), "," );
    std::map< std::string, std::vector<double> >::iterator bin_map_itr = bins.find( arg_bin.at( 0 ).c_str() );
    bin_map_itr->second = { std::stod( set_bins.at( 0 ) ), std::stod( set_bins.at( 1 ) ), std::stod( set_bins.at( 2 ) ) };
  }

  return bins;

}


// turns errorbar on a histogram into histogram containing the error
TH1F * errorbar_to_hist( TH1F * hist, bool absolute ){
  TH1F * err_hist = (TH1F *) hist->Clone();
  err_hist->Reset();
  int bins = err_hist->GetNbinsX();
  for ( int bin_idx = 1; bin_idx <= bins; bin_idx++ ){
    double error = hist->GetBinError( bin_idx );
    if ( !absolute ){ error /= hist->GetBinContent( bin_idx ); }
    err_hist->SetBinContent( bin_idx, error ); 
  }
	return err_hist;
}

// turns a base histogram and a hisogram representing the error into
// a single histogram with an errorbar
TH1F * hist_to_errorbar( TH1F * base, TH1F * err_hist, bool absolute ){
  TH1F * errorbar_hist = (TH1F *) base->Clone();
  errorbar_hist->Reset();
  int bins = errorbar_hist->GetNbinsX();
  for ( int bin_idx = 1; bin_idx <= bins; bin_idx++ ){
    double error = err_hist->GetBinContent( bin_idx );
    if( !absolute ){ error *= base->GetBinContent( bin_idx ); }
    errorbar_hist->SetBinContent( bin_idx, base->GetBinContent( bin_idx ) );
    errorbar_hist->SetBinError( bin_idx, error );
  }
	return errorbar_hist;
}

// take a histogram and an error, convert this into a histogram with the systematic as an error bar;
TH1F * single_sys_to_error( TH1F * base, TH1F * sys ){
  TH1F * err_hist = (TH1F *) base->Clone();
  err_hist->Reset();
  int bins = err_hist->GetNbinsX();
  for ( int bin_idx = 1; bin_idx <= bins; bin_idx++ ){
    err_hist->SetBinContent( bin_idx, base->GetBinContent( bin_idx ) );
    err_hist->SetBinError( bin_idx, std::abs( sys->GetBinContent( bin_idx ) - base->GetBinContent( bin_idx ) ) );
  }
  return err_hist;
}

TH1F * sys_to_error_hist( TH1F * base, TH1F * sys, bool absolute ){
  TH1F * err_hist = (TH1F *) base->Clone();
  err_hist->Reset();
  int bins = err_hist->GetNbinsX();
  for ( int bin_idx = 1; bin_idx <= bins; bin_idx++ ){
    double error = sys->GetBinContent( bin_idx ) - base->GetBinContent( bin_idx );
    if ( !absolute ){ error /= base->GetBinContent( bin_idx );}
    err_hist->SetBinContent( bin_idx, error );
  }
  return err_hist;
}


void make_hf_fit_histogram( char bin_name[150], char err_name[150], TFile* source_file, TH1F * reco_hist, int err_bin ){
  int bins = reco_hist->GetNbinsX();
  bool base_error_available{true};
  for ( int bin_idx = 1; bin_idx <= bins; bin_idx++ ){
    TH1F * base_integral_hist   = (TH1F*) source_file->Get( Form( "%s-%i", bin_name, bin_idx ) );
    TH1F * base_error_hist      = (TH1F*) source_file->Get( Form( "%s-%i", err_name, bin_idx ) );
    reco_hist->SetBinContent( bin_idx, base_integral_hist->Integral());
    if ( base_error_hist != NULL ){ 
      reco_hist->SetBinError( bin_idx, base_error_hist->GetBinError( err_bin ) );
    } else { base_error_available = false; }
  }
  if ( !base_error_available ){ reco_hist->Sumw2(); }
}

void combine_sys_group( std::vector< TH1F *> & group_systematics, TH1F * group_hist, TH1F * base ){
  int bins = group_systematics.at(0)->GetNbinsX();
  std::vector< double > err_max, err_min, combined_sys_err;
  for ( int bin_idx = 1; bin_idx <= bins; bin_idx++ ){ 
    err_max.push_back( 0 ); err_min.push_back( DBL_MAX );
    combined_sys_err.push_back( 0 );
  }
  for ( TH1F *& systematic : group_systematics ){
    for ( int bin_idx = 1; bin_idx <= bins; bin_idx++ ){ 
      if ( systematic->GetBinContent( bin_idx ) < err_min[ bin_idx - 1 ] ){
        err_min[ bin_idx - 1 ]  = systematic->GetBinContent( bin_idx );
      }
      if ( systematic->GetBinContent( bin_idx ) > err_max[ bin_idx - 1 ] ){
        err_max[ bin_idx - 1 ]  = systematic->GetBinContent( bin_idx );
      }
    }
  }
  for ( int bin_idx = 1; bin_idx <= bins; bin_idx++ ){ 
    combined_sys_err[ bin_idx-1 ] = ( err_max[ bin_idx - 1] - err_min[ bin_idx - 1 ] ) / sqrt( 3.0f );
    group_hist->SetBinContent( bin_idx, base->GetBinContent( bin_idx ) );
    group_hist->SetBinError( bin_idx, combined_sys_err[ bin_idx - 1 ] );
  }
}

void add_pad_title( TPad * active_pad, std::string title, bool sci_axis ){
  active_pad->cd();
	TLatex pad_title;
	pad_title.SetTextSize( 0.035 );
	pad_title.DrawLatexNDC( (sci_axis) ? 0.23 : 0.17, 0.8175, title.c_str() );  
}

void set_axis_labels( TH1F * hist, std::string xaxis, std::string yaxis ){
  hist->GetXaxis()->SetTitle( xaxis.c_str() );
  hist->GetYaxis()->SetTitle( yaxis.c_str() );
}

void set_err_axis_labels( TGraphAsymmErrors * err, std::string xaxis, std::string yaxis ){
  err->GetXaxis()->SetTitle( xaxis.c_str() );
  err->GetYaxis()->SetTitle( yaxis.c_str() );
}

void set_2d_axis_labels( TH2F * hist, std::string xaxis, std::string yaxis ){
  hist->GetXaxis()->SetTitle( xaxis.c_str() );
  hist->GetYaxis()->SetTitle( yaxis.c_str() );
}

void add_atlas_decorations( TPad * active_pad, bool wip, bool sim ){
  active_pad->cd();
  TLatex * atlas_logo_ltx = new TLatex(); 
  atlas_logo_ltx->SetNDC(); 
  atlas_logo_ltx->SetTextSize(0.038);
  atlas_logo_ltx->SetTextFont(72);
  atlas_logo_ltx->SetTextColor(1);
  TLatex * wip_ltx = new TLatex(); 
  wip_ltx->SetNDC();
  wip_ltx->SetTextFont(42);
  wip_ltx->SetTextSize(0.038);
  wip_ltx->SetTextColor(1);
  TLatex * sim_ltx = new TLatex();
  sim_ltx->SetNDC();
  sim_ltx->SetTextFont(42);
  sim_ltx->SetTextSize(0.038);
  sim_ltx->SetTextColor(1);
  atlas_logo_ltx->DrawLatexNDC( 0.187, 0.75, "ATLAS" );
  if ( sim ){ sim_ltx->DrawLatexNDC( 0.350, 0.75, "Simulation" ); }
	if ( wip ){ wip_ltx->DrawLatexNDC( 0.187, 0.705, "Work In Progress" ); }
}

TLegend * create_atlas_legend(){
  TLegend * pad_legend = new TLegend( 0.6, 0.6, 0.88, 0.78 );
  pad_legend->SetBorderSize( 0 );
  pad_legend->SetFillColor( 0 );
  pad_legend->SetFillStyle( 0 );
  pad_legend->SetTextFont( 42 );
  pad_legend->SetTextSize( 0.025 );
  return pad_legend;
}

TLegend * below_logo_legend(){
  TLegend * pad_legend = new TLegend( 0.187, 0.6, 0.4, 0.68 );
  pad_legend->SetBorderSize( 0 );
  pad_legend->SetFillColor( 0 );
  pad_legend->SetFillStyle( 0 );
  pad_legend->SetTextFont( 42 );
  pad_legend->SetTextSize( 0.025 );
  return pad_legend;
}

void style_hist( TH1F * hist, std::vector< float > & style_vec ){ 
  hist->SetMarkerStyle( style_vec.at( 0 ) );
  hist->SetMarkerColorAlpha( style_vec.at( 1 ), style_vec.at( 2 ) );
  hist->SetMarkerSize( style_vec.at( 3 ) );
  hist->SetLineStyle( style_vec.at( 4 ) );
  hist->SetLineColorAlpha( style_vec.at( 5 ), style_vec.at( 6 ) );
  hist->SetLineWidth( style_vec.at( 7 ) );
  hist->SetFillColorAlpha( style_vec.at( 8 ), style_vec.at( 9 ) );
  hist->SetFillStyle( style_vec.at( 10 ) );
}


void style_func( TF1 * func, std::vector<float> & style_vec ){
  func->SetLineStyle( style_vec.at( 0 ) );
  func->SetLineColorAlpha( style_vec.at( 1 ), style_vec.at( 2 ) );
  func->SetLineWidth( style_vec.at( 3 ) );
}

TF1 * prep_sg( float min, float max ){
  int rndnum = std::rand() % 10000 + 1;
  TF1 * sg = new TF1( Form( "sg_%i", rndnum ),
                     "[0]*e^((-([1]-x)^(2))/(2*([2])^(2)))",
                     min, max );
  sg->SetParName( 0, "c"); 
  sg->SetParName( 1, "x" ); 
  sg->SetParName( 2, "#sigma" );
  return sg;
}

TF1 * prep_dg( float min, float max ){
  int rndnum = std::rand() % 10000 + 1;
  TF1 * dg = new TF1( Form( "dg_%i", rndnum ), 
                     "[0]*e^((-([1]-x)^(2))/(2*([2])^(2)))"
                     "+ [3]*e^((-([1]-x)^(2))/(2*([4])^(2)))", 
                     min, max );
  dg->SetParName( 0, "c_{1}"); 
  dg->SetParName( 1, "x" ); 
  dg->SetParName( 2, "#sigma_{1}" );
  dg->SetParName( 3, "c_{2}"); 
  dg->SetParName( 4, "#sigma_{2}" );
  return dg;
}

TF1 * prep_line( float min, float max ){
  int rndnum = std::rand() % 10000 + 1;
  TF1 * line = new TF1( Form( "line_%i", rndnum), "[0]*x + [1]", min, max );
  line->SetParameter( 0, 0 );
  line->SetParameter( 1, 1 );
  line->SetParName( 0, "m" );
  line->SetParName( 1, "c" );
  return line;
}

TH1F * quadrature_error_combination( TH1F * stat, std::vector<TH1F *> systematic, bool sys_only ){
  TH1F * combined_hist = (TH1F*) stat->Clone();
  combined_hist->Reset();
  int bins = combined_hist->GetNbinsX();
  for ( int bin_idx = 1; bin_idx <= bins; bin_idx++ ){ 
    double bin_error = 0;
    if ( !sys_only ){ bin_error += std::pow( stat->GetBinError( bin_idx ), 2.0 ); }
    for ( TH1F * sys : systematic ){
      bin_error += std::pow( sys->GetBinError( bin_idx ), 2.0 );
    }
    combined_hist->SetBinContent( bin_idx, stat->GetBinContent( bin_idx ) );
    combined_hist->SetBinError( bin_idx, std::sqrt( bin_error ) );
  }
  return combined_hist;
}

TPaveStats * make_stats( TH1 * hist, bool small, bool shifted ){
  if ( shifted ){ small = true; };
  double shift = 0.13 * shifted;
  double width = 0.24 / ( 1 + small );
  gPad->Update();
  TPaveStats * stats = (TPaveStats*) hist->FindObject("stats");
  stats->SetX1NDC( 0.56 + shift ); 
  stats->SetX2NDC( 0.56 + shift + width);
  stats->SetY1NDC( 0.55 ); 
  stats->SetY2NDC( 0.78 );
  stats->SetFillStyle( 0 ); 
  stats->SetTextFont( 42 );
  stats->SetTextSize( 0.02 * ( 2 / ( 2+small ) ) );
  stats->SetBorderSize( 1 );
  gPad->Modified();
  gPad->Update();
  return stats;
}

TLegend * create_stat_legend(){
  TLegend * pad_legend = new TLegend( 0.6, 0.6, 0.88, 0.78 );
  pad_legend->SetBorderSize( 0 );
  pad_legend->SetFillColor( 0 );
  pad_legend->SetFillStyle( 0 );
  pad_legend->SetTextFont( 42 );
  pad_legend->SetTextSize( 0.025 );
  return pad_legend;
}


//TGraphAsymmErrors * diff_to_err( TH1F * diff, bool absolute);

void compare_systematic( TH1F * base, TH1F * sys, const std::string & var, const std::string & unique ){
  
  // prep a bound manager
  bound_mgr * selections = new bound_mgr();
  selections->load_bound_mgr( "/home/atlas/dhagan/libs/configs/selection_bounds.txt" );

  // prep bound and variables
  bound var_bound = selections->get_bound( var );
  int bins = var_bound.get_bins();
  double min = var_bound.get_min();
  double max = var_bound.get_max();
  double width = var_bound.get_width();

  // ready lines and diffs
  TF1 * unit_line = new TF1( "unit_line", "1", min, max );
  TF1 * zero_line = new TF1( "zero_line", "1", min, max );
  TH1F * rel_diff = sys_to_error_hist( base, sys, false );
  TH1F * abs_diff = sys_to_error_hist( base, sys, true );

  // diff to grapherr start
  double * sys_x = new double[bins];
  double * sys_y = new double[bins];
  double * err_x_lower = new double[bins];
  double * err_x_upper = new double[bins];
  double * rel_err_y_lower = new double[bins];
  double * rel_err_y_upper = new double[bins];
  double * abs_err_y_lower = new double[bins];
  double * abs_err_y_upper = new double[bins];

  for ( int bin = 0; bin < bins; bin++ ){

    sys_x[bin] = ( min + ( width/2.0 ) ) + ( width * ( (double) bin ) );
    sys_y[bin] = 1.0;
    err_x_lower[bin] = 0;
    err_x_upper[bin] = 0;
    double rel_err = rel_diff->GetBinContent( bin + 1 );
    double abs_err = abs_diff->GetBinContent( bin + 1 );
    if ( rel_err > 0 ){
      rel_err_y_lower[bin] = 0.0;
      rel_err_y_upper[bin] = abs( rel_err );
      abs_err_y_lower[bin] = 0.0;
      abs_err_y_upper[bin] = abs( abs_err );
    } else {
      rel_err_y_lower[bin] = abs( rel_err );
      rel_err_y_upper[bin] = 0.0;
      abs_err_y_lower[bin] = abs( abs_err );
      abs_err_y_upper[bin] = 0.0;
    }
  }


  // create symm error graphs
  TGraphAsymmErrors * rel_err_graph = new TGraphAsymmErrors( bins, sys_x, sys_y, 
                                          err_x_lower, err_x_upper,
                                          rel_err_y_lower, rel_err_y_upper);
  rel_err_graph->SetFillStyle( 3004 );
  rel_err_graph->SetFillColorAlpha( 3, 0.9 );
  rel_err_graph->SetLineWidth( 0 );

  TGraphAsymmErrors * abs_err_graph = new TGraphAsymmErrors( bins, sys_x, sys_y, 
                                          err_x_lower, err_x_upper,
                                          abs_err_y_lower, abs_err_y_upper);
  abs_err_graph->SetFillStyle( 3004 );
  abs_err_graph->SetFillColorAlpha( 3, 0.9 );
  abs_err_graph->SetLineWidth( 0 );

  // style base and sys;
  std::vector< float > base_style = { 1, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0, 0, 0 };
  std::vector< float > sys_style =  { 1, 0.0, 0.0, 0.0, 0, kRed+1, 0.5, kRed + 1 };

  style_hist( base, base_style );
  style_hist( sys, base_style );

  // construct canvases
  TCanvas * error_canv = new TCanvas( "err_canv", "", 100, 100, 2000, 2000 );
  error_canv->Divide( 2, 2 );
  TPad * current_pad;

  current_pad = (TPad *) error_canv->cd( 1 );
  hist_prep_axes( base, true );
  add_pad_title( current_pad, Form( "%s %s", var.c_str(), unique.c_str() ), true );
  add_atlas_decorations( current_pad, true, false );
  set_axis_labels( base, var_bound.get_x_str(), Form( "Extracted Yield %s", var_bound.get_y_str().c_str() ) );
  TLegend * base_legend = create_atlas_legend();
  base_legend->AddEntry( base, "Nominal" );
  base_legend->AddEntry( sys, "Systematic" );

  current_pad = (TPad *) error_canv->cd( 3 );
  rel_err_graph->Draw( "3" );
  unit_line->Draw( "L" );
  error_prep_axes( rel_err_graph );
  add_pad_title( current_pad, Form( "Relative Error %s %s", var.c_str(), unique.c_str() ), false );
  add_atlas_decorations( current_pad, true, false );
  set_err_axis_labels( rel_err_graph, var_bound.get_x_str(), Form( "Relative Error %s", var_bound.get_y_str().c_str() ) );

  current_pad = (TPad *) error_canv->cd( 4 );
  abs_err_graph->Draw( "3" );
  zero_line->Draw( "L" );
  error_prep_axes( abs_err_graph );
  add_pad_title( current_pad, Form( "Absolute Error %s %s", var.c_str(), unique.c_str() ), false );
  add_atlas_decorations( current_pad, true, false );
  set_err_axis_labels( abs_err_graph, var_bound.get_x_str(), Form( "Absolute Error %s", var_bound.get_y_str().c_str() ) );

  error_canv->SaveAs( Form( "%s_%s_sys.png", var.c_str(), unique.c_str() ) );

}






//class progress_bar {
//
//	public:
//		int bar_width;
//		int position;
//		float progress;
//		bool iteration_mode;
//		int start, end;
//
//
//		progress_bar( int bar_width ){
//
//			this->bar_width = bar_width;	
//			position = 0;
//			progress = 0;
//
//		}
//
//		void start( ){
//`			for ( int i = 0; i <= bar_width; i++ ) {
//        if (i < pos) std::cout << "=";
//        else if (i == pos) std::cout << ">";
//        else std::cout << " ";
//    	}
//		}
//		
//
//float progress = 0.0;
//while (progress < 1.0) {
//    int barWidth = 70;
//
//    std::cout << "[";
//    int pos = barWidth * progress;
//    for (int i = 0; i < barWidth; ++i) {
//        if (i < pos) std::cout << "=";
//        else if (i == pos) std::cout << ">";
//        else std::cout << " ";
//    }
//    std::cout << "] " << int(progress * 100.0) << " %\r";
//    std::cout.flush();
//
//    progress += 0.16; // for demonstration only
//}
//std::cout << std::endl;
//
//};


//TGraphAsymmErrors * make_diff_graph( TH1F * base, TH1F * sys, bool absolute ){
//
//  double sys_x[15];
//  double sys_y[15];
//  double err_x_upper[15];
//  double err_x_lower[15];
//  double err_y_upper[15];
//  double err_y_lower[15];
//
//  TH1F * err_hist = err_hist( base, sys, absolute )
//  int bins = base->GetNbinsX();
//  double min = base->GetXaxis()->GetBinLowEdge( 1 );
//  double width = base->GetXaxis()->GetBinUpEdge();
//
//  for ( int err_bin = 0; err_bin < bins; err_bin++){ 
//
//    sys_x[err_bin] = ( min + ( width/2.0 ) ) + width*( (double) err_bin );
//    sys_y[err_bin] = 1.0;
//    err_x_upper[err_bin] = 0.0;
//    err_x_lower[err_bin] = 0.0;
//
//    double sys_err = -1.0 + err_hist->GetBinContent( err_bin + 1 );
//
//    if ( sys_err > 0 ){
//      err_y_lower[err_bin] = 0.0;
//      err_y_upper[err_bin] = abs(sys_err);
//    } else {
//      err_y_lower[err_bin] = abs(sys_err);
//      err_y_upper[err_bin] = 0.0;
//    }
//  }
//  TGraphAsymmErrors * err_graph = new TGraphAsymmErrors( 15, sys_x, sys_y, err_x_lower, err_x_upper, 
//                                                        err_y_lower, err_y_upper);
//  err_graph->SetFillStyle(3004);
//  err_graph->SetFillColorAlpha( kBlue+2, 0.8 );
//  err_graph->SetLineWidth(0);
//
//  delete err_hist;
//
//  return err_graph;
//
//}

