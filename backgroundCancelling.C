#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

/***************************************************************************
 *  Start the main algorithm
 * *************************************************************************/

void backgroundCancelling () {
    ifstream lineNumbering("lineNumbering.txt");
    ifstream origin       ("../External/tracklet_data_PbPb_2.76_v5_20000.txt");
        
    if (!lineNumbering.is_open()) {
        std::cout << "Unable to open myfile" << endl;
        system("read -n 1 -s -p \"Press any key to continue...\" echo");
        exit(1);
 	}

    if (!origin.is_open()) {
        std::cout << "Unable to open myfile" << endl;
	    system("read -n 1 -s -p \"Press any key to continue...\" echo");
	    exit(1);
 	}

    string temp, temp1, temp2, temp3;

    for (int i = 1; i <= 1; i++)    getline(lineNumbering, temp1);

    int n1 = stoi(temp1);

    // for (int j = 1; j < n1; j++)
    //     getline(pairs, temp1);

    int n2, n3;
    int maxim = 32991645;   // 32991645;
    /*****************************************
    *  Between n1 and n3 is the data of ONE Event, 
    *  between n1 and n2 is layer_0, 
    *  between n2 and n3 is layer_1.
    * ***************************************/

    TCanvas *c1       = new TCanvas();
    TH1D *HAll        = new TH1D("HAll","HAll",400, -2, 2);
    TH2D *h2D         = new TH2D("2D", "2D histogram", 200, -2, 2, 200, -2*M_PI, 2*M_PI);
    TH1D *HSignal     = new TH1D("HSignal","HSignal",400, -2, 2);
    TH1D *HBackground = new TH1D("HBackground","HBackground",400, -2, 2);

    double events  = 0;
    double entries = 0;
    double firstValue, secondValue, dEta, dPhi;

    // vector <string> vec, vec1, vec2;
    vector<double> Eta0, Eta1, Phi0, Phi1;

    while (n1 <= maxim) {    // 32991645 is the 3rd-to-last line number;
        /* *****************************************************
         * vec1 is matrix storing pairs of (η, φ) in layer_0 of present event;
         * vec2 is matrix storing pairs of (η, φ) in layer_1 of present event.
         * *****************************************************/
        getline(lineNumbering, temp2);
        n2 = stoi(temp2);
        getline(lineNumbering, temp3);
        n3 = stoi(temp3);

        getline(origin, temp);     // get layer information of layer_0;
        std::istringstream check(temp);
        check >> firstValue >> secondValue;
        if (firstValue != 0) {
            for (int i = n1; i < n2 - 1; i++) {
                getline(origin, temp);
                std::istringstream line(temp);
                line >> firstValue >> secondValue;
                if (abs(secondValue) > 2*M_PI) {
                    std::cerr << "Wrong line handling!: " << firstValue << ", " << secondValue << std::endl;
                    break;
                }
                Eta0.push_back(firstValue); Phi0.push_back(secondValue);
                entries++;
            }
        }
        

        getline(origin, temp);     // get layer information of layer_1;
        std::istringstream check1(temp);
        check1 >> firstValue >> secondValue;
        if (firstValue != 0) {
            for (int j = n2; j < n3 - 1; j++) {
                getline(origin, temp);
                std::istringstream line(temp);
                line >> firstValue >> secondValue;
                if (abs(secondValue) > 2*M_PI) {
                    std::cerr << "Wrong line handling!: " << firstValue << ", " << secondValue << std::endl;
                    // break;
                    exit(1);
                }
                Eta1.push_back(firstValue); Phi1.push_back(secondValue);
            }
        }
        // std::cout << "Event between line [" << n1 << ", " << n2 << "] and [" << n2 <<", " << n3 << "];" << endl;
        
        n1 = n3;
        events++;
        std::cout << "Currently " << events << " events, " << endl;
        for (int i = 0; i < Eta0.size(); i++) {
            // std::cout << "Layer0, eta value: " << *it << std::endl;
            for (int j = 0; j < Eta1.size(); j++) {
                // std::cout << "  ---  Layer1, eta value: " << *jt << std::endl;
                dEta = Eta0[i] - Eta1[j];
                dPhi = Phi0[i] - Phi1[j];
                // HAll -> Fill(dEta);
                h2D -> Fill(dEta, dPhi);
                if (abs(dPhi) < 0.1){
                    HSignal -> Fill(dEta);
                }
                else if (abs(dPhi) < 0.2) {
                // else {
                    HBackground -> Fill(dEta);
                }
            }
        }
        Eta0.clear();   Eta1.clear();   Phi0.clear();   Phi1.clear();
        /*
        for (auto it1 = vec1.begin(); it1 != vec1.end(); it1++) {
          stringstream s1(*it1);
          string eta0, phi0;
          double Eta0, Phi0;
          s1 >> eta0;
          Eta0 = stod(eta0);
          //cout << eta0 << ' ';
          s1 >> phi0;
          Phi0 = stod(phi0);
          //cout << phi0 << endl;
          
          for (auto it2 = vec2.begin(); it2 != vec2.end(); it2++) {
            stringstream s2(*it2);
            string eta1, phi1;
            double Eta1, Phi1;
            s2 >> eta1;
            Eta1 = stod(eta1);
            //cout << eta1 << ' ';
            s2 >> phi1;
            Phi1 = stod(phi1);
            //cout << phi1 << endl;
            double delphi = abs(Phi0 - Phi1);
            double deleta = Eta0 - Eta1;
            if (delphi < 0.1){
              HSignal -> Fill(deleta);
            }
            else if (delphi < 0.2) {
              HBackground -> Fill(deleta);
            }


          }
          
        }
        */
    }
    
    TH1D* hDiff = (TH1D*) HSignal->Clone("hDiff");
    hDiff->Add(HBackground, -1.);
    
    /************************************************************/

    /* ifstream fnl("F:\\1V1MIT\\Week2\\final.txt");
    ifstream etaRaw("F:\\1V1MIT\\Week2\\etaRaw.txt");
    //ofstream dqn("F:\\1v1 MIT\\Week2\\dqn.txt");
    if (!fnl.is_open()){
		cout << "Unable to open final";
		system("pause");
		exit(1);
 	}

    if (!etaRaw.is_open()){
		cout << "Unable to open etaRaw";
		system("pause");
		exit(1);
 	}
   
    for (int i = 1; i <= 1; i++)
        getline(fnl, temp1);
    n1 = stoi(temp1);
    
    TH1D *hist_delta_eta = new TH1D("hist_delta_eta","Delta_Eta for 19994 Events",100,-2,2);
     events = 0;
     entries = 0;

    while (n1<= maxim) {    // 32991645 is the 3rd-to-last line number;
        vector <double> vec, vec1, vec2;
        getline(fnl, temp2);
        n2 = stoi(temp2);
        getline(fnl, temp3);
        n3 = stoi(temp3);

        getline(etaRaw, temp);
        for (int i = n1; i < n2 - 1; i++){
            getline(etaRaw, temp);
            vec1.push_back(stod(temp));
        }

        getline(etaRaw, temp);
        for (int j = n2; j < n3 - 1; j++){
            getline(etaRaw, temp);
            vec2.push_back(stod(temp));
        }
        //cout << "Event between line [" << n1 << ", " << n2 << "] and [" << n2 <<", " << n3 << "];" << endl;
        
        vec = GetDel(vec1, vec2);
        
        for (auto it = vec.begin(); it != vec.end(); it++)
	    {
		    entries++;
            hist_delta_eta -> Fill(*it);
            //dqn << *it << ' ';
        }
       
        events++;
        //cout << "Currently " << events << " events, " << fixed <<setprecision(0) << entries << " entries;" << endl;
        
        n1 = n3;
        //hist_delta_eta ->Draw("same");
        //hist_delta_eta -> GetYaxis()->SetRangeUser(0., 300000);
        
    }
    





    


    
    hist_delta_eta -> GetXaxis() -> SetTitle("Delta_Eta"); 
    hist_delta_eta -> GetXaxis() -> SetTitleSize(0.06);
    hist_delta_eta -> GetXaxis() -> SetTitleFont(20);
    hist_delta_eta -> GetYaxis() -> SetTitle("Number of Data");
    hist_delta_eta -> GetYaxis() -> SetTitleSize(0.06);
    hist_delta_eta -> GetYaxis() -> SetTitleFont(20);
    hist_delta_eta -> Draw("SAME"); */
    
    h2D -> Draw("colz");
    h2D -> SetTitle(Form("Filtered Signal for %5.0f Events", events));
    hDiff -> SetFillStyle(3001);    hDiff -> SetFillColor(kCyan - 7);
    gStyle -> SetTitleFont(100,"t");
    gStyle -> SetTitleSize(0.065,"t");
    //gPad ->SetLogy(1);

    cout << "Well done." << endl;

}









