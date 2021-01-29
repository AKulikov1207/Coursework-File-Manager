//---------------------------------------------------------------------------

#ifndef Unit3H
#define Unit3H
//---------------------------------------------------------------------------
#include "Unit2.h"
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>

//---------------------------------------------------------------------------
class TFormDialogCreateDir : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TEdit *edNewDirName;
        TButton *bOk;
        TButton *bCansel;
        void __fastcall bCancelClick(TObject *Sender);
        void __fastcall bOkClick(TObject *Sender);
        void __fastcall edNewDirNameKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
private:	// User declarations
public:		// User declarations
        __fastcall TFormDialogCreateDir(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormDialogCreateDir *FormDialogCreateDir;
//---------------------------------------------------------------------------
#endif
