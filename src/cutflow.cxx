#include <cutflow.hxx>

void cutflow::write( std::string unique ){

  void prep_style();
  std::vector<float> var_style({ 0, 0, 0, 0, 1, 2, 1, 1, 0, 0, 0} );
  
  for ( int cut_idx = 1; cut_idx < cuts; cut_idx++ ){

    TH1F * var_hist = cut_distributions.at(cut_idx-1);

    TCanvas * var_canvas = new TCanvas(Form("var_canv_%i",cut_idx),"",600,600,1500,1500);
    var_canvas->Divide(1);
    TPad * var_pad = (TPad*) var_canvas->cd(1);

    var_hist->Draw( "HIST" );
    hist_prep_axes( var_hist );
    set_axis_labels( var_hist, var_str, Form("Pass cut %i", cut_idx ) );
    style_hist( var_hist, var_style );
    TPaveStats * var_stats = make_stats( var_hist );
    var_stats->Draw();
    add_atlas_decorations( var_pad, true, false );
    std::string var_title(Form("Cut %i, %s, %s",cut_idx, cut_names[cut_idx-1].c_str(), var_str.c_str() ) );
    add_pad_title( var_pad, var_title, true );

    var_canvas->SaveAs( Form("cutflow/%s_cut-%i.png", unique.c_str(), cut_idx ) );
    delete var_canvas;

  }

  TCanvas * cutflow_canvas = new TCanvas("cutflow_canvas","",600,600,1500,1500);
  cutflow_canvas->Divide(1);
  TPad * cutflow_pad = (TPad*) cutflow_canvas->cd(1);

  cutflow_hist->Draw("HIST");
  hist_prep_axes( cutflow_hist );
  set_axis_labels( cutflow_hist, "Cut", "Events passed" );
  TAxis * cutflow_axis = (TAxis*) cutflow_hist->GetXaxis();
  for (int bin_no = 1; bin_no <= ((int) cut_names.size()); ++bin_no){
    cutflow_axis->SetBinLabel(bin_no,(cut_names[bin_no-1]).c_str());
    cutflow_axis->ChangeLabel(bin_no,-90.0);
  }
  TPaveStats * cf_stats = make_stats( cutflow_hist );
  cf_stats->Draw(); 
  add_atlas_decorations( cutflow_pad, true );

  cutflow_canvas->SaveAs(Form("cutflow/%s_cutflow.png",output_str.c_str()));
  cutflow_canvas->Close();
  delete cutflow_canvas;

}
