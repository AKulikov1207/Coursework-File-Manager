//---------------------------------------------------------------------------
#ifndef Unit2H
#define Unit2H
//---------------------------------------------------------------------------
#endif

#define LEFT 1
#define RIGHT 0
#define FO_DELETER 5

extern int DirSel;
extern AnsiString RootDirLeft, RootDirRight;
extern AnsiString CurrentPathLeft, CurrentPathRight;
extern int Panel;

void CreateDirAttribSet();
AnsiString HandlerOfOpen(AnsiString RootDir, int Panel, int DirSel);
void CreateListOfDevices();
bool CreateFolder(int Panel);
void Deleting(int Panel, int Operation);
void CopyOrRemove(int Panel, int Operation);
void Formatting(AnsiString RootDir, int type);
void Searching(AnsiString path, AnsiString name);

