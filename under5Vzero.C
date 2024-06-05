#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

/***************************************************************************
 *  Beginning of the main algorithm
 * *************************************************************************/

void under5Vzero () {
    ifstream myfile("lineNumbering.txt");
    ifstream phiRaw("Phi_w_Info.txt");
    ifstream phiRaw_again("Phi_w_Info.txt");
    ofstream outfile("under5Vzero.txt");
    if (!myfile.is_open()){
		cout << "Unable to open linelabel" << endl;
		system("read -n 1 -s -p \"Press any key to continue...\" echo");
		exit(1);
 	  }
    if (!phiRaw.is_open()){
		cout << "Unable to open etaRaw" << endl;
		system("read -n 1 -s -p \"Press any key to continue...\" echo");
		exit(1);
 	}
    if (!outfile.is_open()){
		cout << "Unable to open outfile" << endl;
		system("read -n 1 -s -p \"Press any key to continue...\" echo");
		exit(1);
 	}
        
    int nline_1, nline_2;
    double VZERO;
	vector<double> vec; 					// temporally store all VZERO values
    double mini, maxi;
    
    string linenumber;
    string dummy;                           // temporally store one VZERO value
    getline(myfile, linenumber);
    nline_1 = stoi(linenumber);
    for (int i = 1; i <= nline_1; i++) {
        getline(phiRaw, dummy);    
    }
    VZERO = stod(dummy);
	vec.push_back(VZERO);
    mini = VZERO;
    maxi = VZERO;
    //cout << VZERO << endl;
	int counter = 1;
    int max = 32991645;
    while (nline_1 <= max) {
        getline(myfile, linenumber);
        nline_2 = stoi(linenumber);
        for (int j = nline_1 + 1; j <= nline_2; j++) {
            getline(phiRaw, dummy);    
        }
        VZERO = stod(dummy);
		vec.push_back(VZERO);
        if (mini >= VZERO)
            mini = VZERO;
        if (maxi <= VZERO)
            maxi = VZERO;
        //cout << VZERO << endl;
		counter++;
		
        nline_1 = nline_2;
		
    }
    cout << "Max = " << maxi << ", Min = " << mini << ", Events = " << counter/2 << endl;

    TCanvas *c1 = new TCanvas();
	TH1D *Vzero = new TH1D("Vzero","Vzero Distribution; Value; Counts",586,mini,maxi);
	for (auto it = vec.begin(); it != vec.end(); it++) {
		Vzero -> Fill(*it);
		it++;
	}
	Vzero -> SetLineWidth(2.);
	Vzero -> Draw();
	double ratio, thresh;

	for (double x = 1; x <= maxi; x++) {
		double h = Vzero -> Integral(Vzero->FindFixBin(0), Vzero->FindFixBin(x), "");
		ratio = h/(counter/2);
		if (ratio >= 0.95) {
			thresh = x;			
			break;	
		}	
	}
    
    string dm, s;            // temporally stores the line number (label) whose VZERO is >= 15198
    double temp;             // temporally stores current VZERO value
    double cc = 0;
    while (getline(phiRaw_again, dm)) {
            cc++;
            temp = stod(dm);
            if (temp >= thresh) {
                stringstream stream;
                stream << fixed << setprecision(0) << cc;
                s = stream.str();
                outfile << s << endl;
            }
    }

    Vzero -> SetTitle("VZEROs Distribution");

    TH1D *Vthresh = new TH1D("Vthresh","Vzero up to 5 percents; Value; Counts",586,mini,maxi);
    for (auto it = vec.begin(); it != vec.end(); it++) {
		if (*it >= 15198) {
            Vthresh -> Fill(*it);
        }
        it++;
	}
    Vthresh -> Draw("HIST SAME");
    Vthresh -> SetLineColor(kRed);
    Vthresh -> SetFillColor(kRed - 7);

    TLegend *legend = new TLegend(0.15, 0.8, 0.6, 0.9);
    legend -> AddEntry(Vthresh, "VZERO Values count for 0-5%", "f");
    legend -> SetHeader("Red Region:");
    gStyle -> SetLegendTextSize(0.04);
    legend -> Draw("same");

    gPad ->SetLogy(1);
	/* TLine *l = new TLine(thresh,0,thresh,1000);
	l ->Draw("same"); */
	cout << ratio << ' ' << thresh << endl;
    double ih = Vzero -> Integral(Vzero->FindFixBin(0), Vzero->FindFixBin(15164), "");
    cout << ih << endl;
    cout << vec.size() << endl;
    cout << Vzero -> GetEntries() << endl;
}