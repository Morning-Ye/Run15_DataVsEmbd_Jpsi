#include "/Users/zaochenye/zaochenUtill/zaochenfunc.h"

//const TString run = "run15pau";
const TString run = "run15pp";
const TString particle = "bht2_jpsi";

void mkEventQA()
{
	gROOT->Reset();
	gStyle->SetOptStat(1);
	gStyle->SetOptFit(1);
	gStyle->SetOptTitle(1);
	gStyle->SetOptStat(0);
	TGaxis::SetMaxDigits(3);
	TH1::SetDefaultSumw2();
	TCanvas *c1 = new TCanvas("c1");

	system("mkdir -p out_"+run+"_"+particle);

	TString date ="";
	if(run=="run15pp") date = "_0510.root";
	else date = "_0516.root";
	
	TFile *fdata = new TFile("./inputfile/qa_"+run+"_data_"+particle+date,"read");	
	TFile *fembd = new TFile("./inputfile/qa_"+run+"_embd_"+particle+date,"read");	
	cout<<"readin file: "<<fdata->GetName()<<endl;
	cout<<"readin file: "<<fembd->GetName()<<endl;

	//Vertext
	TH1F* H_Vz_Data = (TH1F*) fdata->Get("hTpcVz");
	H_Vz_Data -> Rebin(5);
	TH1F* H_Vz_Embd = (TH1F*) fembd->Get("hRcVz");
	H_Vz_Embd -> Rebin(5);

	cout<<"data vz width: "<<H_Vz_Data->GetBinWidth(1)<<endl;
	cout<<"embd vz width: "<<H_Vz_Embd->GetBinWidth(1)<<endl;

	H_Vz_Data->SetMarkerStyle(24);
	H_Vz_Data->SetMarkerColor(2);
	H_Vz_Data->SetTitle("VertexZ");
	H_Vz_Data->SetXTitle("VertexZ (cm)");
	H_Vz_Embd->Scale(1.*H_Vz_Data->GetMaximum()/H_Vz_Embd->GetMaximum());
	H_Vz_Data->SetMaximum(H_Vz_Data->GetMaximum()*1.5);
	H_Vz_Data->SetAxisRange(-150, 150, "x");
	H_Vz_Embd->SetMarkerColor(4);
	//H_Vz_Embd->Sumw2(0);
	H_Vz_Data->Draw("pe");
	H_Vz_Embd->Draw("histsame");

	TLatex tl;
	tl.SetNDC();
	tl.DrawLatex(0.15, 0.80, run);

	TLegend *leg = new TLegend(0.6, 0.75, 0.88, 0.88);
	leg->AddEntry(H_Vz_Data, "Vz in Data", "lp");
	leg->AddEntry(H_Vz_Embd, "Vz in Embd", "lp");
	leg->Draw("same");

	c1->SaveAs("out_"+run+"_"+particle+"/vz.gif");

	c1->SetLogz();
	TH2F* H_Vxy_Data = (TH2F*) fdata->Get("hTpcVxy");
	H_Vxy_Data->SetTitle("Data Vy vs Vx");
	H_Vxy_Data->Draw("colz");
	c1->SaveAs("out_"+run+"_"+particle+"/vxy_data.gif");

	TH2F* H_Vxy_Embd = (TH2F*) fembd->Get("hRcVxy");
	H_Vxy_Embd->SetTitle("Embd Vy vs Vx");
	H_Vxy_Embd->Draw("colz");
	c1->SaveAs("out_"+run+"_"+particle+"/vxy_embd.gif");

	//make single electron Pt, Eta, Phi
	TH2F* H_Data_Prime_EtaPhi_US = (TH2F*) fdata->Get("hPEUS_Prime_EtavsPhi");
	TH2F* H_Data_Prime_EtaPhi_LS = (TH2F*) fdata->Get("hPELS_Prime_EtavsPhi");
	TH2F* H_Data_Prime_EtaPhi_Sig = (TH2F*)H_Data_Prime_EtaPhi_US->Clone("H_Data_Prime_EtaPhi_Sig");
	H_Data_Prime_EtaPhi_Sig->Add(H_Data_Prime_EtaPhi_LS, -1);
	TH1F* H_Data_Prime_Eta_Sig = (TH1F*) H_Data_Prime_EtaPhi_Sig->ProjectionY("H_Data_Prime_Eta_Sig");
	TH1F* H_Data_Prime_Phi_Sig = (TH1F*) H_Data_Prime_EtaPhi_Sig->ProjectionX("H_Data_Prime_Phi_Sig");

	TH2F* H_Data_Prime_DcaPt_US = (TH2F*) fdata->Get("hPEUS_Prime_DcavsPt");
	TH2F* H_Data_Prime_DcaPt_LS = (TH2F*) fdata->Get("hPELS_Prime_DcavsPt");
	TH2F* H_Data_Prime_DcaPt_Sig = (TH2F*)H_Data_Prime_DcaPt_US->Clone("H_Data_Prime_DcaPt_Sig");
	H_Data_Prime_DcaPt_Sig->Add(H_Data_Prime_DcaPt_LS, -1);
	TH1F* H_Data_Prime_Pt_Sig = (TH1F*) H_Data_Prime_DcaPt_Sig->ProjectionX("H_Data_Prime_Pt_Sig");

	TH2F* H_Embd_Prime_EtaPhi_Sig = (TH2F*)fembd->Get("hRcTrgE_EtaPhi");
	TH1F* H_Embd_Prime_Eta_Sig = (TH1F*) H_Embd_Prime_EtaPhi_Sig->ProjectionY("H_Embd_Prime_Eta_Sig");
	TH1F* H_Embd_Prime_Phi_Sig = (TH1F*) H_Embd_Prime_EtaPhi_Sig->ProjectionX("H_Embd_Prime_Phi_Sig");

	TH2F* H_Embd_Prime_DcaPt_Sig = (TH2F*)fembd->Get("hRcTrgE_DcaVsPt");
	TH1F* H_Embd_Prime_Pt_Sig = (TH1F*) H_Embd_Prime_DcaPt_Sig->ProjectionX("H_Embd_Prime_Pt_Sig");

	//draw prime eta
	H_Data_Prime_Eta_Sig->Rebin(8);
	H_Data_Prime_Eta_Sig->SetTitle("Trg e Eta");
	H_Data_Prime_Eta_Sig->SetMarkerStyle(20);
	H_Data_Prime_Eta_Sig->Draw("pe");
	H_Embd_Prime_Eta_Sig->Rebin(8);
	H_Embd_Prime_Eta_Sig->Scale(H_Data_Prime_Eta_Sig->Integral()/H_Embd_Prime_Eta_Sig->Integral());
	H_Embd_Prime_Eta_Sig->SetMarkerStyle(20);
	H_Embd_Prime_Eta_Sig->SetMarkerColor(2);
	H_Embd_Prime_Eta_Sig->Draw("pesame");

	TLegend *legy = new TLegend(0.6, 0.75, 0.88, 0.88);
	legy->AddEntry(H_Data_Prime_Eta_Sig, "Trg e in Data", "lp");
	legy->AddEntry(H_Embd_Prime_Eta_Sig, "Trg e in Embd", "lp");
	legy->Draw("same");

	c1->SaveAs("out_"+run+"_"+particle+"/Prime_Eta_datavsembd.gif");

	//Get Eta ratio
	TH1F* Hratio_prime_Eta = (TH1F*)H_Data_Prime_Eta_Sig->Clone("Hratio_prime_Eta");
	Hratio_prime_Eta->Rebin();
	H_Embd_Prime_Eta_Sig->Rebin();
	Hratio_prime_Eta->Divide(H_Embd_Prime_Eta_Sig);
	Hratio_prime_Eta->Draw("pe");
	c1->SaveAs("out_"+run+"_"+particle+"/Prime_Eta_ratio_afterwt.gif");

	//draw prime Phi
	H_Data_Prime_Phi_Sig->Rebin(8);
	H_Data_Prime_Phi_Sig->SetTitle("Trg e Phi");
	H_Data_Prime_Phi_Sig->SetMarkerStyle(20);
	H_Data_Prime_Phi_Sig->SetMinimum(0);
	H_Data_Prime_Phi_Sig->SetMaximum(H_Data_Prime_Phi_Sig->GetMaximum()*2.);
	H_Data_Prime_Phi_Sig->Draw("pe");
	H_Embd_Prime_Phi_Sig->Rebin(8);
	H_Embd_Prime_Phi_Sig->Scale(H_Data_Prime_Phi_Sig->Integral()/H_Embd_Prime_Phi_Sig->Integral());
	H_Embd_Prime_Phi_Sig->SetMarkerStyle(20);
	H_Embd_Prime_Phi_Sig->SetMarkerColor(2);
	H_Embd_Prime_Phi_Sig->Draw("pesame");

	TLegend *legphi = new TLegend(0.6, 0.75, 0.88, 0.88);
	legphi->AddEntry(H_Data_Prime_Phi_Sig, "Trg e in Data", "lp");
	legphi->AddEntry(H_Embd_Prime_Phi_Sig, "Trg e in Embd", "lp");
	legphi->Draw("same");

	c1->SaveAs("out_"+run+"_"+particle+"/Prime_Phi_datavsembd.gif");

	//draw prime Pt
	H_Data_Prime_Pt_Sig->SetTitle("Trg e Pt");
	H_Data_Prime_Pt_Sig->SetMarkerStyle(20);
	H_Data_Prime_Pt_Sig->Draw("pe");
	H_Embd_Prime_Pt_Sig->Scale(H_Data_Prime_Pt_Sig->Integral()/H_Embd_Prime_Pt_Sig->Integral());
	H_Embd_Prime_Pt_Sig->SetMarkerStyle(20);
	H_Embd_Prime_Pt_Sig->SetMarkerColor(2);
	H_Embd_Prime_Pt_Sig->Draw("pesame");

	TLegend *legpt = new TLegend(0.6, 0.75, 0.88, 0.88);
	legpt->AddEntry(H_Data_Prime_Pt_Sig, "Trg e in Data", "lp");
	legpt->AddEntry(H_Embd_Prime_Pt_Sig, "Trg e in Embd", "lp");
	legpt->Draw("same");

	c1->SaveAs("out_"+run+"_"+particle+"/Prime_Pt_datavsembd.gif");

	//Get Pt ratio
	TH1F* Hratio_prime_Pt = (TH1F*)H_Data_Prime_Pt_Sig->Clone("Hratio_prime_Pt");
	Hratio_prime_Pt->Rebin();
	H_Embd_Prime_Pt_Sig->Rebin();
	Hratio_prime_Pt->Divide(H_Embd_Prime_Pt_Sig);
	Hratio_prime_Pt->SetMaximum(Hratio_prime_Pt->GetMaximum()*1.5);
	Hratio_prime_Pt->SetMinimum(0);
	Hratio_prime_Pt->Draw("pe");
	c1->SaveAs("out_"+run+"_"+particle+"/Prime_Pt_ratio_afterwt.gif");


	//draw for partner

	//make single electron Pt, Eta, Phi
	TH2F* H_Data_Part_EtaPhi_US = (TH2F*) fdata->Get("hPEUS_Part_EtavsPhi");
	TH2F* H_Data_Part_EtaPhi_LS = (TH2F*) fdata->Get("hPELS_Part_EtavsPhi");
	TH2F* H_Data_Part_EtaPhi_Sig = (TH2F*)H_Data_Part_EtaPhi_US->Clone("H_Data_Part_EtaPhi_Sig");
	H_Data_Part_EtaPhi_Sig->Add(H_Data_Part_EtaPhi_LS, -1);
	TH1F* H_Data_Part_Eta_Sig = (TH1F*) H_Data_Part_EtaPhi_Sig->ProjectionY("H_Data_Part_Eta_Sig");
	TH1F* H_Data_Part_Phi_Sig = (TH1F*) H_Data_Part_EtaPhi_Sig->ProjectionX("H_Data_Part_Phi_Sig");

	TH2F* H_Data_Part_DcaPt_US = (TH2F*) fdata->Get("hPEUS_Part_DcavsPt");
	TH2F* H_Data_Part_DcaPt_LS = (TH2F*) fdata->Get("hPELS_Part_DcavsPt");
	TH2F* H_Data_Part_DcaPt_Sig = (TH2F*)H_Data_Part_DcaPt_US->Clone("H_Data_Part_DcaPt_Sig");
	H_Data_Part_DcaPt_Sig->Add(H_Data_Part_DcaPt_LS, -1);
	TH1F* H_Data_Part_Pt_Sig = (TH1F*) H_Data_Part_DcaPt_Sig->ProjectionX("H_Data_Part_Pt_Sig");

	TH2F* H_Embd_Part_EtaPhi_Sig = (TH2F*)fembd->Get("hRcE_EtaPhi");
	TH1F* H_Embd_Part_Eta_Sig = (TH1F*) H_Embd_Part_EtaPhi_Sig->ProjectionY("H_Embd_Part_Eta_Sig");
	TH1F* H_Embd_Part_Phi_Sig = (TH1F*) H_Embd_Part_EtaPhi_Sig->ProjectionX("H_Embd_Part_Phi_Sig");

	TH2F* H_Embd_Part_DcaPt_Sig = (TH2F*)fembd->Get("hRcE_DcaVsPt");
	TH1F* H_Embd_Part_Pt_Sig = (TH1F*) H_Embd_Part_DcaPt_Sig->ProjectionX("H_Embd_Part_Pt_Sig");

	//draw prime eta
	H_Data_Part_Eta_Sig->Rebin(8);
	H_Data_Part_Eta_Sig->SetTitle("Part e Eta");
	H_Data_Part_Eta_Sig->SetMarkerStyle(20);
	H_Data_Part_Eta_Sig->Draw("pe");
	H_Embd_Part_Eta_Sig->Rebin(8);
	H_Embd_Part_Eta_Sig->Scale(H_Data_Part_Eta_Sig->Integral()/H_Embd_Part_Eta_Sig->Integral());
	H_Embd_Part_Eta_Sig->SetMarkerStyle(20);
	H_Embd_Part_Eta_Sig->SetMarkerColor(2);
	H_Embd_Part_Eta_Sig->Draw("pesame");

	TLegend *leg2y = new TLegend(0.6, 0.75, 0.88, 0.88);
	leg2y->AddEntry(H_Data_Part_Eta_Sig, "Part e in Data", "lp");
	leg2y->AddEntry(H_Embd_Part_Eta_Sig, "Part e in Embd", "lp");
	leg2y->Draw("same");

	c1->SaveAs("out_"+run+"_"+particle+"/Part_Eta_datavsembd.gif");

	//Get Eta ratio
	TH1F* Hratio_part_Eta = (TH1F*)H_Data_Part_Eta_Sig->Clone("Hratio_part_Eta");
	Hratio_part_Eta->Rebin();
	H_Embd_Part_Eta_Sig->Rebin();
	Hratio_part_Eta->Divide(H_Embd_Part_Eta_Sig);
	Hratio_part_Eta->Draw("pe");
	c1->SaveAs("out_"+run+"_"+particle+"/Part_Eta_ratio_afterwt.gif");


	//draw part Phi
	H_Data_Part_Phi_Sig->Rebin(8);
	H_Data_Part_Phi_Sig->SetTitle("Part e Phi");
	H_Data_Part_Phi_Sig->SetMarkerStyle(20);
	H_Data_Part_Phi_Sig->SetMaximum(H_Data_Part_Phi_Sig->GetMaximum()*2.);
	H_Data_Part_Phi_Sig->SetMinimum(0);
	H_Data_Part_Phi_Sig->Draw("pe");
	H_Embd_Part_Phi_Sig->Rebin(8);
	H_Embd_Part_Phi_Sig->Scale(H_Data_Part_Phi_Sig->Integral()/H_Embd_Part_Phi_Sig->Integral());
	H_Embd_Part_Phi_Sig->SetMarkerStyle(20);
	H_Embd_Part_Phi_Sig->SetMarkerColor(2);
	H_Embd_Part_Phi_Sig->Draw("pesame");

	TLegend *leg2phi = new TLegend(0.6, 0.75, 0.88, 0.88);
	leg2phi->AddEntry(H_Data_Part_Phi_Sig, "Part e in Data", "lp");
	leg2phi->AddEntry(H_Embd_Part_Phi_Sig, "Part e in Embd", "lp");
	leg2phi->Draw("same");

	c1->SaveAs("out_"+run+"_"+particle+"/Part_Phi_datavsembd.gif");

	//draw prime Pt
	H_Data_Part_Pt_Sig->SetTitle("Part e Pt");
	H_Data_Part_Pt_Sig->SetMarkerStyle(20);
	H_Data_Part_Pt_Sig->Draw("pe");
	H_Embd_Part_Pt_Sig->Scale(H_Data_Part_Pt_Sig->Integral()/H_Embd_Part_Pt_Sig->Integral());
	H_Embd_Part_Pt_Sig->SetMarkerStyle(20);
	H_Embd_Part_Pt_Sig->SetMarkerColor(2);
	H_Embd_Part_Pt_Sig->Draw("pesame");

	TLegend *leg2pt = new TLegend(0.6, 0.75, 0.88, 0.88);
	leg2pt->AddEntry(H_Data_Part_Pt_Sig, "Part e in Data", "lp");
	leg2pt->AddEntry(H_Embd_Part_Pt_Sig, "Part e in Embd", "lp");
	leg2pt->Draw("same");

	c1->SaveAs("out_"+run+"_"+particle+"/Part_Pt_datavsembd.gif");

	//Get Pt ratio
	TH1F* Hratio_part_Pt = (TH1F*)H_Data_Part_Pt_Sig->Clone("Hratio_part_Pt");
	Hratio_part_Pt->Rebin();
	H_Embd_Part_Pt_Sig->Rebin();
	Hratio_part_Pt->Divide(H_Embd_Part_Pt_Sig);
	Hratio_part_Pt->SetMaximum(Hratio_part_Pt->GetMaximum()*1.5);
	Hratio_part_Pt->SetMinimum(0);
	Hratio_part_Pt->Draw("pe");
	c1->SaveAs("out_"+run+"_"+particle+"/Part_Pt_ratio_afterwt.gif");

	c1->SetLogz();
	//compare DsmAdc vs Adc0
	TH2F* H_Data_E_DsmAdcAdc0_US = (TH2F*) fdata->Get("hPEUS_DsmAdcvsAdc0");
	TH2F* H_Data_E_DsmAdcAdc0_LS = (TH2F*) fdata->Get("hPELS_DsmAdcvsAdc0");
	TH2F* H_Data_E_DsmAdcAdc0_Sig = (TH2F*)H_Data_E_DsmAdcAdc0_US->Clone("H_Data_E_DsmAdcAdc0_Sig");
	H_Data_E_DsmAdcAdc0_Sig->SetTitle("DsmAdc vs Adc0 in Data");
	H_Data_E_DsmAdcAdc0_Sig->Draw("colz");

	c1->SaveAs("out_"+run+"_"+particle+"/DsmAdcvsAdc0_data.gif");

	TH2F* H_Embd_E_DsmAdcAdc0 = (TH2F*)fembd->Get("hRcEmcE_DsmAdcvsAdc0");
	H_Embd_E_DsmAdcAdc0->SetTitle("DsmAdc vs Adc0 in Embd");
	H_Embd_E_DsmAdcAdc0->Draw("colz");

	c1->SaveAs("out_"+run+"_"+particle+"/DsmAdcvsAdc0_Embd.gif");

	//for the DsmAdc vs Adc0 difference between data and embedding
	TH1F* hPEUS_TowId[4];
	TH1F* hPELS_TowId[4];
	TH1F* hPE_TowId[4];
	TString CutMode[4] = {"DsmAdc>mTHDsmAdc", "Adc0>mTHAdc0", "DsmAdc>0&DsmAdc<Adc0/16-5", "Adc0>0&&DsmAdc=0"};
	for(int i=0; i<4; i++){
		hPEUS_TowId[i] = (TH1F*)fdata->Get(Form("hPEUS_TowId_%d",i));
		hPELS_TowId[i] = (TH1F*)fdata->Get(Form("hPELS_TowId_%d",i));
		hPE_TowId[i] = (TH1F*) hPEUS_TowId[i]->Clone(Form("hPE_TowId_%d",i));
		hPE_TowId[i]->Add(hPELS_TowId[i], -1);


		hPE_TowId[i]->SetMaximum(hPE_TowId[i]->GetMaximum()*2.6);
		hPE_TowId[i]->SetMarkerStyle(20);
		hPE_TowId[i]->SetMarkerSize(0.5);
		hPE_TowId[i]->Draw("pe");

		tl.DrawLatex(0.12,0.8, CutMode[i]);
		c1->SaveAs("out_"+run+"_"+particle+Form("/towid_%d.gif",i));
		

}







	TFile *outf = new TFile("out_"+run+"_"+particle+"/"+run+"_"+particle+"_out_SingleE_PtEta_wtafwt.root", "recreate");
	outf->cd();
	Hratio_prime_Pt->Write();
	Hratio_prime_Eta->Write();
	Hratio_part_Pt->Write();
	Hratio_part_Eta->Write();
	H_Data_E_DsmAdcAdc0_Sig->Write();
	H_Embd_E_DsmAdcAdc0->Write();

	for(int i=0; i<4; i++){
		hPE_TowId[i]->Write();
	}



	cout<<"outputfile: "<<outf->GetName()<<endl;
	outf->Close();



}
