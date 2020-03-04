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
	  VirtualStringTree->NodeDataSize = sizeof(VST); // Размер элемента по структуре
}
//---------------------------------------------------------------------------
void __fastcall TForm1::VirtualStringTreeGetText(TBaseVirtualTree *Sender, PVirtualNode Node,
		  TColumnIndex Column, TVSTTextType TextType, UnicodeString &CellText)

	// Обработчик события для отображения текста в поле VirtualStringTree

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
	VirtualStringTree -> Clear(); // Очистка элемента
	sqlite3 *DB = 0;    // Открываем соединеие
	if (sqlite3_open("history.sqlite", &DB)) {  //  Открывыем БД в программном интерфейсе
		Status->Caption = "Error opening the database: %s", sqlite3_errmsg(DB);
	}

	sqlite3_stmt *pullStatement; // Класс предоставляющий доступ к подготовленным запросам SQL
	const char *sqlSelect = "select * from urls"; // SQL-запрос вывода таблицы
	int result = sqlite3_prepare_v2(DB, sqlSelect, -1, &pullStatement, NULL);  // Подготавлиает SQL - запрос для выполнения

	if (result != SQLITE_OK) // Обработка ошибок
	{
		const char* errmsg = sqlite3_errmsg(DB); // Вывод ошибки
		Status->Caption = errmsg;
	}
	else // Если все хорошо - загружаем базу
	{
		Status -> Caption = "Complete!";
		VirtualStringTree->BeginUpdate(); // Запрет изменений таблицы (увеличение производительности)
		while (sqlite3_step(pullStatement) != SQLITE_DONE)  // Прохождение всей таблицы
		{
			PVirtualNode entryNode = VirtualStringTree->AddChild(VirtualStringTree->RootNode); // Добавление записи
			VST *nodeData = (VST*)VirtualStringTree->GetNodeData(entryNode);
			nodeData->id = sqlite3_column_int(pullStatement, 0);
			nodeData->url  = (wchar_t*)sqlite3_column_text16(pullStatement, 1);
			nodeData->info  = (wchar_t*)sqlite3_column_text16(pullStatement, 2);
			nodeData->count = sqlite3_column_int(pullStatement, 3);
		}
		VirtualStringTree->EndUpdate(); // Отображаем таблицу
	}
	sqlite3_finalize(pullStatement); // Освобождение выделенных ресурсов
	sqlite3_close(DB); // Закрываем соединеие
}
//---------------------------------------------------------------------------
void __fastcall TForm1::VirtualStringTreeAddToSelection(TBaseVirtualTree *Sender,
		  PVirtualNode Node)

	// Обработка события, выделяет информацию узла и выводит в поле Status
    // Выводи скрытыую информацию о количестве посещений сайта

{
	String count = ((VST*)VirtualStringTree->GetNodeData(Node))->count;
	Status->Caption = "Number of visits: " + count;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender)
{
	sqlite3* DB;    // Открываем соединеие
	sqlite3_open("history.sqlite", &DB);    //  Открывыем БД в программном интерфейсе
	const char *sqlRemoveAll = "delete from urls";  // Формируем запрос к БД
	char *errorMsg; // Для корректного выполенения функции
	int result = sqlite3_exec(DB, sqlRemoveAll, NULL, NULL, &errorMsg); // Функция для удаления или добавления объектов БД
	sqlite3_close(DB);  // Закрываем соединеие

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
	if (VirtualStringTree->FocusedNode != NULL) // Проверяем выделена ли строка
	{
		int ID = ((VST*)VirtualStringTree->GetNodeData(VirtualStringTree->FocusedNode))->id;
		const char *sqlRemoveRow = ("delete from urls where id=" + std::to_string(ID)).c_str();
		sqlite3* DB;    // Открываем соединеие
		sqlite3_open("history.sqlite", &DB);    //  Открывыем БД в программном интерфейсе
		char *errorMsg;
		sqlite3_exec(DB, sqlRemoveRow, NULL, NULL, &errorMsg); // Функция для удаления или добавления объектов БД
		sqlite3_close(DB);  // Закрываем соединеие
		VirtualStringTree->DeleteSelectedNodes(); // Удаление из графического интерфейса
		Status->Caption = "Complete!";
	}
	else
		Status->Caption = "Select the line!";
}
//---------------------------------------------------------------------------
