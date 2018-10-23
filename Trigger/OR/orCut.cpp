
/* 
| The program creates a map with TCut objects identified with their trigger path names. The purpose is 
| to find the couple of trigggers with the highest efficiency; the results are printed on the terminal.
|
| To compile:
| c++ -o orCut orCut.cpp `root-config --cflags --glibs`
*/

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
#include "TTreeReaderValue.h"
#include <TPaveStats.h>
#include <TLegend.h>
#include <THStack.h>
#include <string>
#include <cstring>
#include <stdio.h>
#include <limits>

#include <map>
#include <TCut.h>
#include <list> 


using namespace std;

int main(int argc, char** argv){

	//Access to the file with the events
	TFile * f = new TFile("TTrees.root");

	TTree * tMu;
	TTree * tEle;

	//Take the TTrees 
	f->GetObject("mu_tree", tMu);
	f->GetObject("ele_tree", tEle);

	//Create two maps for the trigger path names and cuts (for muons and electrons) 
	map<string, TCut> muTriggers;
	map<string, TCut> eleTriggers;

	//Create another map and a vector for the results;
	vector<double> ele_results; 
	map<double, string> ele_orCuts;

	vector<double> mu_results; 
	map<double, string> mu_orCuts;

	/*
	| First case: an electron is produced. 
	| Cuts are saved in a TCut object and inserted in the map. The keys are the HLT path names.
	*/

	TCut first_eleCut = "av_max > 300";

	TCut c1 = "ele_pt > 28";
	TCut c2 = "abs(ele_eta) < 2.1";
	TCut c3 = "HT > 150";	
	TCut second_eleCut = c1 && c2 && c3;

	TCut third_eleCut = "ele_pt > 32";

	TCut fourth_eleCut = "HT > 1050";

	TCut fifth_eleCut = "MET > 200";

	TCut c4 = "ele_pt > 30";
	TCut c5 = "abs(ele_eta) < 2.1";
	TCut c6 = "jmax_pt > 35";
	TCut c7 = "abs(jmax_eta) < 1.5";
	TCut sixth_eleCut = c4 && c5 && c6 && c7;

	TCut c8 = "MET > 105";
	TCut c9 = "ele_pt > 50";
	TCut seventh_eleCut = c8 && c9;

	TCut c10 = "MET > 110";
	TCut c11 = "MHT > 110";
	TCut eighth_eleCut = c10 && c11; 

	//Now fill the map
	eleTriggers["HLT_DiPFJetAve300_HFJEC_v*"] = first_eleCut;
	eleTriggers["HLT_Ele28_eta2p1_WPTight_Gsf_HT150_v*"] = second_eleCut;
	eleTriggers["HLT_Ele32_WPTight_Gsf_v*"] = third_eleCut;
	eleTriggers["HLT_PFHT1050_v*"] = fourth_eleCut;
	eleTriggers["HLT_PFMET200_HBHE_BeamHaloCleaned_v*"] = fifth_eleCut;
	eleTriggers["HLT_Ele30_eta2p1_WPTight_Gsf_CentralPFJet35_EleCleaned_v*"] = sixth_eleCut;
	eleTriggers["HLT_MET105_IsoTrk50_v*"] = seventh_eleCut;
	eleTriggers["HLT_PFMET110_PFMHT110_IDTight_CaloBTagDeepCSV_3p1_v*"] = eighth_eleCut;

	//Create two iterators.. 
	map<string, TCut>:: iterator first_it;
	map<string, TCut>:: iterator second_it;

	int events = 0;
	double survivors = 0;
	double efficiency = 0;

	//.. and find all the possible combinations
	for (first_it = eleTriggers.begin(); first_it != eleTriggers.end(); ++first_it ) {

		for (second_it = next(first_it); second_it != eleTriggers.end(); ++second_it) {

			//Get the number of entries: that will be the number of events				
			events = tEle->GetEntries();

			//Get the number of events that survive the current combination of cuts
			survivors = tEle->GetEntries(first_it->second || second_it->second);

			efficiency = survivors / events;

			//Efficiency values are saved in a vector
			ele_results.push_back(efficiency);

			//Save this combination of triggers in a string..
			TString triggers = (first_it->first);
				triggers += (" || ");
				triggers += (second_it->first); 

			//.. and save it in the other map
			ele_orCuts[efficiency] = triggers;
		}
	}

	//Now sort the efficiency values in the vectors 
	sort(ele_results.begin(), ele_results.end()); 	

	//Create another iterator to find the elements in ele_orCuts
	map<double, string>:: iterator it;

	//The results are printed on the terminal 
	cout << "\n" << "First case: an electron is produced" << "\n" << endl;

	for (int i=0; i<ele_results.size(); ++i) {

		it = ele_orCuts.find(ele_results[i]);

		cout << ele_results[i] << " -> " << it->second << endl;
	} 

	/*
	| Second case: a muon is produced. 
	| The process is identical to the previous one
	*/

	TCut first_muCut = "av_max > 300";

	TCut c12 = "mu_pt > 27"; 
	TCut c13 = "MET > 90";
	TCut second_muCut = c12 && c13;

	TCut third_muCut = "mu_pt > 24";

	TCut fourth_muCut = "HT > 1050";

	TCut fifth_muCut = "MET  > 200";

	TCut c14 = "MET > 105";
	TCut c15 = "mu_pt > 50";
	TCut sixth_muCut = c14 && c15;

	TCut c16 = "MET > 110";
	TCut c17 = "MHT > 110";
	TCut seventh_muCut = c16 && c17;

	TCut c18 = "METNoMu > 120";
	TCut c19 = "MHTNoMu > 120";
	TCut eighth_muCut = c18 && c19;

	//Fill the map 
	muTriggers["HLT_DiPFJetAve300_HFJEC_v*"] = first_muCut;
	muTriggers["HLT_IsoMu27_MET90_v*"] = second_muCut;
	muTriggers["HLT_IsoMu24_v*"] = third_muCut;
	muTriggers["HLT_PFHT1050_v*"] = fourth_muCut;
	muTriggers["HLT_PFMET200_HBHE_BeamHaloCleaned_v*"] = fifth_muCut;
	muTriggers["HLT_MET105_IsoTrk50_v*"] = sixth_muCut;
	muTriggers["HLT_PFMET110_PFMHT110_IDTight_CaloBTagDeepCSV_3p1_v*"] = seventh_muCut;
	muTriggers["HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_v*"] = eighth_muCut;

	//Loop over the triggers.. 
	for (first_it = muTriggers.begin(); first_it != muTriggers.end(); ++first_it ) {

		for (second_it = next(first_it); second_it != muTriggers.end(); ++second_it) {

			events = tMu->GetEntries();
			survivors = tMu->GetEntries(first_it->second || second_it->second);

			efficiency = survivors / events;

			mu_results.push_back(efficiency);

			TString triggers = (first_it->first);
				triggers += (" || ");
				triggers += (second_it->first); 

			mu_orCuts[efficiency] = triggers;
		}
	}

	sort(mu_results.begin(), mu_results.end());	

	//.. and print the results
	cout << "\n" << "Second case: a muon is produced" << "\n" << endl;

	for (int j=0; j<mu_results.size(); ++j) {

		it = mu_orCuts.find(mu_results[j]);

		cout << mu_results[j] << " -> " << it->second << endl;
	} 	

	return 0;
}
