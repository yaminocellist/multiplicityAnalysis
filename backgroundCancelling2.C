#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <unistd.h>
#include "TH1.h"

using namespace std;

/***************************************************************************
 *  Beginning of the main algorithm
 * *************************************************************************/

void backgroundCancelling2 () {
    ifstream lineNumbering("lineNumbering.txt");
    ifstream etaRaw       ("Eta_w_Info.txt");
    ifstream phiRaw       ("Phi_w_Info.txt");
        
    if (!lineNumbering.is_open()){
		  std::cout << "Unable to open lineNumbering" << std::endl;
		  system("read -n 1 -s -p \"Press any key to continue...\" echo");
		  exit(1);
 	  }

    if (!phiRaw.is_open()){
		  std::cout << "Unable to open phiRaw" << std::endl;
		  system("read -n 1 -s -p \"Press any key to continue...\" echo");
		  exit(1);
 	}

    if (!etaRaw.is_open()){
		  std::cout << "Unable to open etaRaw" << std::endl;
		  system("read -n 1 -s -p \"Press any key to continue...\" echo");
		  exit(1);
 	}

    string temp, temp1, temp2, temp3;
    // 'temp' is just a dummy;
    // 'temp1' stores 'n1';
    // 'temp2' stores 'n2';
    // 'temp3' stores 'n3';

    for (int i = 1; i <= 1; i++)  getline(lineNumbering, temp);
    int n1 = stoi(temp);
    for (int j = 1; j < n1; j++) {
        getline(etaRaw, temp1);
        getline(phiRaw, temp2);
    }
    int n2, n3;

    /*********************************
    *  Between n1 and n3 is ONE Event, 
    *  between n1 and n2 is layer_0, 
    *  between n2 and n3 is layer_1.
    * ********************************/

    Double_t binEdges[481];
	  double dx_1 = 0.01;
	  binEdges[0] = -2;
	  for (int i = 1; i <= 190; i++) {
    	binEdges[i] = binEdges[0] + i*dx_1;
    }
	  double dx_2 = 0.002;
	  for (int j = 1; j <= 100; j++) {
    	binEdges[j+190] = binEdges[190] + j*dx_2;
    }
	  for (int l = 1; l <= 190; l++) {
    	binEdges[l + 290] = binEdges[290] + l*dx_1;
    }

    TCanvas *c1 = new TCanvas();
    c1 -> Divide(1,2);
    TH1D *HSignal = new TH1D("HSignal","HSignal; Delta_Eta; Hits",480, binEdges);
    HSignal -> SetMarkerStyle(20);
    HSignal -> SetMarkerSize(0.5);
    HSignal -> SetLineColor(kRed);
    
    TH1D *HBackground = new TH1D("HBackground","HBackground;Delta_Eta; Hits",480, binEdges);
    HBackground -> SetMarkerStyle(21);
    HBackground -> SetMarkerSize(0.5);
    HBackground -> SetLineColor(kBlue);

    TH1D *HBackground_1 = new TH1D("HBackground_1","HBackground_1;Delta_Eta; Hits", 480, binEdges);
    HBackground_1 -> SetMarkerStyle(21);
    HBackground_1 -> SetMarkerSize(0.5);
    HBackground_1 -> SetLineColor(kBlue);
    
    double events = 0;
    double entries = 0;
    double delphi, deleta, absdphi;
    
    int maxim = 1000000;
    while (n1 <= maxim) {    // 32991645 is the 3rd-to-last line number;
        vector <double> vec1, vec2, vec3, vec4;
        getline(lineNumbering, temp2);
        n2 = stoi(temp2);
        getline(lineNumbering, temp3);
        n3 = stoi(temp3);

        getline(etaRaw, temp1);     
        getline(phiRaw, temp2);
        
        for (int i = n1; i < n2 - 1; i++){      // hits of layer_0;
            getline(etaRaw, temp1);
            getline(phiRaw, temp2);
            vec1.push_back(stod(temp1));        // storing η of layer_0;
            vec2.push_back(stod(temp2));        // storing φ of layer_0;
        }
        
        getline(etaRaw, temp1);
        getline(phiRaw, temp2);     
        for (int j = n2; j < n3 - 1; j++){      // hits of layer_1;
            getline(etaRaw, temp1);
            getline(phiRaw, temp2);
            vec3.push_back(stod(temp1));        // storing η of layer_1;
            vec4.push_back(stod(temp2));        // storing φ of layer_1;
        }
        //cout << "Event between line [" << n1 << ", " << n2 << "] and [" << n2 <<", " << n3 << "];" << endl;
        
        
        events++;
        //cout << "Currently " << events << " events, " << endl;
        
        for (int k = 0; k < n2-n1-1; k++) {
          double Eta0, Phi0;
          Eta0 = vec1[k];
          
          //cout << Eta0 << ' ';
          Phi0 = vec2[k];
          
          //cout << Phi0 << endl;
          
          for (int l = 0; l < n3-n2-1; l++) {
            double Eta1, Phi1;
            Eta1 = vec3[l];
            
            //cout << eta1 << ' ';
            Phi1 = vec4[l];
            
            //cout << phi1 << endl;
            delphi = abs(vec2[k] - vec4[l]);
            deleta = vec1[k] - vec3[l];
            if (delphi < 0.1){
              HSignal -> Fill(deleta);
            }
            else if (delphi < 0.2) {
              HBackground -> Fill(deleta);
            }
            /* else if (delphi > 0.1 && delphi < 1) {
              HBackground -> Fill(deleta);
            } */
          }
          
        }
        n1 = n3;
    }
    
    // double N1 = HBackground -> GetEntries();
    // double N2 = HSignal -> GetEntries();

    double N1 = HBackground -> Integral(HBackground->FindFixBin(-2), HBackground->FindFixBin(-0.1), "") + 
                HBackground -> Integral(HBackground->FindFixBin(0.1), HBackground->FindFixBin(2), "");
    double N2 = HSignal -> Integral(HSignal->FindFixBin(-2), HSignal->FindFixBin(-0.1), "") + 
                HSignal -> Integral(HSignal->FindFixBin(0.1), HSignal->FindFixBin(2), "");
    double N  = N2/N1;
    
    c1 -> cd(1);
    HSignal -> Sumw2();
    HSignal -> Draw("HIST SAME");
    HSignal -> Draw("e1psame");
    HSignal -> SetFillColor(kRed-10);
    HSignal -> SetFillStyle(1001);
    HSignal -> SetTitle(Form("HSignal & Normalized Background for %5.0f Events", events));
    //HSignal -> GetXaxis() -> SetRange(HSignal -> FindFixBin(-0.5),HSignal -> FindFixBin(0.5));
    HSignal -> SetFillColor(kCyan - 7);
    HSignal -> SetMarkerSize(0.55);
    HSignal -> SetFillStyle(1001);
    gPad -> SetLogy(1);

    TH1D* HNormalized = (TH1D*) HBackground->Clone("Normalized HBackground");
    HNormalized -> Add(HBackground, N-1);
    HNormalized -> Sumw2();
    HNormalized -> Draw("HIST SAME");
    HNormalized -> Draw("e1psame");

    HNormalized -> SetFillColor(kRed-7);
    HNormalized -> SetFillStyle(1001);
    //HNormalized -> GetXaxis() -> SetRange(HNormalized -> FindFixBin(-0.5),HNormalized -> FindFixBin(0.5));
  
    
    HNormalized -> SetTitle(Form("Normalized HBackground Between 0.1 and 0.2 for %5.0f Events", events));


    c1 -> cd(2);
    TH1D* hDiff = (TH1D*) HSignal->Clone("Subtracted Signal");
    hDiff -> Add(HNormalized, -1);
    //hDiff -> Sumw2();
    hDiff -> Draw("HIST SAME");
    hDiff -> Draw("e1psame");

    hDiff -> SetFillColor(kYellow - 7);
    hDiff -> SetFillStyle(1001);
    //hDiff -> GetXaxis() -> SetRange(hDiff -> FindFixBin(-0.5),hDiff -> FindFixBin(0.5));
    double peak = hDiff -> Integral(hDiff->FindFixBin(-0.1), hDiff->FindFixBin(0.1), "");
    double Ratio = peak/events;
   
   
    
    //hDiff -> Add(HBackground, -N);
    //hDiff -> Draw();
    
    hDiff -> SetTitle(Form("Subtracted Signal for %5.0f Events", events));

    gStyle->SetEndErrorSize(6);
    gStyle->SetErrorX(0.5);
    
    gStyle -> SetTitleFont(100,"t");
    gStyle -> SetTitleSize(0.065,"t");

    //gPad -> SetLogy(1);
    TLine *l = new TLine(-2,0,2,0);
	  l ->Draw("same"); 
    l -> SetLineColor(kGreen);

/***************************** Reminders *****************************/
	  /* system("say -v 'Victoria' 'Your code is done'");
	  for (int i = 1; i <= 1; i++) {
    	system("tput bel");
    	sleep(1);
  	} */
    cout << "Ns is " << N2 << ", Nb is " << N1 << endl;
    cout << "Multiplicity Density is " << Ratio << endl;
	  cout << "Well done." << endl;
}