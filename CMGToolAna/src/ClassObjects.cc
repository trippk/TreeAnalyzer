#include "../interface/ClassObjects.h"

using namespace std;

/*
// global Objects
vector<Jet> goodJet;
vector<Jet> goodBJet;

TLorentzVector MET;
TLorentzVector genMET;
TLorentzVector METnoPU;

vector<Lepton> goodLep;
vector<Lepton> goodEl;
vector<Lepton> goodMu;

vector<Lepton> vetoLep;
vector<Lepton> vetoEl;
vector<Lepton> vetoMu;

vector<GenLepton> genLep;
vector<GenLepton> genEl;
vector<GenLepton> genMu;
vector<GenLepton> genTau;

vector<GenLepton> genLepFromTau;
vector<GenLepton> genElFromTau;
vector<GenLepton> genMuFromTau;
*/

///////////////////////////////////
///////////////////////////////////
///////////////////////////////////
// Object cuts
Float_t goodEta = 2.4;

//leptons
Float_t goodElPt = 20.0;
Float_t goodMuPt = 20.0;
Float_t goodLepPt = 20.0;
Float_t vetoLepPt = 10.0;

Float_t goodEl_relIso03 = 0.14;
Float_t goodMu_relIso03 = 0.12;
Float_t goodLep_relIso03 = 0.15;

Bool_t goodMu_tightID = true;
Bool_t goodEl_tightID = true;

//jets
Float_t goodJetPt = 40.0;
Float_t goodJetBtagCSV = 0.679;

///////////////////////////////////
///////////////////////////////////
///////////////////////////////////

// variables for fTree
const int arrayN = 50;
Float_t Jet_pt[arrayN];
Float_t Jet_eta[arrayN];
Float_t Jet_phi[arrayN];
Float_t Jet_mass[arrayN];
Float_t Jet_btagCSV[arrayN];

Float_t LepGood_pt[arrayN];
Float_t LepGood_eta[arrayN];
Float_t LepGood_phi[arrayN];
Float_t LepGood_mass[arrayN];
Float_t LepGood_relIso03[arrayN];
Int_t   LepGood_pdgId[arrayN];
Bool_t  LepGood_tightID[arrayN];

// Gen particles
Float_t genLep_pt[2]; //[ngenLep]
Float_t genLep_mass[2]; //[ngenLep]
Float_t genLep_eta[2]; //[ngenLep]
Float_t genLep_phi[2]; //[ngenLep]
Int_t genLep_pdgId[2]; //[ngenLep]
//Float_t genLep_charge[2]; //[ngenLep]

// MET
Float_t met_eta;
Float_t met_phi;
Float_t met_pt;
Float_t met_mass;

void ObjectChain::GetLeptons(vector<Lepton> *leptons, vector<Lepton> *electrons, vector<Lepton> *muons, vector<Lepton> *vetolep){

    // clearing objects
    leptons->clear();
    vetolep->clear();
    electrons->clear();
    muons->clear();

//    vector<Lepton> * vetoLep;

    int nLepGood = 0;
    int nMuGood = 0;
    int nElGood = 0;

    int nLepVeto = 0;
    int nElVeto = 0;
    int nMuVeto = 0;

    // filling objects from fTree
    int nLep = Get(nLep,"nLepGood");
    Get(LepGood_pt[0],"LepGood_pt");
    Get(LepGood_eta[0],"LepGood_eta");
    Get(LepGood_phi[0],"LepGood_phi");
    Get(LepGood_mass[0],"LepGood_mass");
    Get(LepGood_relIso03[0],"LepGood_relIso03");
    Get(LepGood_pdgId[0],"LepGood_pdgId");
    Get(LepGood_tightID[0],"LepGood_tightId");

    for(int ilep = 0; ilep < nLep; ilep++){

	Lepton dummyLep;
	dummyLep.SetPtEtaPhiM(LepGood_pt[ilep],LepGood_eta[ilep],LepGood_phi[ilep],LepGood_mass[ilep]);
	dummyLep.pdgID = LepGood_pdgId[ilep];
	dummyLep.tightID = LepGood_tightID[ilep];
	dummyLep.relIso03 = LepGood_relIso03[ilep];

	bool isVetoMu = false;
	bool isVetoEl = false;

	// common cuts for all leptons (good and veto leps pass)
	if(dummyLep.Pt() <= vetoLepPt || fabs(dummyLep.Eta()) > goodEta)
	    continue;
	// Muon cuts
	if(abs(LepGood_pdgId[ilep]) == 13){
	    if( dummyLep.Pt() > goodMuPt && LepGood_tightID[ilep] && LepGood_relIso03[ilep] < goodMu_relIso03){
			leptons->push_back(dummyLep);
			muons->push_back(dummyLep);
			nMuGood++;
			nLepGood++;

			//continue;
		    }
		    else{
			isVetoMu = true;
			nMuVeto++;
		    }
		}

	// Electron cuts
	if(abs(LepGood_pdgId[ilep]) == 11){
	    if( dummyLep.Pt() > goodElPt && LepGood_tightID[ilep] && LepGood_relIso03[ilep] < goodEl_relIso03){
//                    isGoodEl = true;
			leptons->push_back(dummyLep);
			electrons->push_back(dummyLep);
			nElGood++;
			nLepGood++;

		       // continue;
		    }
		    else{
			isVetoEl = true;
			nElVeto++;
		    }

	    }

	// Only non-good El or Mu will pass => veto leptons
	if(isVetoEl || isVetoMu){
	    vetolep->push_back(dummyLep);
	    nLepVeto++;
	}
    }

    if(debug_){
	cout << "Get leptons summary: total number of good Leptons = \t" << nLepGood << endl;
	cout << "Number of good Muons = \t" << nMuGood << " and veto Mu = \t" << nMuVeto << endl;
	cout << "Number of good Electrons = \t" << nElGood  << " and veto El = \t" << nElVeto << endl;
	cout << "Number of veto leptons = \t" << nLepVeto << endl;
    }
}

/*

void ObjectChain::GetGenLeptons(vector<GenLepton> lepton, vector<GenLepton> electron, vector<GenLepton> muon){

    // clearing objects
    genLep.clear();
    nGenLep = 0;

    // filling objects from this
    Get(nGenLep,"ngenLep"); //n prompt Lep
    Get(genLep_pt[0],"genLep_pt");
    Get(genLep_mass[0],"genLep_mass");
    Get(genLep_eta[0],"genLep_eta");
    Get(genLep_phi[0],"genLep_phi");
    Get(genLep_pdgId[0],"genLep_pdgId");

    for(int ilep = 0; ilep < nGenLep; ilep++){

	GenLepton dummyLep;
	dummyLep.SetPtEtaPhiM(genLep_pt[ilep],genLep_eta[ilep],genLep_phi[ilep],genLep_mass[ilep]);

	genLep.push_back(dummyLep);
//      nGenLep++;

	// Muon cuts
	if(abs(genLep_pdgId[ilep]) == 13){

	    genMu.push_back(dummyLep);
//            nGenMu++;

	    continue;
	}

	// Electron cuts
	if(abs(genLep_pdgId[ilep]) == 11){

	    genEl.push_back(dummyLep);
//            nGenEl++;

	    continue;
	}
    }
}


void ObjectChain::GetGenLeptonsFromTau(vector<GenLepton> lepton, vector<GenLepton> electron, vector<GenLepton> muon){

    // clearing objects
    genLepFromTau.clear();
    genMuFromTau.clear();
    genElFromTau.clear();

    nGenLepFromTau = 0;

    // filling objects from this
    Get(nGenLepFromTau,"ngenLepFromTau");// Lep from Tau decay

    Get(genLep_pt[0],"genLepFromTau_pt");
    Get(genLep_mass[0],"genLepFromTau_mass");
    Get(genLep_eta[0],"genLepFromTau_eta");
    Get(genLep_phi[0],"genLepFromTau_phi");
    Get(genLep_pdgId[0],"genLepFromTau_pdgId");

    for(int ilep = 0; ilep < nGenLepFromTau; ilep++){

	GenLepton dummyLep;
	dummyLep.SetPtEtaPhiM(genLep_pt[ilep],genLep_eta[ilep],genLep_phi[ilep],genLep_mass[ilep]);

	genLepFromTau.push_back(dummyLep);
//      nGenLepFromTau++;

// TO BE enhanced

	if(abs(genLep_pdgId[ilep]) == 13){
	    genMuFromTau.push_back(dummyLep);
//            nGenMu++;
	    continue;
	}

	if(abs(genLep_pdgId[ilep]) == 11){
	    genElFromTau.push_back(dummyLep);
//            nGenEl++;
	    continue;
	}
    }
}

void ObjectChain::GetGenTaus(vector<GenLepton> lepton){

    // clearing objects
    genTau.clear();
    nGenTau = 0;

    // filling objects from this
    Get(nGenTau,"ngenTau");// gen Tau
    Get(genLep_pt[0],"genTau_pt");
    Get(genLep_eta[0],"genTau_eta");
    Get(genLep_phi[0],"genTau_phi");
    Get(genLep_pdgId[0],"genTau_pdgId");
    Get(genLep_mass[0],"genTau_mass");

    for(int ilep = 0; ilep < nGenTau; ilep++){

	GenLepton dummyLep;
	dummyLep.SetPtEtaPhiM(genLep_pt[ilep],genLep_eta[ilep],genLep_phi[ilep],genLep_mass[ilep]);

	if(abs(genLep_pdgId[ilep]) == 15){
	    genTau.push_back(dummyLep);
//            nGenTau++;
	}
    }
}
*/

void ObjectChain::GetJets(vector<Jet> * jets, vector<Jet> * bjets){

    jets->clear();
    bjets->clear();

    double ST = 0;
    double HT40 = 0;

    double nJetGood = 0;
    double nBJetGood = 0;

    int nJet = Get(nJet,"nJet");
    Get(Jet_pt[0],"Jet_pt");
    Get(Jet_eta[0],"Jet_eta");
    Get(Jet_phi[0],"Jet_phi");
    Get(Jet_mass[0],"Jet_mass");
    Get(Jet_btagCSV[0],"Jet_btagCSV");

    for(int ijet = 0; ijet < nJet; ijet++)
    {
	Jet dummyJet;
	dummyJet.SetPtEtaPhiM(Jet_pt[ijet],Jet_eta[ijet],Jet_phi[ijet],Jet_mass[ijet]);
	//put pt, eta, cuts and other stuff
	//jets are already cleaned from all loose leptons that are in LepGood
	if(dummyJet.Pt() > goodJetPt && fabs(dummyJet.Eta()) < goodEta){
	    jets->push_back(dummyJet);
	    nJetGood++;
	    HT40 = HT40 + dummyJet.Pt();

	    // filling B jets
	    if(Jet_btagCSV[ijet] > goodJetBtagCSV){
		bjets->push_back(dummyJet);
		nBJetGood++;
	    }
	}
    }

    if(debug_){
	cout << "Get jets summary: total number of jets = \t" << nJet << endl;
	cout << "Number of good jets = \t" << nJetGood  << " and b jets = \t" << nBJetGood << endl;
	cout << "Number of good jets = \t" << jets->size()  << " and b jets = \t" << bjets->size() << endl;
    }
}

void ObjectChain::GetMET(TLorentzVector & MET){
    MET.SetPtEtaPhiM(0,0,0,0);

    Get(met_pt,"met_pt");
    Get(met_eta,"met_eta");
    Get(met_phi,"met_phi");
    Get(met_mass,"met_mass");

    MET.SetPtEtaPhiM(met_pt,met_eta,met_phi,met_mass);
}

void ObjectChain::GetGenMET(TLorentzVector & genMET){
    genMET.SetPtEtaPhiM(0,0,0,0);

    Get(met_pt,"met_genPt");
    Get(met_eta,"met_genEta");
    Get(met_phi,"met_genPhi");
    Get(met_mass,"met_sumEt");

    genMET.SetPtEtaPhiM(met_pt,met_eta,met_phi,met_mass);
}

void ObjectChain::GetMETnoPU(TLorentzVector & METnoPU){
    METnoPU.SetPtEtaPhiM(0,0,0,0);

    Get(met_pt,"metNoPU_pt");
    Get(met_eta,"metNoPU_eta");
    Get(met_phi,"metNoPU_phi");
    Get(met_mass,"metNoPU_mass");

    METnoPU.SetPtEtaPhiM(met_pt,met_eta,met_phi,met_mass);
}