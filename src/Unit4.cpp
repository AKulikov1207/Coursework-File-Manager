//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit4.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormDialogDelete *FormDialogDelete;
#include "Unit2.h"
//---------------------------------------------------------------------------
__fastcall TFormDialogDelete::TFormDialogDelete(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormDialogDelete::bCancelClick(TObject *Sender)
{
	FormDialogDelete->bRecycler->SetFocus();
	FormDialogDelete->Hide();
}
//---------------------------------------------------------------------------
void __fastcall TFormDialogDelete::bDeleteClick(TObject *Sender)
{
	Deleting(Panel, FO_DELETE);
	FormDialogDelete->bRecycler->SetFocus();
	FormDialogDelete->Hide();
}
//---------------------------------------------------------------------------
void __fastcall TFormDialogDelete::bRecyclerClick(TObject *Sender)
{
	Deleting(Panel, FO_DELETER);
	FormDialogDelete->bRecycler->SetFocus();
	FormDialogDelete->Hide();
}
//---------------------------------------------------------------------------

