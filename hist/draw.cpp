#include <iostream>
#include <string>
#include <TH1D.h>
#include <TFile.h>
#include <TLegend.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <TF1.h>
#include <TF2.h>
#include <TApplication.h>

void draw_1D(const char*, const char*, TF1*);
void draw_2D(const char*, const char*, TF2*);

int main(int argc, char **argv){  
	TApplication app("app", &argc, argv);
	
	TF1 *fbp = new TF1("fhb", "1000 * ([0] * (1 - x) ^3 + [1] * x * (1 - x)^2)", 0, 1);
	TF1 *fne1D = new TF1("fnepq", "1000 * ([0] - [1] * x)", 0, 1);
	TF2 *fne2D = new TF2("fne", "1000 * ([0] - [1] * x) * ([2] - [3] * y)", 0, 1, 0, 1);
	fbp->SetParameters(1, 3);
	fne1D->SetParameters(1, 1);
	fne2D->SetParameters(1, 1, 1, 1);
	
	
	TF1 *fshor_p = new TF1("fshp", "1000 * [0] * (1 - x^3 - 3 * x^2 * (1 - x))^3 + 1000 * [1] * (x^3 + 3 * x^2 * (1 - x))^2 * (1 - x^3 - 3 * x^2 * (1 - x))", 0, 1);
	TF1 *fshor_q = new TF1("fshp", "1000 * [0] * ((1 - x)^3 + 3 * (1 - x) * x^2)^3 + 1000 * [1] * (3 * x * (1 - x )^2 + x^3)* ((1 - x)^3 + 3 * (1 - x) * x^2)^2", 0, 1);
	fshor_p->SetParameters(1, 3);
	fshor_q->SetParameters(1, 3);
	
	int input;
	std::cout << "Choose an histogram typing the corresponding number:" <<std::endl;
	std::cout << "1) No encoding (bit flip)" <<std::endl;
	std::cout << "2) No encoding (phase flip)" <<std::endl;
	std::cout << "3) No encoding 2D" <<std::endl;
	std::cout << "4) Bit flip circuit" <<std::endl;
	std::cout << "5) Phase flip circuit" <<std::endl;
	std::cout << "6) Shor code (bit flip)" <<std::endl;
	std::cout << "7) Shor code (phase flip)" <<std::endl;
	std::cin >> input;
	switch(input){
		case 1:
			draw_1D("no_encoding_q", "Phase flip (no encoding);q;Success rate", fne1D);
			break;
		case 2:
			draw_1D("no_encoding_q", "Phase flip (no encoding);q;Success rate", fne1D);
			break;
		case 3:
			draw_2D("no_encoding", "No encoding;p;q;Success rate", fne2D);
			break;
		case 4:
			draw_1D("bit_flip", "Bit flip;p;Success rate", fbp);
			break;
		case 5:
			draw_1D("phase_flip", "Phase flip;q;Success rate", fbp);
			break;
		case 6:
			draw_1D("shor_p", "Shor code (bit flip);p;Success rate", fshor_p);
			break;
		case 7:
			draw_1D("shor_q", "Shor code (phase flip);q;Success rate", fshor_q);
			break;
	}
	
	app.Run();
	return 0;
}


void draw_1D(const char* name, const char* title, TF1* func){
	// std::string filename = "hist/" + std::string(name) + ".root";
	std::string filename = std::string(name) + ".root";
	TFile* f = new TFile(filename.c_str(), "READ");
	TH1D* h = (TH1D*)f->Get(name);
	h->SetStats(0);
	h->SetDirectory(0);
	f->Close();
	
	h->Draw("HIST");
	h->SetTitle(title); 				// title; xlabel; ylabel
	h->SetFillStyle(3165);
	h->SetLineColor(kBlack);
	h->SetLineWidth(2);
	h->Fit(func);
	
	func->Draw("SAME");
	
	TF1* line = new TF1("line", "1000 - 1000 * x", 0, 1);
	line->Draw("SAME");
	line->SetLineColor(kBlue);
	line->SetLineWidth(3);
	line->SetLineStyle(2);
	
	auto legend = new TLegend(0.65, 0.7, .9, .9);
	legend->AddEntry((TObject*)0, "", "");
	legend->AddEntry(h,"Success cases","f");
	legend->AddEntry(func,"Fit","l");
	legend->AddEntry(line,"Without encoding","l");
	legend->AddEntry((TObject*)0, "", "");
	legend->Draw();
}

void draw_2D(const char* name, const char* title, TF2* func){
	std::string filename = std::string(name) + ".root";
	TFile* f = new TFile(filename.c_str(), "READ");
	TH2D* h = (TH2D*)f->Get(name);
	h->SetStats(0);
	h->SetDirectory(0);
	f->Close();
	
	h->Draw("LEGO");
	h->SetTitle(title); 				// title; xlabel; ylabel
	h->SetFillStyle(3165);
	h->SetFillColor(kBlack);
	h->SetLineColor(kBlack);
	h->SetLineWidth(2);
	h->Fit(func);
	
	 
    auto legend = new TLegend(0.65, 0.7, .9, .9);
	legend->AddEntry((TObject*)0, "", "");
	legend->AddEntry(h,"Success cases","f");
	legend->AddEntry(func,"Fit","l");
	legend->AddEntry((TObject*)0, "", "");
	legend->Draw();
}
