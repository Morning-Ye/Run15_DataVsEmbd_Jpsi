const int nVar = 8;
const TString dataHistName[nVar] = {"Adc0", "DsmAdc", "nHitsFit", "nHitsDedxFit", "Dca", "EvP", "phiDist", "zDist"};
const TString embdHistName[nVar] = {"Adc0", "DsmAdc", "nHitPts",  "nHitDedxPts",  "Dca", "EvP", "phiDist", "zDist"};
const int nPtBin = 7;
const float ptlow_partner[nPtBin]  = { 0.5, 1,  1.5, 2, 3, 5, 7  };
const float pthigh_partner[nPtBin] = { 1.0, 1.5, 2., 3, 5, 7, 10 };
const float ptlow_prime[nPtBin]  = { 2.0, 2.5, 3.0, 3.5, 4.0, 5.0, 6.0};
const float pthigh_prime[nPtBin] = { 2.5, 3.0, 3.5, 4.0, 5.0, 6.0, 10.0};


void comRun15DataVsEmbd(TString run = "", TString particle="", bool primeonly_flag = true);
//const TString run = "run15pau";
//const TString particle = "bht2_ups1s";
//const TString particle = "bht2_jpsi";
//const bool primeonly_flag = false;

void newcomRun15DataVsEmbd(){
	
	comRun15DataVsEmbd("run15pau", "bht2_jpsi", true);
	comRun15DataVsEmbd("run15pau", "bht2_jpsi", false);
	comRun15DataVsEmbd("run15pau", "bht1_jpsi", true);
	comRun15DataVsEmbd("run15pau", "bht1_jpsi", false);
	
	comRun15DataVsEmbd("run15pp", "bht2_jpsi", true);
	comRun15DataVsEmbd("run15pp", "bht2_jpsi", false);
	comRun15DataVsEmbd("run15pp", "bht1_jpsi", true);
	comRun15DataVsEmbd("run15pp", "bht1_jpsi", false);
	
}


void comRun15DataVsEmbd(TString run = "", TString particle="", bool primeonly_flag = true)
{
	const bool partonly_flag = !primeonly_flag;
	
	gROOT->Reset();
	gStyle->SetOptStat(1);
	gStyle->SetOptFit(1);
	gStyle->SetOptTitle(1);
	gStyle->SetOptStat(0);
	TGaxis::SetMaxDigits(3);
	TH1::SetDefaultSumw2();


	system("mkdir -p out_"+run+"_"+particle);

	TString date ="";
	if(run=="run15pp") date = "_0510.root";
	else date = "_0516.root";
	
	TFile *fdata = new TFile("./inputfile/qa_"+run+"_data_"+particle+date,"read");	
	TFile *fembd = new TFile("./inputfile/qa_"+run+"_embd_"+particle+date,"read");	
	cout<<"readin file: "<<fdata->GetName()<<endl;
	cout<<"readin file: "<<fembd->GetName()<<endl;

	
	//compare in each pt, eta bins	
	TH2F* HistDataPart_US[nVar];
	TH2F* HistDataPart_LS[nVar];
	TH2F* HistDataPart_SIG[nVar];
	TH2F* HistDataPrime_US[nVar];
	TH2F* HistDataPrime_LS[nVar];
	TH2F* HistDataPrime_SIG[nVar];

	TH2F* HistEmbd_SIG[nVar];
	TH1F* HistEmbd_SIG_PT[nVar][nPtBin];
	TH2F* HistEmbdTrg_SIG[nVar];
	TH1F* HistEmbdTrg_SIG_PT[nVar][nPtBin];
	TH1F* HistDataPart_SIG_PT[nVar][nPtBin];
	TH1F* HistDataPrime_SIG_PT[nVar][nPtBin];

	TCanvas *cvs[nVar];
	TPad *pad[nVar];
	TPDF *pdf[nVar];
	TLatex tl;
	tl.SetNDC();
	TString particlename = "";
	if(primeonly_flag) particlename = "_primeonly_";
	if(partonly_flag) particlename = "_partonly_";
	
	for(int i=0; i<nVar; i++){
		cvs[i] = new TCanvas(Form("c_%d",i));
		pdf[i] = new TPDF("out_"+run+"_"+particle+"/data_embd_comp_"+dataHistName[i]+particlename+".pdf");
		cvs[i]->cd();
		
		HistDataPart_US[i] = (TH2F*) fdata->Get("hPEUS_Part_"+dataHistName[i]+"vsPt");
		HistDataPart_LS[i] = (TH2F*) fdata->Get("hPELS_Part_"+dataHistName[i]+"vsPt");
		HistDataPart_SIG[i]= (TH2F*) HistDataPart_US[i]->Clone("HistDataPart_SIG_"+dataHistName[i]);
		HistDataPart_SIG[i]->Sumw2();
		HistDataPart_LS[i]->Sumw2();
		HistDataPart_SIG[i]->Add(HistDataPart_LS[i], -1);

		HistDataPrime_US[i] = (TH2F*) fdata->Get("hPEUS_Prime_"+dataHistName[i]+"vsPt");
		HistDataPrime_LS[i] = (TH2F*) fdata->Get("hPELS_Prime_"+dataHistName[i]+"vsPt");
		HistDataPrime_SIG[i]= (TH2F*) HistDataPrime_US[i]->Clone("HistDataPrime_SIG_"+dataHistName[i]);
		HistDataPrime_SIG[i]->Sumw2();
		HistDataPrime_LS[i]->Sumw2();
		HistDataPrime_SIG[i]->Add(HistDataPrime_LS[i], -1);
		tl.SetTextSize(0.04);
		tl.SetTextColor(1);
		HistEmbd_SIG[i] = (TH2F*) fembd->Get("hRcE_"+embdHistName[i]+"VsPt");
		HistEmbd_SIG[i]->SetName("HistEmbd_SIG_"+embdHistName[i]);
		HistEmbd_SIG[i]->SetTitle(embdHistName[i]);
		HistEmbdTrg_SIG[i] = (TH2F*) fembd->Get("hRcTrgE_"+embdHistName[i]+"VsPt");
		HistEmbdTrg_SIG[i]->SetName("HistEmbdTrg_SIG_"+embdHistName[i]);
		TH1F* HPT_data_tem = (TH1F*)HistDataPart_SIG[i]->ProjectionX("HPT_data_tem");
		TH1F* HPT_embd_tem = (TH1F*)HistEmbd_SIG[i]->ProjectionX("HPT_embd_tem");

		//const TString dataHistName[nVar] = {"Adc0", "DsmAdc", "nHitsFit", "nHitsDedxFit", "Dca", "EvP", "phiDist", "zDist"};
		if(i==2||i==3) {
			HistDataPart_SIG[i]->RebinY(1);
			HistDataPrime_SIG[i]->RebinY(1);
			HistEmbd_SIG[i]->RebinY(1);
			HistEmbdTrg_SIG[i]->RebinY(1);
		}
		if(i==0||i==7||i==4||i==5||i==6){
			HistDataPart_SIG[i]->RebinY(5);
			HistDataPrime_SIG[i]->RebinY(5);
			HistEmbd_SIG[i]->RebinY(5);
			HistEmbdTrg_SIG[i]->RebinY(5);
		}

		for(int j=0; j<nPtBin; j++){

			int lptbin_data_partner = HPT_data_tem->FindBin(ptlow_partner[j]);
			int hptbin_data_partner = HPT_data_tem->FindBin(pthigh_partner[j]);
			int lptbin_embd_partner = HPT_embd_tem->FindBin(ptlow_partner[j]);
			int hptbin_embd_partner = HPT_embd_tem->FindBin(pthigh_partner[j]);
			int lptbin_data_prime = HPT_data_tem->FindBin(ptlow_prime[j]);
			int hptbin_data_prime = HPT_data_tem->FindBin(pthigh_prime[j]);
			int lptbin_embd_prime = HPT_embd_tem->FindBin(ptlow_prime[j]);
			int hptbin_embd_prime = HPT_embd_tem->FindBin(pthigh_prime[j]);
			//cout<<"--------------------------"<<endl;
			
			if(primeonly_flag) {
				HistDataPrime_SIG_PT[i][j] = (TH1F*)HistDataPrime_SIG[i]->ProjectionY("HistDataPrime_SIG_PT", lptbin_data_prime, hptbin_data_prime);
				HistDataPrime_SIG_PT[i][j]->SetMarkerStyle(24);
				HistDataPrime_SIG_PT[i][j]->SetMarkerColor(1);
				HistDataPrime_SIG_PT[i][j]->SetLineColor(1);
				HistDataPrime_SIG_PT[i][j]->SetTitle(dataHistName[i]);
				HistDataPrime_SIG_PT[i][j]->Scale( 1./HistDataPrime_SIG_PT[i][j]->Integral() );
				
				int tembin = HistDataPrime_SIG_PT[i][j]->GetMaximumBin();
				double temscf = 1.6;
				if(dataHistName[i] == "phiDist" || dataHistName[i] == "zDist") temscf = 2.5;
				HistDataPrime_SIG_PT[i][j]->SetMaximum(HistDataPrime_SIG_PT[i][j]->GetBinContent(tembin)*temscf);
				HistDataPrime_SIG_PT[i][j]->SetMinimum(0.);
				HistDataPrime_SIG_PT[i][j]->Draw("pe");
				
				HistEmbdTrg_SIG_PT[i][j] = (TH1F*)HistEmbdTrg_SIG[i]->ProjectionY("HistEmbdTrg_SIG_PT", lptbin_embd_prime, hptbin_embd_prime);
				//HistEmbdTrg_SIG_PT[i][j]->Sumw2();
				HistEmbdTrg_SIG_PT[i][j]->Scale(1.*HistDataPrime_SIG_PT[i][j]->Integral()/HistEmbdTrg_SIG_PT[i][j]->Integral());
				//HistEmbdTrg_SIG_PT[i][j]->SetMarkerStyle(31);
				//HistEmbdTrg_SIG_PT[i][j]->SetMarkerColor(4);
				HistEmbdTrg_SIG_PT[i][j]->SetLineColor(4);
				HistEmbdTrg_SIG_PT[i][j]->Draw("histsame");
				tl.DrawLatex(0.62, 0.7, Form("%.1f<p_{T}<%.1f GeV/c",ptlow_prime[j], pthigh_prime[j]));
			}
			if(partonly_flag){
				HistDataPart_SIG_PT[i][j] = (TH1F*)HistDataPart_SIG[i]->ProjectionY("HistDataPart_SIG_PT", lptbin_data_partner, hptbin_data_partner);
				HistDataPart_SIG_PT[i][j]->SetTitle(dataHistName[i]);
				HistDataPart_SIG_PT[i][j]->SetMarkerStyle(24);
				HistDataPart_SIG_PT[i][j]->SetMarkerColor(1);
				HistDataPart_SIG_PT[i][j]->SetLineColor(1);
				HistDataPart_SIG_PT[i][j]->Scale( 1./HistDataPart_SIG_PT[i][j]->Integral() );

				int tembin = HistDataPart_SIG_PT[i][j]->GetMaximumBin();
				double temscf = 1.6;
				if(dataHistName[i] == "phiDist" || dataHistName[i] == "zDist") temscf = 2.5;
				HistDataPart_SIG_PT[i][j]->SetMaximum(HistDataPart_SIG_PT[i][j]->GetBinContent(tembin)*temscf);
				HistDataPart_SIG_PT[i][j]->SetMinimum(0.);
				HistDataPart_SIG_PT[i][j]->Draw("pe");

				HistEmbd_SIG_PT[i][j] = (TH1F*)HistEmbd_SIG[i]->ProjectionY("HistEmbd_SIG_PT", lptbin_embd_partner, hptbin_embd_partner);
				//HistEmbd_SIG_PT[i][j]->Sumw2();
				HistEmbd_SIG_PT[i][j]->Scale(1.*HistDataPart_SIG_PT[i][j]->Integral()/HistEmbd_SIG_PT[i][j]->Integral());
				//HistEmbd_SIG_PT[i][j]->SetMarkerStyle(24);
				//HistEmbd_SIG_PT[i][j]->SetMarkerColor(1);
				HistEmbd_SIG_PT[i][j]->SetLineColor(4);
				HistEmbd_SIG_PT[i][j]->Draw("histsame");
				tl.DrawLatex(0.62, 0.7, Form("%.1f<p_{T}<%.1f GeV/c",ptlow_partner[j], pthigh_partner[j]));
			}
			
			TLegend *leg = new TLegend(0.62, 0.75, 0.89, 0.89);
			if(primeonly_flag){
				leg->AddEntry(HistDataPrime_SIG_PT[i][j],"electron in Data", "lp");
				leg->AddEntry(HistEmbdTrg_SIG_PT[i][j], "electron in Embd", "lp");
			}
			if(partonly_flag){
				leg->AddEntry(HistDataPart_SIG_PT[i][j], "electron in Data", "lp");
				leg->AddEntry(HistEmbd_SIG_PT[i][j], "electron in Embd", "lp");
			}
			leg->Draw("same");
		
			cvs[i]->SaveAs("out_"+run+"_"+particle+"/data_embd_comp_"+dataHistName[i]+particlename+Form("pt_%d",j)+".png");

			cvs[i]->cd();
			cvs[i]->Update();
		}

		cout<<"output pdf is: "<<pdf[i]->GetName()<<endl;
		pdf[i]->Close();

	}


}
