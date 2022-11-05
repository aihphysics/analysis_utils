
#ifndef utils_hxx
#define utils_hxx

#include "common.hxx"
#include "bound.hxx"
#include "bound_mgr.hxx"

extern "C" void hist_prep_axes( TH1 * hist, bool zero=false );
extern "C" void error_prep_axes( TGraphAsymmErrors * err );
extern "C" TH1F * errorbar_to_hist( TH1F * hist, bool absolute=true );
extern "C" TH1F * hist_to_errorbar( TH1F * base, TH1F * err_hist, bool absolute=true );
extern "C" TH1F * quadrature_error_combination( TH1F * stat, std::vector<TH1F *> systematic, bool sys_only=false);
extern "C" TH1F * single_sys_to_error( TH1F * base, TH1F * sys );
extern "C" TGraphAsymmErrors * err_graph_calc( TH1F * base, TH1F * sys, bool absolute=true );
extern "C" void make_hf_fit_histogram( char bin_name[150], char err_name[150], TFile * source_file, TH1F * reco_hist, int err_bin);
extern "C" std::map< std::string, std::vector< double > > prep_binnings( std::string input_range );
extern "C" void add_pad_title( TPad * active_pad, std::string title, bool sci_axis=true );
extern "C" void add_atlas_decorations( TPad * active_pad, bool wip=true, bool sim=false );
extern "C" void combine_sys_group( std::vector< TH1F *> & group_systematics, TH1F * group_hist, TH1F * base );
extern "C" void split_strings(std::vector<std::string> & vec_split, std::string str_split, std::string delim);
extern "C" TLegend * create_atlas_legend();
extern "C" void style_hist( TH1F * hist, std::vector< float > & style_vec );
// add ability to style fits
//extern "C" void style_hist( TH1F * hist, std::vector< float > & style_vec );
extern "C" void set_axis_labels( TH1F * hist, std::string xaxis, std::string yaxis );
extern "C" void set_err_axis_labels( TGraphAsymmErrors err, std::string xaxis, std::string yaxis );
extern "C" void set_2d_axis_labels( TH2F * hist, std::string xaxis, std::string yaxis );
extern "C" TLegend * below_logo_legend();
extern "C" TPaveStats * make_stats( TH1 * hist, bool small=false, bool shifted=false );
extern "C" TLegend * create_stat_legend();
extern "C" void align_sg( TF1 * sg_func, TH1F * hist, bool limit=false );
extern "C" void align_dg( TF1 * sg_func, TH1F * hist, bool limit=false );
extern "C" void style_func( TF1 * func, std::vector<float> & style_vec );
extern "C" TF1 * prep_dg( float min = -100, float max = 100 );
extern "C" TF1 * prep_sg( float min = -100, float max = 100 );
extern "C" TF1 * prep_line( float min = -100, float max = 100 );
extern "C" TH1F * sys_to_error_hist( TH1F * base, TH1F * sys, bool absolute=true );
extern "C" void compare_systematic( TH1F * base, TH1F * sys, const std::string & var, const std::string & unique );


#endif
