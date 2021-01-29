//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "Unit2.h"
#include "Unit3.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormDialogCreateDir *FormDialogCreateDir;
//---------------------------------------------------------------------------
__fastcall TFormDialogCreateDir::TFormDialogCreateDir(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormDialogCreateDir::bCancelClick(TObject *Sender)
{
	FormDialogCreateDir->edNewDirName->Text = "";
	FormDialogCreateDir->edNewDirName->SetFocus();
	FormDialogCreateDir->Hide();
}
//---------------------------------------------------------------------------
void __fastcall TFormDialogCreateDir::bOkClick(TObject *Sender)
{
	bool Result = CreateFolder(Panel);
	FormDialogCreateDir->edNewDirName->SetFocus();

	if (Result == true) {
		FormDialogCreateDir->edNewDirName->Text = "";
		FormDialogCreateDir->Hide();
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormDialogCreateDir::edNewDirNameKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
	bool Result;
	switch (Key) {
		case 13:
			Result = CreateFolder(Panel);
			FormDialogCreateDir->edNewDirName->SetFocus();

			if (Result == true) {
				FormDialogCreateDir->edNewDirName->Text = "";
				FormDialogCreateDir->Hide();
			}
			break;
		case 27:
			FormDialogCreateDir->edNewDirName->Text = "";
			FormDialogCreateDir->edNewDirName->SetFocus();
			FormDialogCreateDir->Hide();
			break;
	}
}
//---------------------------------------------------------------------------

