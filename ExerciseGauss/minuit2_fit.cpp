#include <iostream>
#include <cmath>
#include <string>
#include <vector>

#include <TApplication.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TH1.h>
#include <TAxis.h>
#include <TLine.h>
#include <TVirtualFitter.h>


using namespace std;

vector<double> *xVecPtr = new vector<double>();

void getData(vector<double> *xVecPtr, const vector<double> &z_points)
{

   for (auto &point : z_points)
   {
      xVecPtr->push_back(point);
   }
}

int main(int argc, char **argv)
{

   TApplication theApp("App", &argc, argv);
   TCanvas *canvas = new TCanvas("Canvas", "Gaussian Fit", 10, 10, 900, 900);
   canvas->SetFillColor(0);
   canvas->UseCurrentStyle();
   canvas->SetBorderMode(0);      // still leaves red frame bottom and right
   canvas->SetFrameBorderMode(0); // need this to turn off red hist frame!
   gROOT->SetStyle("Plain");
   canvas->UseCurrentStyle();
   gROOT->ForceStyle();

   gStyle->SetOptStat(1111111);
   gStyle->SetOptFit(1111111);
   gStyle->SetTitleBorderSize(0);
   gStyle->SetTitleSize(0.04);
   gStyle->SetTitleFont(42, "hxy"); // for histogram and axis titles
   gStyle->SetLabelFont(42, "xyz"); // for axis labels (values)
   gROOT->ForceStyle();

   vector<double> zPoints = {93.4, 89.3, 94.9, 90.7, 89.9, 89.3, 90.9, 93.8, 91.3, 92.9, 91.4, 87.8, 91.5, 89.9, 94.4, 91.3, 88.3, 91.9, 95.4, 91.1, 92.1, 91.7, 94.3, 92.0, 89.9, 90.6, 91.7, 90.4, 92.6, 93.8, 90.9, 90.4, 89.3, 94.6, 90.6, 94.1, 89.7, 90.0, 92.8, 93.8, 93.8, 91.7, 88.2, 92.9, 86.6, 90.6, 89.9, 95.3, 92.5, 96.5};
   getData(xVecPtr, zPoints);

   // Minuit2 not working

   // string type = "Minuit2";

   // TVirtualFitter::SetDefaultFitter(type.c_str());

   auto h1 = new TH1D("h1", "LH Gauss Fit", 100, 75, 100);
   auto h2 = new TH1D("h2", "Chi2 Gauss Fit", 100, 75, 100);
   auto h3 = new TH1D("h3", "Chi2Minos Gauss Fit", 100, 75, 100);
   auto h4 = new TH1D("h4", "Chi2IntMinos Gauss Fit", 100, 75, 100);

   for (const auto &point : zPoints)
   {
      h1->Fill(point);
      h2->Fill(point);
      h3->Fill(point);
      h4->Fill(point);
   }
   canvas->Divide(2, 2);

   canvas->cd(1);
   h1->Fit("gaus", "LE"); //Likelihood Fit with Minos Error
   h1->Draw();
   canvas->cd(2);
   h2->Fit("gaus", "Q"); // Chi2 Fit
   h2->Draw();
   canvas->cd(3);
   h3->Fit("gaus", "QE"); //Chi2 Fit with  Minos
   h3->Draw();
   canvas->cd(4);
   h4->Fit("gaus", "IE"); //Chi2 Fit with Integral and Minos
   h4->Draw();

  
   cout << "To exit, quit ROOT from the File menu of the plot" << endl;
   theApp.Run(true);
   canvas->Close();

   delete canvas;
   delete xVecPtr;

   return 0;
}
