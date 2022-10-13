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

void prep_style(){
  gStyle->SetEndErrorSize( 15 );
  SetAtlasStyle();
  gStyle->SetEndErrorSize( 15 );
  gROOT->SetBatch(true);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(1);
  gStyle->SetPadTopMargin(0.19);
  gStyle->SetPadRightMargin(0.16);
  gStyle->SetPadBottomMargin(0.16);
  gStyle->SetPadLeftMargin(0.16);
  gStyle->SetEndErrorSize( 15 );
}



void hist_prep_axes( TH1F * hist ){
  hist->GetYaxis()->SetRangeUser( 0, hist->GetMaximum()*1.75 );
  hist->GetYaxis()->SetLabelSize( 0.035 );
  hist->GetYaxis()->SetTitleSize( 0.035 );
  hist->GetXaxis()->SetLabelSize( 0.035 );
  hist->GetXaxis()->SetTitleSize( 0.035 );
  hist->GetYaxis()->SetMaxDigits( 3 );
}

void hist_prep_data( TH1F * hist ){
  hist->GetYaxis()->SetRangeUser( 0, hist->GetMaximum()*1.5 );
  hist->SetLineWidth( 1 );
  hist->SetLineColorAlpha( 1, 0.8 );
  hist->GetYaxis()->SetLabelSize( 0.035 );
  hist->GetYaxis()->SetTitleSize( 0.035 );
  hist->GetXaxis()->SetLabelSize( 0.035 );
  hist->GetXaxis()->SetTitleSize( 0.035 );
  hist->GetYaxis()->SetMaxDigits( 3 );
}

void hist_prep_sign( TH1F * hist ){
  hist->GetYaxis()->SetRangeUser( 0, hist->GetMaximum()*1.5 );
  hist->SetLineWidth( 1 );
  hist->SetLineColorAlpha( kRed+1, 1 );
  hist->GetYaxis()->SetLabelSize( 0.035 );
  hist->GetYaxis()->SetTitleSize( 0.035 );
  hist->GetXaxis()->SetLabelSize( 0.035 );
  hist->GetXaxis()->SetTitleSize( 0.035 );
  hist->GetYaxis()->SetMaxDigits( 3 );
}

void hist_prep_bbbg( TH1F * hist ){
  hist->GetYaxis()->SetRangeUser( 0, hist->GetMaximum()*1.5 );
  hist->SetLineWidth( 1 );
  hist->SetLineColorAlpha( kGreen+1, 0.9 );
  hist->GetYaxis()->SetLabelSize( 0.035 );
  hist->GetYaxis()->SetTitleSize( 0.035 );
  hist->GetXaxis()->SetLabelSize( 0.035 );
  hist->GetXaxis()->SetTitleSize( 0.035 );
  hist->GetYaxis()->SetMaxDigits( 3 );
}


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

void add_atlas_decorations( TPad * active_pad, bool wip, bool sim ){
  active_pad->cd();
  TLatex * atlas_logo_ltx = new TLatex(); 
  atlas_logo_ltx->SetNDC(); 
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
  if ( sim ){ sim_ltx->DrawLatexNDC( 0.187, 0.705, "Simulation" ); }
	if ( wip && !sim ){
    wip_ltx->DrawLatexNDC( 0.187, 0.705, "Work In Progress" );
	} else if ( wip && sim ){
    wip_ltx->DrawLatexNDC( 0.187, 0.695, "Work In Progress" );
	}
}

TLegend * create_atlas_legend(){
  TLegend * pad_legend = new TLegend( 0.6, 0.6, 0.88, 0.78 );
  pad_legend->SetBorderSize( 0 );
  pad_legend->SetFillColor( 0 );
  pad_legend->SetFillStyle( 0 );
  pad_legend->SetTextFont( 42 );
  pad_legend->SetTextSize( 0.015 );
  return pad_legend;
}

TLegend * below_logo_legend(){
  TLegend * pad_legend = new TLegend( 0.187, 0.6, 0.4, 0.68 );
  pad_legend->SetBorderSize( 0 );
  pad_legend->SetFillColor( 0 );
  pad_legend->SetFillStyle( 0 );
  pad_legend->SetTextFont( 42 );
  pad_legend->SetTextSize( 0.015 );
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

TPaveStats * get_fit_stats( TH1F * hist ){
  gPad->Modified(); gPad->Update();
  TPaveStats * stats = (TPaveStats*) hist->FindObject("stats");
  stats->SetX1NDC( 0.56 ); 
  stats->SetX2NDC( 0.81 );
  stats->SetY1NDC( 0.55 ); 
  stats->SetY2NDC( 0.78 );
  stats->SetFillStyle( 0 ); 
  stats->SetTextFont( 42 );
  stats->SetTextSize( 0.02 );
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

