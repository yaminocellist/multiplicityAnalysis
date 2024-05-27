#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <unistd.h>

using namespace std;

/*********************************************************************
 * Inspects whether a number is odd or even;
 * In practice, if it's layer_0, it returns '0'; else, it returns '1';
 * ******************************************************************/

int OddEven(double f){
    int n = static_cast<int>(f);
    if (n%2 == 0)
        return 1;
    else
        return 0;
}

/*********************************************************************
 * Inspects whether a number is integer;
 * In practice, if it's integer, returns TRUE, else FALSE;
 * ******************************************************************/

bool IsInt (double n) {
  int res = n/1;
  if (res == n)
    return true;
  else
    return false;
}

/********************************************************************************************************
 * Main function, implements:
 * 1. Divide the original file into "Eta+Hits.txt" and "Phi+VZERO.txt" two files;
 * 2. Create a file indicates information lines in form: "hits number + VZERO value";
 * 3. Create a file indicates on which line (line numnber) lays the information;
 * ******************************************************************************************************/

void rawFileProcessing () {
/*************************************************************************************
* I/O and double checks;
* ************************************************************************************/
  ifstream myfile("../External/tracklet_data_PbPb_2.76_v5_20000.txt");
  ofstream Eta_w_Info("Eta_w_Info.txt");
  ofstream Phi_w_Info("Phi_w_Info.txt");
  ofstream allInfo("All_Layer_Information.txt");
  ofstream layerCounts("LineLabel.txt");
  ofstream vzeroValue("Vzero.txt");
  ofstream vzero_w_line("vzero_w_line.txt");
  if (!myfile.is_open()){
    cout << "Unable to open myfile" << endl;
	  system("read -n 1 -s -p \"Press any key to continue...\" echo");
	  exit(1);
 	}
  
/********************************** end of I/O **********************************/

	string temp;                // stores one line of data;
	string left, right;         // respectively stores left and right column;
  int counter = 1;            // indicates now is the ??th line of the original file;
  double leftValue, rightValue;             // indicates the left column value, the right column value;
  double NumOfLayer = 0;
  double LabelOfLines;
  while(getline(myfile, temp)) {
    left = temp.substr(0, temp.find(' '));     // datum of Eta_w_Hits (before the space)
    right = temp.substr(temp.find(' ') + 1);     // datum of Phi_w_Hits (after the space)
    
    /****************************************************
     * Writes 'Eta_w_Hits.txt' and 'Phi_w_Hits.txt' files;
     * *************************************************/
    Eta_w_Hits << left << endl;
    Phi_w_Hits << right << endl;
    
    /***************************************************************************************
     * Writes 'All_Layer_Information.txt'; Format:
     * 'Now is the ?th line'+'the ??th layer'+'hitNumber'+'VZEROValue'+'layer_0 or layer_1';
     * ************************************************************************************/
    leftValue = stod(left);
    rightValue = stod(right);
    if (leftValue >=0 && IsInt(leftValue)) {          // the number of hits must be nonnegative integer;
      NumOfLayer++;
      LabelOfLines = OddEven(NumOfLayer);
      info << counter << ' ' << NumOfLayer << ' ' << left << ' ' << right << ' ' << "Layer_" << LabelOfLines << endl;
      if (IsInt(rightValue) && rightValue >= 0) {     // by human checking, the VZERO value should be non negative integer;
        LayerCounts << counter << endl;               // Writes 'LineLabel.txt';
        vzero << right << endl;
        vzero_w_line << counter << ' ' << right << endl;
      }
    }
    counter++;
  }
  LayerCounts << 32991655;
/***************************** Reminders *****************************/
  system("say -v 'Victoria' 'Your code is done'");
  for (int i = 1; i <= 1; i++) {
    system("tput bel");
    sleep(1);
  }
  cout << "Well done." << endl;
}
