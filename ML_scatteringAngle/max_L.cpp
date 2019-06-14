#include <iostream>
#include <cmath>
#include <string>
#include <vector>

#include <TMinuit.h>
#include <TApplication.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TF1.h>
#include <TH1D.h>
#include <TAxis.h>
#include <TLine.h>
#include <TRandom3.h>

using namespace std;

vector<double> *xVecPtr = new vector<double>();

double scatAngle(double *xPtr, double par[] /*, const double &xmin, const double &xmax*/)
{
   double x = *xPtr;
   double a = par[0];
   double b = par[1];
   double f = 0;
   double nom = (1.0 + a * x + b * x * x);
   // double denom = (xmax - xmin) + 0.5 * a * (xmax * xmax - xmin * xmin) + (xmax * xmax * xmax - xmin * xmin * xmin) * b / 3.0;
   // f = (1.0 + a * x + b * x * x) / (2.0 + 2.0 * b / 3.0);
   f = nom;
   return f;
}

double AnglePDF(double *xPtr, double par[])
{
   double x = *xPtr;
   double a = par[0];
   double b = par[1];
   double f = 0;

   // double nom = (1.0 + a * x + b * x * x);
   // double denom = (xmax - xmin) + 0.5 * a * (xmax * xmax - xmin * xmin) + (xmax * xmax * xmax - xmin * xmin * xmin) * b / 3.0;
   // f = nom;

   f = (1.0 + a * x + b * x * x) / (2.0 + 2.0 * b / 3.0);
   return f;
}

void randomData(vector<double> *xVecPtr)
{
   TF1 *pdf = new TF1("funcplot", scatAngle, -0.95, 0.95, 2);
   pdf->SetParameters(0.5, 0.5);

   for (int i = 0; i < 2000; i++)
   {
      xVecPtr->push_back(pdf->GetRandom(-0.95, 0.95));
   }
   delete pdf;
}

void fcn(int &npar, double *deriv, double &f, double par[], int flag)
{

   vector<double> xVec = *xVecPtr; // xVecPtr is global
   // double xmax = *max_element(xVec.begin(), xVec.end());
   // double xmin = *min_element(xVec.begin(), xVec.end());
   double xmax = 0.95;
   double xmin = -0.95;
   double denom = (xmax - xmin) + 0.5 * par[0] * (xmax * xmax - xmin * xmin) + (xmax * xmax * xmax - xmin * xmin * xmin) * par[1] / 3.0;

   int n = xVec.size();

   double lnL = 0.0;
   for (int i = 0; i < n; i++)
   {
      double x = xVec[i];
      double pdf = scatAngle(&x, par) / denom;

      if (pdf > 0.0)
      {
         lnL += log(pdf); // need positive f
      }
      else
      {
         cout << "WARNING -- pdf is negative!!!" << endl;
      }
   }
   f = -2.0 * lnL; // factor of -2 so minuit gets the errors right
}

int main(int argc, char **argv)
{
   TApplication theApp("App", &argc, argv);
   TCanvas *canvas = new TCanvas();
   canvas->SetFillColor(0);
   canvas->UseCurrentStyle();
   canvas->SetBorderMode(0);      // still leaves red frame bottom and right
   canvas->SetFrameBorderMode(0); // need this to turn off red hist frame!
   gROOT->SetStyle("Plain");
   canvas->UseCurrentStyle();
   gROOT->ForceStyle();

   // gStyle->SetOptStat(111111);
   gStyle->SetTitleBorderSize(0);
   gStyle->SetTitleSize(0.04);
   gStyle->SetTitleFont(42, "hxy"); // for histogram and axis titles
   gStyle->SetLabelFont(42, "xyz"); // for axis labels (values)
   gROOT->ForceStyle();

   randomData(xVecPtr);

   // Initialize minuit, set initial values etc. of parameters.
   const int npar = 2; // the number of parameters
   TMinuit minuit(npar);
   minuit.SetFCN(fcn);

   double par[npar];      // the start values
   double stepSize[npar]; // step sizes
   double minVal[npar];   // minimum bound on parameter
   double maxVal[npar];   // maximum bound on parameter
   string parName[npar];

   par[0] = 1.0; // a guess
   par[1] = 1.0;
   stepSize[0] = 0.1; // take e.g. 0.1 of start value
   stepSize[1] = 0.1;
   minVal[0] = 0.0001; // if min and max values = 0, parameter is unbounded.
   maxVal[0] = 10000;
   minVal[1] = 0.0001; // if min and max values = 0, parameter is unbounded.
   maxVal[1] = 10000;
   parName[0] = "a";
   parName[1] = "b";

   for (int i = 0; i < npar; i++)
   {
      minuit.DefineParameter(i, parName[i].c_str(),
                             par[i], stepSize[i], minVal[i], maxVal[i]);
   }

   // Do the minimization!

   minuit.Migrad(); // Minuit's best minimization algorithm
   double outpar[npar], err[npar];
   for (int i = 0; i < npar; i++)
   {
      minuit.GetParameter(i, outpar[i], err[i]);
   }

   //Plot the result.For this example plot x values as tick marks.

   double xmin = -1.0;
   double xmax = 1.0;
   TF1 *func = new TF1("funcplot", AnglePDF, xmin, xmax, npar);

   func->SetParameters(outpar);
   // func->Draw();

   func->SetLineStyle(1); //  1 = solid, 2 = dashed, 3 = dotted
   func->SetLineColor(1); //  black (default)
   func->SetLineWidth(1);

   func->GetXaxis()->SetTitle("x");
   func->GetYaxis()->SetTitle("f(x;#xi)");

   vector<double> xVec = *xVecPtr;
   auto dataHist = new TH1D("dataHist", "Points", 50, -1.0, 1.0);
   for (const auto &point : xVec)
   {
      dataHist->Fill(point);
   }
   // dataHist->Scale(1.0 / 2000);
   dataHist->DrawNormalized("SAME");

   cout << "To exit, quit ROOT from the File menu of the plot" << endl;
   theApp.Run(true);
   canvas->Close();

   delete canvas;
   // delete tick;
   delete xVecPtr;

   return 0;
}
