#include "style.hxx"

void prep_style() {


  
  gROOT->SetBatch(true);

  gStyle->SetPaperSize( 20, 26 );

  gStyle->SetOptStat(0);
  gStyle->SetOptFit(1);
  gStyle->SetOptTitle(0);
  gStyle->SetPadTopMargin(0.19);
  gStyle->SetPadRightMargin(0.16);
  gStyle->SetPadBottomMargin(0.16);
  gStyle->SetPadLeftMargin(0.16);
  gStyle->SetEndErrorSize( 5 );
  gStyle->SetErrorX( 0 );

  Int_t icol = 0;
  gStyle->SetFrameBorderMode(icol);
  gStyle->SetFrameFillColor(icol);
  gStyle->SetCanvasBorderMode(icol);
  gStyle->SetCanvasColor(icol);
  gStyle->SetPadBorderMode(icol);
  gStyle->SetPadColor(icol);
  gStyle->SetStatColor(icol);


  // set title offsets (for axis label)
  gStyle->SetTitleXOffset(1.3);
  gStyle->SetTitleYOffset(1.3);

  // use large fonts
  //Int_t font=72; // Helvetica italics
  Int_t font=42; // Helvetica
  Double_t text_size = 0.035;
  gStyle->SetTextFont( font );
  gStyle->SetTextSize( text_size );
  gStyle->SetLabelFont( font,"x" );
  gStyle->SetTitleFont( font,"x" );
  gStyle->SetLabelFont( font,"y" );
  gStyle->SetTitleFont( font,"y" );
  gStyle->SetLabelFont( font,"z" );
  gStyle->SetTitleFont( font,"z" );
  gStyle->SetLabelSize( text_size,"x" );
  gStyle->SetTitleSize( text_size,"x" );
  gStyle->SetLabelSize( text_size,"y" );
  gStyle->SetTitleSize( text_size,"y" );
  gStyle->SetLabelSize( text_size,"z" );
  gStyle->SetTitleSize( text_size,"z" );

  // use bold lines and markers
  //atlasStyle->SetMarkerStyle( 20 );
  //atlasStyle->SetMarkerSize( 1 );
  gStyle->SetLineStyleString( 2,"[12 12]"); 

  // put tick marks on top and RHS of plots
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);

  gROOT->ForceStyle();

}

