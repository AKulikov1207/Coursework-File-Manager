//---------------------------------------------------------------------------

#ifndef Unit4H
#define Unit4H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TFormDialogDelete : public TForm
{
__published:	// IDE-managed Components
        TButton *bDelete;
        TButton *bCancel;
        TButton *bRecycler;
        TLabel *Label1;
        void __fastcall bCancelClick(TObject *Sender);
        void __fastcall bDeleteClick(TObject *Sender);
        void __fastcall bRecyclerClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
		__fastcall TFormDialogDelete(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormDialogDelete *FormDialogDelete;
//---------------------------------------------------------------------------
#endif
