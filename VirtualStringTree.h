//---------------------------------------------------------------------------
#ifndef VirtualStringTreeH
#define VirtualStringTreeH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "VirtualTrees.hpp"
//---------------------------------------------------------------------------

struct VST
{
	int id;
	UnicodeString url;
	UnicodeString info;
	int count;
};

//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TVirtualStringTree *VirtualStringTree;
	TLabel *Information;
	TLabel *Status;
	TButton *Button1;
	TButton *Button2;
	TButton *Button3;
	void __fastcall VirtualStringTreeGetText(TBaseVirtualTree *Sender, PVirtualNode Node,
          TColumnIndex Column, TVSTTextType TextType, UnicodeString &CellText);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall VirtualStringTreeAddToSelection(TBaseVirtualTree *Sender, PVirtualNode Node);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);


private:	// User declarations
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
