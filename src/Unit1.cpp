//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "Unit1.h"
#include "Unit2.h"
#include "Unit3.h"
#include "Unit4.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
	CreateListOfDevices();
	CreateDirAttribSet();
	RootDirLeft = "C:\\";
	RootDirRight = "C:\\";
	DirSel = 1;
	CurrentPathLeft = HandlerOfOpen(RootDirLeft, LEFT, DirSel);
	CurrentPathRight = HandlerOfOpen(RootDirRight, RIGHT, DirSel);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FileListLeftDblClick(TObject *Sender)
{
	DirSel = 0;
	CurrentPathLeft = HandlerOfOpen(RootDirLeft, LEFT, DirSel);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FileListRightDblClick(TObject *Sender)
{
	DirSel = 0;
	CurrentPathRight = HandlerOfOpen(RootDirRight, RIGHT, DirSel);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ListDevicesLeftSelect(TObject *Sender)
{
	RootDirLeft = Form1->ListDevicesLeft->Text.Delete
						(3, (Form1->ListDevicesLeft->Text.Length() - 2));
	DirSel = 1;
	CurrentPathLeft = HandlerOfOpen(RootDirLeft, LEFT, DirSel);
	Form1->FileListLeft->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ListDevicesRightSelect(TObject *Sender)
{
	RootDirRight = Form1->ListDevicesRight->Text.Delete
					   (3, (Form1->ListDevicesRight->Text.Length() - 2));
	DirSel = 1;
	CurrentPathRight = HandlerOfOpen(RootDirRight, RIGHT, DirSel);
	Form1->FileListRight->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FileListLeftEnter(TObject *Sender)
{
	Panel = LEFT;
	Form1->LabelCurrentPathLeft->Color = TColor(0x00E9E9E9);
	Form1->ListDevicesLeft->Color = TColor(0x00E9E9E9);
	Form1->FileListLeft->Color = clGradientInactiveCaption;
	Form1->LabelCurrentPathRight->Color = TColor(0x00C4C4C4);
	Form1->ListDevicesRight->Color = TColor(0x00C4C4C4);
	Form1->FileListRight->Color = TColor(0x00C4C4C4);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FileListRightEnter(TObject *Sender)
{
	Panel = RIGHT;
	Form1->LabelCurrentPathRight->Color = TColor(0x00E9E9E9);
	Form1->ListDevicesRight->Color = TColor(0x00E9E9E9);
	Form1->FileListRight->Color = clGradientInactiveCaption;
	Form1->LabelCurrentPathLeft->Color = TColor(0x00C4C4C4);
	Form1->ListDevicesLeft->Color = TColor(0x00C4C4C4);
	Form1->FileListLeft->Color = TColor(0x00C4C4C4);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ListDevicesLeftEnter(TObject *Sender)
{
	Form1->LabelCurrentPathLeft->Color = TColor(0x00E9E9E9);
	Form1->ListDevicesLeft->Color = TColor(0x00E9E9E9);
	Form1->FileListLeft->Color = clGradientInactiveCaption;
	Form1->LabelCurrentPathRight->Color = TColor(0x00C4C4C4);
	Form1->ListDevicesRight->Color = TColor(0x00C4C4C4);
	Form1->FileListRight->Color = TColor(0x00C4C4C4);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ListDevicesRightEnter(TObject *Sender)
{
	Form1->LabelCurrentPathRight->Color = TColor(0x00E9E9E9);
	Form1->ListDevicesRight->Color = TColor(0x00E9E9E9);
	Form1->FileListRight->Color = clGradientInactiveCaption;
	Form1->LabelCurrentPathLeft->Color = TColor(0x00C4C4C4);
	Form1->ListDevicesLeft->Color = TColor(0x00C4C4C4);
	Form1->FileListLeft->Color = TColor(0x00C4C4C4);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::bCreateDirClick(TObject *Sender)
{
	FormDialogCreateDir->Show();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::bDeleteClick(TObject *Sender)
{
	FormDialogDelete->Show();
	FormDialogDelete->bRecycler->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::bCopyClick(TObject *Sender)
{
	CopyOrRemove(Panel, FO_COPY);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::bRemoveClick(TObject *Sender)
{
	CopyOrRemove(Panel, FO_MOVE);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FileListLeftKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
	switch (Key) {
		case 13:
			DirSel = 0;
			CurrentPathLeft = HandlerOfOpen(RootDirLeft, LEFT, DirSel);
			break;
		case 116:
			CopyOrRemove(Panel, FO_COPY);
			break;
		case 117:
			CopyOrRemove(Panel, FO_MOVE);
			break;
		case 118:
			FormDialogCreateDir->Show();
			break;
		case 119:
			FormDialogDelete->Show();
			FormDialogDelete->bRecycler->SetFocus();
			break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FileListRightKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
	switch (Key) {
		case 13:
			DirSel = 0;
			CurrentPathRight = HandlerOfOpen(RootDirRight, RIGHT, DirSel);
			break;
		case 116:
			CopyOrRemove(Panel, FO_COPY);
			break;
		case 117:
			CopyOrRemove(Panel, FO_MOVE);
			break;
		case 118:
			FormDialogCreateDir->Show();
			break;
		case 119:
			FormDialogDelete->Show();
			FormDialogDelete->bRecycler->SetFocus();
			break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::bCurrentRootDirLeftClick(TObject *Sender)
{
	DirSel = 2;
	CurrentPathLeft = HandlerOfOpen(RootDirLeft, LEFT, DirSel);
	Form1->FileListLeft->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::bCurrentRootDirRightClick(TObject *Sender)
{
	DirSel = 2;
	CurrentPathRight = HandlerOfOpen(RootDirRight, RIGHT, DirSel);
	Form1->FileListRight->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::bFormattingClick(TObject *Sender)
{
	if (Form1->QuickFormatting->Checked == true) {
		Formatting(RootDirLeft, 1);
	}
	else if (Form1->StandardFormatting->Checked == true) {
		Formatting(RootDirLeft, 0);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::bSearchClick(TObject *Sender)
{
	Searching("C:\\", Form1->LabeledEditSearch->Text);
}
//---------------------------------------------------------------------------

