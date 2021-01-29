//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>

//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TComboBox *ListDevicesLeft;
        TComboBox *ListDevicesRight;
        TStringGrid *FileListRight;
        TLabel *LabelCurrentPathRight;
        TLabel *LabelCurrentPathLeft;
        TButton *bRemove;
        TButton *bCopy;
        TButton *bCreateDir;
        TButton *bDelete;
        TButton *bCurrentRootDirLeft;
        TButton *bCurrentRootDirRight;
        TStringGrid *FileListLeft;
		TLabel *LabelFormatting;
		TButton *bFormatting;
		TCheckBox *StandardFormatting;
		TCheckBox *QuickFormatting;
	TLabeledEdit *LabeledEditSearch;
	TButton *bSearch;
        void __fastcall FileListLeftDblClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall ListDevicesLeftSelect(TObject *Sender);
        void __fastcall ListDevicesRightSelect(TObject *Sender);
        void __fastcall FileListRightDblClick(TObject *Sender);
        void __fastcall FileListLeftEnter(TObject *Sender);
        void __fastcall FileListRightEnter(TObject *Sender);
        void __fastcall bCreateDirClick(TObject *Sender);
        void __fastcall ListDevicesLeftEnter(TObject *Sender);
        void __fastcall ListDevicesRightEnter(TObject *Sender);
        void __fastcall bDeleteClick(TObject *Sender);
        void __fastcall bCopyClick(TObject *Sender);
        void __fastcall bRemoveClick(TObject *Sender);
        void __fastcall FileListLeftKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall FileListRightKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall bCurrentRootDirLeftClick(TObject *Sender);
        void __fastcall bCurrentRootDirRightClick(TObject *Sender);
	    void __fastcall bFormattingClick(TObject *Sender);
	void __fastcall bSearchClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1; 
//---------------------------------------------------------------------------
#endif

