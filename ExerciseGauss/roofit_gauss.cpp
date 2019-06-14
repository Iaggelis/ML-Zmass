#include <RooRealVar.h>
#include <RooDataSet.h>
#include <RooDataHist.h>
#include <RooGaussian.h>
#include <RooConstVar.h>
#include <RooPolynomial.h>
#include <RooAddPdf.h>
#include <RooProdPdf.h>
#include <RooFitResult.h>
#include <RooPlot.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TH1D.h>
#include <TApplication.h>
#include <vector>

using namespace RooFit;
using namespace std;

int main(int argc, char **argv)
{
   TApplication theApp("App", &argc, argv);
   TCanvas *canvas = new TCanvas();
   canvas->SetFillColor(0);
   canvas->UseCurrentStyle();
   canvas->SetBorderMode(0);
   vector<double> zPoints = {93.4, 89.3, 94.9, 90.7, 89.9, 89.3, 90.9, 93.8, 91.3, 92.9, 91.4, 87.8, 91.5, 89.9, 94.4, 91.3, 88.3, 91.9, 95.4, 91.1, 92.1, 91.7, 94.3, 92.0, 89.9, 90.6, 91.7, 90.4, 92.6, 93.8, 90.9, 90.4, 89.3, 94.6, 90.6, 94.1, 89.7, 90.0, 92.8, 93.8, 93.8, 91.7, 88.2, 92.9, 86.6, 90.6, 89.9, 95.3, 92.5, 96.5};
   auto h1 = new TH1D("h1", "LH Gauss Fit", 20, 75, 100);

   for (const auto &point : zPoints)
   {
      h1->Fill(point);
   }
   RooRealVar x("x", "x", 75, 125);
   RooDataHist dh("dh", "dh", RooArgList(x), h1);

   dh.plotOn(x.frame());
   x.frame()->Draw();
   h1->Draw("same");
   canvas->Draw();

   // // Construct a Gaussian p.d.f
   // RooRealVar m("m", "m", 0, 75, 100);
   // RooRealVar s("s", "s", 2, 0.1, 10);
   // RooGaussian gauss("gauss", "gauss(x,m,s)", x, m, s);

   // RooAddPdf("model", "model", );
   cout << "To exit, quit ROOT from the File menu of the plot" << endl;
   theApp.Run(true);
   canvas->Close();
   return 0;
}