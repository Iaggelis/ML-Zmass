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
#include <TAxis.h>
#include <TLine.h>
#include <TRandom3.h>

using namespace std;

vector<double> *xVecPtr = new vector<double>();

double gaussPDF(double *xPtr, double par[])
{
   double x = *xPtr;
   double mu = par[0];
   double sigma = par[1];
   double f = 0;
   f = 1.0 / (sqrt(2 * M_PI * sigma * sigma)) * exp(-(x - mu) * (x - mu) / (2 * sigma * sigma));
   return f;
}

void getData(vector<double> *xVecPtr, const vector<double> &z_points)
{

   for (auto &point : z_points)
   {
      xVecPtr->push_back(point);
   }
}

void randomData(vector<double> *xVecPtr)
{
   TRandom3 R(1);

   for (int i = 0; i < 50; i++)
   {
      xVecPtr->push_back(R.Gaus(91.3, 5));
   }
}

void fcn(int &npar, double *deriv, double &f, double par[], int flag)
{

   vector<double> xVec = *xVecPtr; // xVecPtr is global
   int n = xVec.size();

   double lnL = 0.0;
   for (int i = 0; i < n; i++)
   {
      double x = xVec[i];
      double pdf = gaussPDF(&x, par);

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

   vector<double> zPoints = {93.4, 89.3, 94.9, 90.7, 89.9, 89.3, 90.9, 93.8, 91.3, 92.9, 91.4, 87.8, 91.5, 89.9, 94.4, 91.3, 88.3, 91.9, 95.4, 91.1, 92.1, 91.7, 94.3, 92.0, 89.9, 90.6, 91.7, 90.4, 92.6, 93.8, 90.9, 90.4, 89.3, 94.6, 90.6, 94.1, 89.7, 90.0, 92.8, 93.8, 93.8, 91.7, 88.2, 92.9, 86.6, 90.6, 89.9, 95.3, 92.5, 96.5};
   getData(xVecPtr, zPoints);
   // randomData(xVecPtr);

   // Initialize minuit, set initial values etc. of parameters.

   const int npar = 2; // the number of parameters
   TMinuit minuit(npar);
   minuit.SetFCN(fcn);

   double par[npar];      // the start values
   double stepSize[npar]; // step sizes
   double minVal[npar];   // minimum bound on parameter
   double maxVal[npar];   // maximum bound on parameter
   string parName[npar];

   par[0] = 90.0; // a guess
   par[1] = 1.0;
   stepSize[0] = 0.2; // take e.g. 0.1 of start value
   stepSize[1] = 0.1;
   minVal[0] = 0.00001; // if min and max values = 0, parameter is unbounded.
   maxVal[0] = 1000000;
   minVal[1] = 0.0001; // if min and max values = 0, parameter is unbounded.
   maxVal[1] = 1000;
   parName[0] = "mean";
   parName[1] = "sigma";

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

   double xmin = 75.0;
   double xmax = 120.0;
   TF1 *func = new TF1("funcplot", gaussPDF, xmin, xmax, npar);

   func->SetParameters(outpar);
   func->Draw();

   func->SetLineStyle(1); //  1 = solid, 2 = dashed, 3 = dotted
   func->SetLineColor(1); //  black (default)
   func->SetLineWidth(1);

   func->GetXaxis()->SetTitle("x");
   func->GetYaxis()->SetTitle("f(x;#xi)");

   vector<double> xVec = *xVecPtr;
   const double tickHeight = 0.05;
   TLine *tick = new TLine();
   for (int i = 0; i < xVec.size(); i++)
   {
      tick->DrawLine(xVec[i], 0, xVec[i], tickHeight);
   }

   cout << "To exit, quit ROOT from the File menu of the plot" << endl;
   theApp.Run(true);
   canvas->Close();

   delete canvas;
   delete tick;
   delete xVecPtr;

   return 0;
}
