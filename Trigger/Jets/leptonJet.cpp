

//c++ leptonJet.cpp -o leptonJet `root-config --cflags --glibs`

#include <iostream>
#include <cstdlib>
#include <TCanvas.h>
#include <TApplication.h>
#include <cmath>
#include <TStyle.h>
#include <TF1.h>
#include <TH1F.h>
#include <fstream>
#include <TMath.h>
#include <TH2F.h>
#include <TPad.h>
#include <TRandom.h>
#include <TLatex.h>
#include "TH1.h"
#include "TFile.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "TTreeReaderArray.h"
#include <TPaveStats.h>
#include <TLegend.h>
#include <THStack.h>
#include <string>
#include <cstring>
#include <stdio.h>
#include <limits>

using namespace std;

int main (int argc, char** argv) {
	
	//Apro il file in cui sono contenuti gli eventi di segnale
	TFile * input = TFile::Open( "unweighted_events.root" );

	//Salvo le variabili passate a terminale in una stringa	
	double jet_pt_cut, lep_pt_cut;

	cout << "Inserisci il valore del taglio da applicare al momento trasverso del jet: " << endl;
	cin >> jet_pt_cut;

	cout << "Inserisci il valore del taglio da applicare al momento trasverso del leptone: " << endl;
	cin >> lep_pt_cut;

	int lep;

	cout << "Il leptone è un muone o un elettrone? " << endl;
	cout << "1: muone    2: elettrone" << endl;
	cin >> lep;

	//Lettura del tree contenuto nel file
	TTreeReader reader("tree", input);

	//Jet transverse momentum	
	TTreeReaderArray<double>  first_jet( reader, "j1_pt");
	TTreeReaderArray<double>  second_jet( reader, "j2_pt");
	TTreeReaderArray<double>  third_jet( reader, "jb1_pt");
	TTreeReaderArray<double>  fourth_jet( reader, "jb2_pt");

	//Leptons transverse momentum
	TTreeReaderArray<double>  electron_pt( reader, "ele_pt");
	TTreeReaderArray<double>  muon_pt( reader, "mu_pt");

	double survivors = 0;

	while (reader.Next()) {

		//Loop over the events
		for (int iVar = 0; iVar < first_jet.GetSize(); ++iVar) {

			//Ordino i pt dal più piccolo al più grande
			vector<double> jets {first_jet[iVar], second_jet[iVar], third_jet[iVar], fourth_jet[iVar] };
			sort(jets.begin(), jets.end());

			//Se ho un muone:
			if (lep == 1) {

				if (jets[3] == first_jet[iVar]) {

					if (jets[3] > jet_pt_cut && muon_pt[iVar] > lep_pt_cut) {

						survivors += 1;
					}
				}

				if (jets[3] == second_jet[iVar]) {

					if (jets[3] > jet_pt_cut && muon_pt[iVar] > lep_pt_cut) {

						survivors += 1;
					}
				}

				if (jets[3] == third_jet[iVar]) {

					if (jets[3] > jet_pt_cut && muon_pt[iVar] > lep_pt_cut) {

						survivors += 1;
					}
				}

				if (jets[3] == fourth_jet[iVar]) {

					if (jets[3] > jet_pt_cut && muon_pt[iVar] > lep_pt_cut) {

						survivors += 1;
					}
				}
			} //end of lep == 1

			//Se ho un elettrone:
			if (lep == 2) {

				if (jets[3] == first_jet[iVar]) {

					if (jets[3] > jet_pt_cut && electron_pt[iVar] > lep_pt_cut) {

						survivors += 1;
					}
				}

				if (jets[3] == second_jet[iVar]) {

					if (jets[3] > jet_pt_cut && electron_pt[iVar] > lep_pt_cut) {

						survivors += 1;
					}
				}

				if (jets[3] == third_jet[iVar]) {

					if (jets[3] > jet_pt_cut && electron_pt[iVar] > lep_pt_cut) {

						survivors += 1;
					}
				}

				if (jets[3] == fourth_jet[iVar]) {

					if (jets[3] > jet_pt_cut && electron_pt[iVar] > lep_pt_cut) {

						survivors += 1;
					}
				}
			} //end of lep == 2

		}

	 }//end of while

	double efficiency = survivors/1000000;

	cout << "L'efficienza vale: " << efficiency << endl;

	return 0;
}
