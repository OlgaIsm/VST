//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "VirtualStringTree.h"
#include "sqlite3.h"
#include <string>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "VirtualTrees"
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	  VirtualStringTree->NodeDataSize = sizeof(VST); // ������ �������� �� ���������
}
//---------------------------------------------------------------------------
void __fastcall TForm1::VirtualStringTreeGetText(TBaseVirtualTree *Sender, PVirtualNode Node,
		  TColumnIndex Column, TVSTTextType TextType, UnicodeString &CellText)

	// ���������� ������� ��� ����������� ������ � ���� VirtualStringTree

{
	if(!Node) return;
	VST *nodeData = (VST*)Sender->GetNodeData(Node);
	switch(Column)
	{
		case 0: CellText = nodeData->id; break;
		case 1: CellText = nodeData->url; break;
		case 2: CellText = nodeData->info; break;
		case 3: CellText = nodeData->count; break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
	VirtualStringTree -> Clear(); // ������� ��������
	sqlite3 *DB = 0;    // ��������� ���������
	if (sqlite3_open("history.sqlite", &DB)) {  //  ��������� �� � ����������� ����������
		Status->Caption = "Error opening the database: %s", sqlite3_errmsg(DB);
	}

	sqlite3_stmt *pullStatement; // ����� ��������������� ������ � �������������� �������� SQL
	const char *sqlSelect = "select * from urls"; // SQL-������ ������ �������
	int result = sqlite3_prepare_v2(DB, sqlSelect, -1, &pullStatement, NULL);  // ������������� SQL - ������ ��� ����������

	if (result != SQLITE_OK) // ��������� ������
	{
		const char* errmsg = sqlite3_errmsg(DB); // ����� ������
		Status->Caption = errmsg;
	}
	else // ���� ��� ������ - ��������� ����
	{
		Status -> Caption = "Complete!";
		VirtualStringTree->BeginUpdate(); // ������ ��������� ������� (���������� ������������������)
		while (sqlite3_step(pullStatement) != SQLITE_DONE)  // ����������� ���� �������
		{
			PVirtualNode entryNode = VirtualStringTree->AddChild(VirtualStringTree->RootNode); // ���������� ������
			VST *nodeData = (VST*)VirtualStringTree->GetNodeData(entryNode);
			nodeData->id = sqlite3_column_int(pullStatement, 0);
			nodeData->url  = (wchar_t*)sqlite3_column_text16(pullStatement, 1);
			nodeData->info  = (wchar_t*)sqlite3_column_text16(pullStatement, 2);
			nodeData->count = sqlite3_column_int(pullStatement, 3);
		}
		VirtualStringTree->EndUpdate(); // ���������� �������
	}
	sqlite3_finalize(pullStatement); // ������������ ���������� ��������
	sqlite3_close(DB); // ��������� ���������
}
//---------------------------------------------------------------------------
void __fastcall TForm1::VirtualStringTreeAddToSelection(TBaseVirtualTree *Sender,
		  PVirtualNode Node)

	// ��������� �������, �������� ���������� ���� � ������� � ���� Status
    // ������ �������� ���������� � ���������� ��������� �����

{
	String count = ((VST*)VirtualStringTree->GetNodeData(Node))->count;
	Status->Caption = "Number of visits: " + count;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender)
{
	sqlite3* DB;    // ��������� ���������
	sqlite3_open("history.sqlite", &DB);    //  ��������� �� � ����������� ����������
	const char *sqlRemoveAll = "delete from urls";  // ��������� ������ � ��
	char *errorMsg; // ��� ����������� ����������� �������
	int result = sqlite3_exec(DB, sqlRemoveAll, NULL, NULL, &errorMsg); // ������� ��� �������� ��� ���������� �������� ��
	sqlite3_close(DB);  // ��������� ���������

	if (result == SQLITE_OK)
	{
		VirtualStringTree -> BeginUpdate();
		VirtualStringTree -> Clear();
		VirtualStringTree -> EndUpdate();
		Status->Caption = "Sector clear!";
	}
	else
		Status->Caption = "Database cleanup request failed!";
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button3Click(TObject *Sender)
{
	if (VirtualStringTree->FocusedNode != NULL) // ��������� �������� �� ������
	{
		int ID = ((VST*)VirtualStringTree->GetNodeData(VirtualStringTree->FocusedNode))->id;
		const char *sqlRemoveRow = ("delete from urls where id=" + std::to_string(ID)).c_str();
		sqlite3* DB;    // ��������� ���������
		sqlite3_open("history.sqlite", &DB);    //  ��������� �� � ����������� ����������
		char *errorMsg;
		sqlite3_exec(DB, sqlRemoveRow, NULL, NULL, &errorMsg); // ������� ��� �������� ��� ���������� �������� ��
		sqlite3_close(DB);  // ��������� ���������
		VirtualStringTree->DeleteSelectedNodes(); // �������� �� ������������ ����������
		Status->Caption = "Complete!";
	}
	else
		Status->Caption = "Select the line!";
}
//---------------------------------------------------------------------------
