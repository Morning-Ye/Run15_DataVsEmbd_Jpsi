
const TString run = "run15pau";
const TString particle = "bht1_jpsi";

void getPtYWT(){

	gROOT->Reset();
	gStyle->SetOptStat(1);
	gStyle->SetOptFit(1);
	gStyle->SetOptTitle(1);
	gStyle->SetOptStat(0);
	TGaxis::SetMaxDigits(3);
	TH1::SetDefaultSumw2();

	TCanvas* c1 = new TCanvas("c1");
	
	system("mkdir -p out_"+run+"_"+particle);
	TFile *fdata = new TFile("./inputfile/qa_"+run+"_data_"+particle+"_0516.root","read");	
	TFile *fembd = new TFile("./inputfile/qa_"+run+"_embd_"+particle+"_0516.root","read");	
	cout<<"readin file: "<<fdata->GetName()<<endl;
	cout<<"readin file: "<<fembd->GetName()<<endl;
	
	TH2F* hJpsiUS_YvsPt =  (TH2F*) fdata->Get("hJpsiUS_YvsPt");
	TH2F* hJpsiLS_YvsPt =  (TH2F*) fdata->Get("hJpsiLS_YvsPt");
	TH2F* hJpsiUS_YvsPt_Sig =  (TH2F*)hJpsiUS_YvsPt->Clone("hJpsiUS_YvsPt_Sig");
	hJpsiUS_YvsPt_Sig->Sumw2();
	hJpsiUS_YvsPt_Sig->Add(hJpsiLS_YvsPt,-1);
	
	TH1F* hJpsi_data_Y = (TH1F*) hJpsiUS_YvsPt_Sig->ProjectionY("hJpsi_data_Y");
	hJpsi_data_Y->SetName("hJpsi_data_Y");
	
	TH1F* hJpsi_data_Pt = (TH1F*) hJpsiUS_YvsPt_Sig->ProjectionX("hJpsi_data_Pt");
	hJpsi_data_Pt->SetName("hJpsi_data_Pt");

	//readin embding
	TH2F* hrcQM_YVsPt = (TH2F*) fembd -> Get("hrcQM_YVsPt_detc");
	TH1F* hJpsi_embd_Y = (TH1F*) hrcQM_YVsPt->ProjectionY("hJpsi_embd_Y");
	hJpsi_embd_Y->SetName("hJpsi_embd_Y");

	TH1F* hJpsi_embd_Pt = (TH1F*) hrcQM_YVsPt->ProjectionX("hJpsi_embd_Pt");
	hJpsi_embd_Pt->SetName("hJpsi_embd_Pt");
	
	hJpsi_data_Y->Rebin(8);
	hJpsi_embd_Y->Rebin(8);
	hJpsi_data_Y->SetTitle("J/#psi y data vs embd");
	hJpsi_data_Y->SetMarkerStyle(20);
	hJpsi_data_Y->Draw("pe");
	hJpsi_embd_Y->Scale(hJpsi_data_Y->Integral()/hJpsi_embd_Y->Integral());
	hJpsi_embd_Y->SetMarkerStyle(20);
	hJpsi_embd_Y->SetMarkerColor(4);
	hJpsi_embd_Y->SetLineColor(4);
	hJpsi_embd_Y->Draw("pesame");
	TLegend *legy = new TLegend(0.6, 0.75, 0.88, 0.88);
	legy->AddEntry(hJpsi_embd_Y, "J/#psi in Embd bfwt", "lp");
	legy->AddEntry(hJpsi_data_Y, "J/#psi in Data bfwt", "lp");
	legy->Draw("same");

	c1->SaveAs("out_"+run+"_"+particle+"/jpsi_Y_data_embd_bfwt.gif");

	TH1F* Hratio_Y = (TH1F*) hJpsi_data_Y->Clone("Hratio_Y");
	Hratio_Y->Divide(hJpsi_embd_Y);
	Hratio_Y->SetTitle("J/#psi y ratio data/embd ");
	Hratio_Y->Draw("pe");

	c1->SaveAs("out_"+run+"_"+particle+"/jpsi_Y_ratio.gif");

	hJpsi_data_Pt->Rebin(4);
	hJpsi_embd_Pt->Rebin(4);
	hJpsi_data_Pt->SetTitle("J/#psi Pt data vs embd");
	hJpsi_data_Pt->SetMarkerStyle(20);
	hJpsi_data_Pt->Draw("pe");
	hJpsi_embd_Pt->Scale(hJpsi_data_Pt->Integral()/hJpsi_embd_Pt->Integral());
	hJpsi_embd_Pt->SetMarkerStyle(20);
	hJpsi_embd_Pt->SetMarkerColor(4);
	hJpsi_embd_Pt->SetLineColor(4);
	hJpsi_embd_Pt->Draw("pesame");
	TLegend *legpt = new TLegend(0.6, 0.75, 0.88, 0.88);
	legpt->AddEntry(hJpsi_embd_Pt, "J/#psi in Embd bfwt", "lp");
	legpt->AddEntry(hJpsi_data_Pt, "J/#psi in Data bfwt", "lp");
	legpt->Draw("same");

	c1->SaveAs("out_"+run+"_"+particle+"/jpsi_Pt_data_embd_bfwt.gif");

	TH1F* Hratio_Pt = (TH1F*) hJpsi_data_Pt->Clone("Hratio_Pt");
	Hratio_Pt->Divide(hJpsi_embd_Pt);
	Hratio_Pt->SetTitle("J/#psi Pt ratio data/embd ");
	Hratio_Pt->Draw("pe");

	c1->SaveAs("out_"+run+"_"+particle+"/jpsi_Pt_ratio.gif");

	TH2F* HrcQM_YVsPhi = (TH2F*) fembd -> Get("hrcQM_YVsPhi_detc");
	TH1F* HJpsi_embd_Phi = (TH1F*) HrcQM_YVsPhi->ProjectionX("HJpsi_embd_Phi");
	HJpsi_embd_Phi->SetName("HJpsi_embd_Phi");


	//compare the Jpsi pt,y,phi data vs the embd after reweight
	cout<<"================================================================"<<endl;
	cout<<"compare the Jpsi pt,y,phi data vs the embd after reweight "<<endl;
	cout<<"now readin the Jpsi in embding after the reweight "<<endl;
	
	TH2F* HJpsi_embd_YPt_wt = (TH2F*)fembd->Get("hrcQM_YVsPt_detc_wt");
	TH1F* HJpsi_embd_Pt_wt = (TH1F*) HJpsi_embd_YPt_wt->ProjectionX("HJpsi_embd_Pt_wt");
	TH1F* HJpsi_embd_Y_wt = (TH1F*) HJpsi_embd_YPt_wt->ProjectionY("HJpsi_embd_Y_wt");
	
	TH2F* HJpsi_embd_YPhi_wt = (TH2F*)fembd->Get("hrcQM_YVsPhi_detc_wt");
	TH1F* HJpsi_embd_Phi_wt = (TH1F*) HJpsi_embd_YPhi_wt->ProjectionX("HJpsi_embd_Phi_wt");

	hJpsi_data_Pt->Draw("pe");
	HJpsi_embd_Pt_wt->Rebin(4);
	HJpsi_embd_Pt_wt->Scale(hJpsi_data_Pt->Integral()/HJpsi_embd_Pt_wt->Integral());
	HJpsi_embd_Pt_wt->SetMarkerStyle(20);
	HJpsi_embd_Pt_wt->SetMarkerColor(2);
	HJpsi_embd_Pt_wt->SetLineColor(2);
	HJpsi_embd_Pt_wt->Draw("pesame");
	TLegend *legptwt = new TLegend(0.6, 0.75, 0.88, 0.88);
	legptwt->AddEntry(HJpsi_embd_Pt_wt, "J/#psi in Embd afwt", "lp");
	legptwt->AddEntry(hJpsi_data_Pt, "J/#psi in Data", "lp");
	legptwt->Draw("same");

	c1->SaveAs("out_"+run+"_"+particle+"/Jpsi_Pt_datavsembd_afwt.gif");
	
	hJpsi_data_Y->Draw("pe");
	HJpsi_embd_Y_wt->Rebin(8);
	HJpsi_embd_Y_wt->Scale(hJpsi_data_Y->Integral()/HJpsi_embd_Y_wt->Integral());
	HJpsi_embd_Y_wt->SetMarkerStyle(20);
	HJpsi_embd_Y_wt->SetMarkerColor(2);
	HJpsi_embd_Y_wt->SetLineColor(2);
	HJpsi_embd_Y_wt->Draw("pesame");

	TLegend *legywt = new TLegend(0.6, 0.75, 0.88, 0.88);
	legywt->AddEntry(HJpsi_embd_Y_wt, "J/#psi in Embd afwt", "lp");
	legywt->AddEntry(hJpsi_data_Y, "J/#psi in Data", "lp");
	legywt->Draw("same");
	c1->SaveAs("out_"+run+"_"+particle+"/Jpsi_Y_datavsembd_afwt.gif");

	TH2F* hJpsiUS_YvsPhi =  (TH2F*) fdata->Get("hJpsiUS_YvsPhi");
	TH2F* hJpsiLS_YvsPhi =  (TH2F*) fdata->Get("hJpsiLS_YvsPhi");
	TH2F* hJpsiUS_YvsPhi_Sig =  (TH2F*)hJpsiUS_YvsPhi->Clone("hJpsiUS_YvsPhi_Sig");
	hJpsiUS_YvsPhi_Sig->Sumw2();
	hJpsiUS_YvsPhi_Sig->Add(hJpsiLS_YvsPhi,-1);
	TH1F* hJpsi_data_Phi = (TH1F*) hJpsiUS_YvsPhi_Sig->ProjectionX("hJpsi_data_Phi");
	hJpsi_data_Phi->SetName("hJpsi_data_Phi");

	hJpsi_data_Phi->SetMarkerStyle(20);
	hJpsi_data_Phi->Rebin(2);
	HJpsi_embd_Phi_wt->Rebin(2);
	hJpsi_data_Phi->SetMaximum(hJpsi_data_Phi->GetMaximum()*2.8);
	hJpsi_data_Phi->SetMinimum(hJpsi_data_Phi->GetMinimum()*0.2);
	hJpsi_data_Phi->Draw("pe");
	//	HJpsi_embd_Phi_wt->Rebin(8);
	HJpsi_embd_Phi_wt->Scale(hJpsi_data_Phi->Integral()/HJpsi_embd_Phi_wt->Integral());
	HJpsi_embd_Phi_wt->SetMarkerStyle(20);
	HJpsi_embd_Phi_wt->SetMarkerColor(2);
	HJpsi_embd_Phi_wt->SetLineColor(2);
	HJpsi_embd_Phi_wt->Draw("pesame");

	TLegend *legphiwt = new TLegend(0.6, 0.75, 0.88, 0.88);
	legphiwt->AddEntry(HJpsi_embd_Phi_wt, "J/#psi in Embd afwt", "lp");
	legphiwt->AddEntry(hJpsi_data_Phi, "J/#psi in Data", "lp");
	legphiwt->Draw("same");
	c1->SaveAs("out_"+run+"_"+particle+"/Jpsi_Phi_datavsembd_afwt.gif");

	HJpsi_embd_Phi->Rebin(2);
	hJpsi_data_Phi->Draw("pe");
	//	HJpsi_embd_Phi->Rebin(8);
	HJpsi_embd_Phi->Scale(hJpsi_data_Phi->Integral()/HJpsi_embd_Phi->Integral());
	HJpsi_embd_Phi->SetMarkerStyle(20);
	HJpsi_embd_Phi->SetMarkerColor(2);
	HJpsi_embd_Phi->SetLineColor(2);
	HJpsi_embd_Phi->Draw("pesame");

	TLegend *legphi = new TLegend(0.6, 0.75, 0.88, 0.88);
	legphi->AddEntry(HJpsi_embd_Phi, "J/#psi in Embd bfwt", "lp");
	legphi->AddEntry(hJpsi_data_Phi, "J/#psi in Data", "lp");
	legphi->Draw("same");
	c1->SaveAs("out_"+run+"_"+particle+"/Jpsi_Phi_datavsembd_bfwt.gif");


	//------------------------------------------------------------------------------	
	TFile *outf = new TFile("out_"+run+"_"+particle+"/"+run+"_"+particle+"_out_Jpsi_PtY_wt.root", "recreate");
	outf->cd();
	hJpsi_data_Y->Write();
	hJpsi_data_Pt->Write();
	hJpsi_embd_Y->Write();
	hJpsi_embd_Pt->Write();
	Hratio_Pt->Write();
	Hratio_Y->Write();
	cout<<"outputfile: "<<outf->GetName()<<endl;
	outf->Close();
}
