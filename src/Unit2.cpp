//---------------------------------------------------------------------------
#pragma hdrstop
#include "Unit1.h"
#include "Unit3.h"
#include <winbase.h>
#include <_str.h>
#include <stdlib.h>
#include <system.hpp>
#include <shellapi.h>
#include <windows.h>
#include <winnt.h>
#include <deque>
#include <set>
#include <string>
#include <Shlobj.h>
#include <SysUtils.hpp>

using namespace std;

//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
int DirSel = 0;                                // ������� ���������� ������ � ListDevices
AnsiString RootDirLeft, RootDirRight;          // ������� �������� ����������
AnsiString CurrentPathLeft, CurrentPathRight;  // ������� ����
int Panel = LEFT;

struct _FileProperty{                 //��������� ������

	char    FileAttrib[12];           // �������� �����
	char    CreateTime[25];           // ����� �������� �����
	char    AccessTime[25];           // ����� �������
	char    WriteTime[25];            // ����� ������
	char    FileSizeBy[21];           // ������ ����� � �
	char    FileSizeKb[18];           // ������ ����� � ��
	TCHAR   FileName[MAX_PATH];       // ��� �����
	TCHAR   AlterFileName[14];        // �������������� ��� �����
}FileProperty;

deque<WIN32_FIND_DATAA> DequeFindLeft; //������� ����������� ������
deque<WIN32_FIND_DATAA> DequeFindRight;

WIN32_FIND_DATAA StructFind;           //��������� ����������� ������

int ColName = 0,                       //������� ������� ������
	ColSize = 1,
	ColAttrib = 2,
	ColCreTime = 5,
	ColAccTime = 4,
	ColWriTime = 3;

//��������� ��������� ����������
set<unsigned long, less<unsigned long> > DirAttribSet;
void CreateDirAttribSet(){            //�������� ��������� ��������� ����������
	DirAttribSet.insert(16);
	DirAttribSet.insert(17);
	DirAttribSet.insert(18);
	DirAttribSet.insert(19);
	DirAttribSet.insert(20);
	DirAttribSet.insert(21);
	DirAttribSet.insert(22);
	DirAttribSet.insert(48);
	DirAttribSet.insert(49);
	DirAttribSet.insert(50);
	DirAttribSet.insert(8208);
}
//---------------------------------------------------------------------------
//�������� ������ ��������� ���������
void CreateListOfDevices() {
	UINT DriveType;
	char RootPath[4];

	for(char i = 'A'; i <= 'Z'; i++) {
		strcpy(RootPath, "\0");
		RootPath[0] = i;
		RootPath[1] = ':';
		RootPath[2] = '\\';
		RootPath[3] = '\0';
		DriveType = GetDriveTypeA(RootPath);

		switch (DriveType) {
			case DRIVE_REMOVABLE:
				strcat(RootPath, "  Flash drive");
				Form1->ListDevicesLeft->Items->Add(RootPath);
				Form1->ListDevicesRight->Items->Add(RootPath);
				break;
			case DRIVE_FIXED:
				strcat(RootPath, "  Hard drive");
				Form1->ListDevicesLeft->Items->Add(RootPath);
				Form1->ListDevicesRight->Items->Add(RootPath);
				break;
			case DRIVE_REMOTE:
				strcat(RootPath, "  Network drive");
				Form1->ListDevicesLeft->Items->Add(RootPath);
				Form1->ListDevicesRight->Items->Add(RootPath);
			case DRIVE_CDROM:
				strcat(RootPath, "  CD-ROM");
				Form1->ListDevicesLeft->Items->Add(RootPath);
				Form1->ListDevicesRight->Items->Add(RootPath);
				break;
			case DRIVE_RAMDISK:
				strcat(RootPath, "  RAM-Disk");
				Form1->ListDevicesLeft->Items->Add(RootPath);
				Form1->ListDevicesRight->Items->Add(RootPath);
				break;
		}
	}
}
//------------------------------------------------------------------------------

//�������������� �� ��������� ����������� ������ � ��������� ������
void StrucFindToFileProp(WIN32_FIND_DATAA StructFind, _FileProperty &FileProperty) {
	//�������������� �����
	strcpy((char*)FileProperty.FileName, (const char *)StructFind.cFileName);
	strcpy((char*)FileProperty.AlterFileName, (const char *)StructFind.cAlternateFileName);
	char  Temp[10];

	//�������������� ���������
	switch (StructFind.dwFileAttributes) {
		case 1:
		case 33:
			strcpy(FileProperty.FileAttrib, "ROnly\0");
			break;
		case 2:
			strcpy(FileProperty.FileAttrib, "Hide\0");
			break;
		case 4:
		case 6:
		case 34:
		case 35:
		case 38:
		case 39:
			strcpy(FileProperty.FileAttrib, "Sys\0");
			break;
		case 8:
			strcpy(FileProperty.FileAttrib, "VolID\0");
			break;
		case 16:
		case 17:
		case 20:
		case 21:
		case 48:
		case 49:
		case 8208:
			strcpy(FileProperty.FileAttrib, "Dir\0");
			break;
		case 18:
			strcpy(FileProperty.FileAttrib, "HideDir\0");
			break;
		case 19:
		case 50:
		case 22:
			strcpy(FileProperty.FileAttrib, "HideSysDir\0");
			break;
		case 32:
			strcpy(FileProperty.FileAttrib, "Arch\0");
			break;
		case 63:
			strcpy(FileProperty.FileAttrib, "AnyF\0");
			break;
		case 64:
			strcpy(FileProperty.FileAttrib, "Device\0");
			break;
		case 256:
			strcpy(FileProperty.FileAttrib, "Temp\0");
			break;
		case 512:
			strcpy(FileProperty.FileAttrib, "Sprase\0");
			break;
		case 1024:
			strcpy(FileProperty.FileAttrib, "RepraseP\0");
			break;
		case 2048:
			strcpy(FileProperty.FileAttrib, "Compress\0");
			break;
		case 8192:
			strcpy(FileProperty.FileAttrib, "NContentInd\0");
			break;
		case 4096:
			strcpy(FileProperty.FileAttrib, "Offline\0");
			break;
		case 16384:
			strcpy(FileProperty.FileAttrib, "Encryp\0");
			break;
		default:
			strcpy(FileProperty.FileAttrib, ultoa(StructFind.dwFileAttributes, Temp, 10));
			strcat(FileProperty.FileAttrib, "\0");
			break;
	}

	//�������������� �������
	if (StructFind.nFileSizeHigh != 0){                   // ���� ������� ����� �� ����� ����
		char TempStr[20];
		strcpy(FileProperty.FileSizeBy, ultoa(StructFind.nFileSizeHigh, TempStr, 10));
		strcat(FileProperty.FileSizeBy, ultoa(StructFind.nFileSizeLow, TempStr, 10));
		strcat(FileProperty.FileSizeBy, "\0");
		strcpy(FileProperty.FileSizeKb, ">4��\0");
	}
	else {
		char TempStr[20];                                 // ���� ������� ����� ����� ����
		strcpy(FileProperty.FileSizeBy, ultoa(StructFind.nFileSizeLow, TempStr, 10));
		strcat(FileProperty.FileSizeBy, "\0");
		strcpy(FileProperty.FileSizeKb, ultoa((StructFind.nFileSizeLow>>10), TempStr, 10));
		strcat(FileProperty.FileSizeKb, "\0");
	}

	if (DirAttribSet.count(StructFind.dwFileAttributes)){ // ���� �����, �� �� ������
		strcpy(FileProperty.FileSizeBy, "DIR\0");         // �� ������������
	 	strcpy(FileProperty.FileSizeKb, "DIR\0");
	}
	else if ((StructFind.nFileSizeLow>>10) == 0){         // ���� ������ ����� ������ ���������,
		strcpy(FileProperty.FileSizeKb, FileProperty.FileSizeBy); // �� ������������ ��� ������ � ������
		strcat(FileProperty.FileSizeKb, " �   ");
	}

	//������������� �������...
	void FileTimeToStrTime(_FILETIME &FileTime, char CharTime[]);
	FileTimeToStrTime(StructFind.ftCreationTime, FileProperty.CreateTime);
	FileTimeToStrTime(StructFind.ftLastAccessTime, FileProperty.AccessTime);
	FileTimeToStrTime(StructFind.ftLastWriteTime, FileProperty.WriteTime);
}
//------------------------------------------------------------------------------
void FileTimeToStrTime(_FILETIME &FileTime, char CharTime[]) {
	_SYSTEMTIME DigitTime;
	char TempStr[3];

	FileTimeToSystemTime(&FileTime, &DigitTime);
	strcpy(CharTime, "\0");
	strcat(CharTime, ultoa(DigitTime.wDay, TempStr, 10));
	strcat(CharTime, ".");
	strcat(CharTime, ultoa(DigitTime.wMonth, TempStr, 10));
	strcat(CharTime, ".");
	strcat(CharTime, ultoa(DigitTime.wYear, TempStr, 10));
	strcat(CharTime, " ");
	strcat(CharTime, ultoa(DigitTime.wHour, TempStr, 10));
	strcat(CharTime, ":");
	strcat(CharTime, ultoa(DigitTime.wMinute, TempStr, 10));
	strcat(CharTime, ":");
	strcat(CharTime, ultoa(DigitTime.wSecond, TempStr, 10));
	strcat(CharTime, "\0");
}
//------------------------------------------------------------------------------

//������������ ����������� ��������
void ScanDir(const char *Path, int Panel) {
	HANDLE HWNDFind;
	bool Flag = true;

	if (Panel == LEFT) {                            // ���� ������� ����� ������
		if (!DequeFindLeft.empty())
			DequeFindLeft.clear();                  // ������� ������� �������
		HWNDFind = FindFirstFileA(Path, &StructFind);
		while (Flag) {
			Flag = FindNextFileA(HWNDFind, &StructFind);
			if (Flag)
				DequeFindLeft.push_back(StructFind);
		}
		FindClose(HWNDFind);
	}
	else {                                          // ���� ������� ������ ������
		if (!DequeFindRight.empty())
			DequeFindRight.clear();
		HWNDFind = FindFirstFileA(Path, &StructFind);
		while (Flag) {
			Flag = FindNextFileA(HWNDFind, &StructFind);
			if (Flag)
				DequeFindRight.push_back(StructFind);
		}
		FindClose(HWNDFind);
	}
}
//------------------------------------------------------------------------------

//����������� ����������� ��������
void ViewDirectory(const char *Path, int Panel) {
	ScanDir(Path, Panel);
	deque<WIN32_FIND_DATAA>::iterator iter;
	WIN32_FIND_DATAA StructFindTemp;

	int i = 0;                                 // ���������� ���������� ��������
	Form1->FileListLeft->Cells[ColName][i] = "���";
	Form1->FileListLeft->Cells[ColSize][i] = "������ ��";
	Form1->FileListLeft->Cells[ColAttrib][i] = "��������";
	Form1->FileListLeft->Cells[ColCreTime][i] = "������";
	Form1->FileListLeft->Cells[ColAccTime][i] = "������";
	Form1->FileListLeft->Cells[ColWriTime][i] = "�������";

	Form1->FileListRight->Cells[ColName][i] = "���";
	Form1->FileListRight->Cells[ColSize][i] = "������ ��";
	Form1->FileListRight->Cells[ColAttrib][i] = "��������";
	Form1->FileListRight->Cells[ColCreTime][i] = "������";
	Form1->FileListRight->Cells[ColAccTime][i] = "������";
	Form1->FileListRight->Cells[ColWriTime][i] = "�������";
	i++;

	if (Panel == LEFT){                        // ���� ������� ����� ������
		iter = DequeFindLeft.begin();
		Form1->FileListLeft->RowCount = 2;     // ������� ����������� �������,
											   // ���� ������������ �����������
		while (iter != DequeFindLeft.end()) {
			StructFindTemp =* iter;

			if (DirAttribSet.count(StructFindTemp.dwFileAttributes)){// ����������� ����������
				StrucFindToFileProp(StructFindTemp, FileProperty);    // (� ������� ���� ������)
				Form1->FileListLeft->Cells[ColName][i] = FileProperty.FileName;
				Form1->FileListLeft->Cells[ColSize][i] = FileProperty.FileSizeKb;
				Form1->FileListLeft->Cells[ColAttrib][i] = FileProperty.FileAttrib;
				Form1->FileListLeft->Cells[ColCreTime][i] = FileProperty.CreateTime;
				Form1->FileListLeft->Cells[ColAccTime][i] = FileProperty.AccessTime;
				Form1->FileListLeft->Cells[ColWriTime][i] = FileProperty.WriteTime;
				Form1->FileListLeft->RowCount = Form1->FileListLeft->RowCount + 1;
				i++;
			}
			iter++;
		}

		iter = DequeFindLeft.begin();
		while (iter != DequeFindLeft.end()){ // ����������� ������ (� ������� ����� ����������)
			StructFindTemp =* iter;

			if (DirAttribSet.count(StructFindTemp.dwFileAttributes) == 0) {
				StrucFindToFileProp (*iter, FileProperty);
				Form1->FileListLeft->Cells[ColName][i] = FileProperty.FileName;
				Form1->FileListLeft->Cells[ColSize][i] = FileProperty.FileSizeKb;
				Form1->FileListLeft->Cells[ColAttrib][i] = FileProperty.FileAttrib;
				Form1->FileListLeft->Cells[ColCreTime][i] = FileProperty.CreateTime;
				Form1->FileListLeft->Cells[ColAccTime][i] = FileProperty.AccessTime;
				Form1->FileListLeft->Cells[ColWriTime][i] = FileProperty.WriteTime;
				Form1->FileListLeft->RowCount = Form1->FileListLeft->RowCount + 1;
				i++;
			}
			iter++;
		}

		if (i == 1){                 // ���� ��� ������ � ������ ��� �����������
			Form1->FileListLeft->Cells[ColName][i] = "\0";
			Form1->FileListLeft->Cells[ColSize][i] = "\0";
			Form1->FileListLeft->Cells[ColAttrib][i] = "\0";
			Form1->FileListLeft->Cells[ColCreTime][i] = "\0";
			Form1->FileListLeft->Cells[ColAccTime][i] = "\0";
			Form1->FileListLeft->Cells[ColWriTime][i] = "\0";
		}

		if (i > 1)
			Form1->FileListLeft->RowCount = Form1->FileListLeft->RowCount - 1;
	}
	else { 										   // ���� ������� ������ ������
		iter = DequeFindRight.begin();             // (���������� ����� ������)
		Form1->FileListRight->RowCount = 2;

		while (iter!=DequeFindRight.end()) {
			StructFindTemp =* iter;

			if (DirAttribSet.count(StructFindTemp.dwFileAttributes)) {
				StrucFindToFileProp (StructFindTemp, FileProperty);
				Form1->FileListRight->Cells[ColName][i] = FileProperty.FileName;
				Form1->FileListRight->Cells[ColSize][i] = FileProperty.FileSizeKb;
				Form1->FileListRight->Cells[ColAttrib][i] = FileProperty.FileAttrib;
				Form1->FileListRight->Cells[ColCreTime][i] = FileProperty.CreateTime;
				Form1->FileListRight->Cells[ColAccTime][i] = FileProperty.AccessTime;
				Form1->FileListRight->Cells[ColWriTime][i] = FileProperty.WriteTime;
				Form1->FileListRight->RowCount = Form1->FileListRight->RowCount + 1;

			    i++;
			}
			iter++;
		}

		iter = DequeFindRight.begin();

		while (iter != DequeFindRight.end()) {
			StructFindTemp =* iter;

			if (DirAttribSet.count(StructFindTemp.dwFileAttributes) == 0) {
				StrucFindToFileProp (*iter, FileProperty);
				Form1->FileListRight->Cells[ColName][i] = FileProperty.FileName;
				Form1->FileListRight->Cells[ColSize][i] = FileProperty.FileSizeKb;
				Form1->FileListRight->Cells[ColAttrib][i] = FileProperty.FileAttrib;
				Form1->FileListRight->Cells[ColCreTime][i] = FileProperty.CreateTime;
				Form1->FileListRight->Cells[ColAccTime][i] = FileProperty.AccessTime;
				Form1->FileListRight->Cells[ColWriTime][i] = FileProperty.WriteTime;
				Form1->FileListRight->RowCount = Form1->FileListRight->RowCount + 1;
			    i++;
			}
			iter++;
		}

		if (i == 1) {
			Form1->FileListRight->Cells[ColName][i] = "\0";
			Form1->FileListRight->Cells[ColSize][i] = "\0";
			Form1->FileListRight->Cells[ColAttrib][i] = "\0";
			Form1->FileListRight->Cells[ColCreTime][i] = "\0";
			Form1->FileListRight->Cells[ColAccTime][i] = "\0";
			Form1->FileListRight->Cells[ColWriTime][i] = "\0";
		}

		if (i > 1)
			Form1->FileListRight->RowCount = Form1->FileListRight->RowCount - 1;
	}
}
//------------------------------------------------------------------------------

//���������� �������� ����� ��� �����
AnsiString HandlerOfOpen(AnsiString RootDir, int Panel, int DirSel) {
	const AnsiString Mask = "\\*.*\0";
	char PathChar[256];
	AnsiString Path;
	static AnsiString CurrentPathLeft = RootDir;
	static AnsiString CurrentPathRight = RootDir;
	long int Row = Form1->FileListLeft->Row;
	AnsiString Name = Form1->FileListLeft->Cells[ColName][Row];

	if (Panel == LEFT) {
		if (DirSel != 0){                                       // ���� ��������� �������� ����������
			switch (DirSel) {
				case 1:                                         // ���� ����� � ListDevices
					if (RootDir.AnsiCompare(RootDirRight) == 0) // ���� ��� ����� �������� �����������
					{                                           // ������������ ������ �� �� ����������, ���
						CurrentPathLeft = CurrentPathRight;     // � ������ ������,  �� ������������
						Path = CurrentPathLeft + Mask;          // ����� �������� � ��� ������
						strcpy(PathChar, Path.c_str());
						ViewDirectory(PathChar, LEFT);
						Form1->LabelCurrentPathLeft->Caption = CurrentPathLeft;
					}
					else {                   // ���� ������� ���������, �������� �� ��������� �� ������ ������
						CurrentPathLeft = RootDir + "\\";
						Path = RootDir + Mask;
						strcpy(PathChar, Path.c_str());         //������������� AnsiString � char
						ViewDirectory(PathChar, Panel);
						Form1->LabelCurrentPathLeft->Caption = CurrentPathLeft;
					}
					break;
				case 2:                                         //���� ������� �� bCurrentRootDir
					CurrentPathLeft = RootDir + "\\";
					Path = RootDir + Mask;
					strcpy(PathChar, Path.c_str());             //������������� AnsiString � char
					ViewDirectory(PathChar, Panel);
					Form1->LabelCurrentPathLeft->Caption = CurrentPathLeft;
					break;
			}
		}
		else {
			if ((Form1->FileListLeft->Cells[ColAttrib][Row] == "Dir\0") ||     //���� ������� ���� ��
				(Form1->FileListLeft->Cells[ColAttrib][Row] == "HideDir\0") || //����������
				(Form1->FileListLeft->Cells[ColAttrib][Row] == "HideSysDir\0"))
				if (Name == "..\0"){                            //���� ������� �� ���� ������� �����
					int Pos, Len = CurrentPathLeft.Length();
					for (int i = Len - 2; i >= 3; i--) {
						if (CurrentPathLeft[i] == '\\') {
							Pos = i;
							break;
						}
					}
					CurrentPathLeft.Delete(Pos, (Len - Pos));
					Path = CurrentPathLeft + Mask;
					strcpy(PathChar, Path.c_str());             //������������� AnsiString � char
					ViewDirectory(PathChar, LEFT);
					Form1->LabelCurrentPathLeft->Caption = CurrentPathLeft;
				}
				else {                                          //���� �������� �����������
					CurrentPathLeft += Name;
					Path = CurrentPathLeft + Mask;
					CurrentPathLeft += "\\";
					strcpy(PathChar, Path.c_str());             //������������� AnsiString � char
					Form1->LabelCurrentPathLeft->Caption = CurrentPathLeft;
					ViewDirectory(PathChar, LEFT);
				}
			else {                                              //���� �������� ����� (�� �����)
				if (Name != "\0") {
					AnsiString FilePath;
					FilePath = CurrentPathLeft + Name;
					strcpy(PathChar, FilePath.c_str());
					ShellExecuteA(0, "open", (LPCSTR)PathChar , "", "", 1);
				}
			}
			return CurrentPathLeft;
		}
    }
	else {                                                      //���� ������� ������ ������
		long int Row = Form1->FileListRight->Row;
		AnsiString Name = Form1->FileListRight->Cells[ColName][Row];

		if (DirSel != 0){//���� ��������� �������� ����������
			switch (DirSel) {
				case 1:
					if (RootDir.AnsiCompare(RootDirLeft) == 0)  // ���� ��� ����� �������� �����������
					{                                           // ������������ ������ �� �� ����������, ���
						CurrentPathRight = CurrentPathLeft;     // � ������ ������,  �� ������������
						Path = CurrentPathRight + Mask;         // ����� �������� � ��� ������
						strcpy(PathChar, Path.c_str());
						ViewDirectory(PathChar, RIGHT);
						Form1->LabelCurrentPathRight->Caption = CurrentPathRight;
					}
					else {
						CurrentPathRight = RootDir + '\\';
						Path = RootDir + Mask;
						strcpy(PathChar, Path.c_str());         //������������� AnsiString � char
						ViewDirectory(PathChar, Panel);
						Form1->LabelCurrentPathRight->Caption = CurrentPathRight;
					}
					break;
				case 2:
					CurrentPathRight = RootDir + '\\';
					Path = RootDir + Mask;
					strcpy(PathChar, Path.c_str());             // ������������� AnsiString � char
					ViewDirectory(PathChar, Panel);
					Form1->LabelCurrentPathRight->Caption = CurrentPathRight;
					break;
			}
		}
		else {
			if ((Form1->FileListRight->Cells[ColAttrib][Row] == "Dir\0") ||     // ���� ������� ���� ��
				(Form1->FileListRight->Cells[ColAttrib][Row] == "HideDir\0") || // �� ����������
				(Form1->FileListRight->Cells[ColAttrib][Row] == "HideSysDir\0"))
				if (Name == "..\0") {                            // ������� �� ���� ������� �����
					int Pos, Len = CurrentPathRight.Length();
					for (int i = Len - 2; i >= 3; i--) {
						if (CurrentPathRight[i] == '\\') {
							Pos = i;
							break;
						}
					}
					CurrentPathRight.Delete(Pos, (Len - Pos));
					Path = CurrentPathRight + Mask;
					strcpy(PathChar, Path.c_str()); 			 // ������������� AnsiString � char
					ViewDirectory(PathChar, RIGHT);
					Form1->LabelCurrentPathRight->Caption = CurrentPathRight;
				}
				else {                                           // �������� �����������
					CurrentPathRight += Name;
					Path = CurrentPathRight + Mask;
					CurrentPathRight += "\\";
					strcpy(PathChar, Path.c_str());              // ������������� AnsiString � char
					ViewDirectory(PathChar, RIGHT);
					Form1->LabelCurrentPathRight->Caption = CurrentPathRight;
				}
			else {
				if (Name != "\0") {                              // ���� ������� ���� �� ����� (�� �� �����)
					AnsiString FilePath;
					FilePath = CurrentPathRight + Name;
					strcpy(PathChar, FilePath.c_str());
					ShellExecuteA(0, "open", (LPCSTR)PathChar , "", "", 1);
				}
			}
		}
		return CurrentPathRight;
	}
}
//------------------------------------------------------------------------------
//�������� ��������
bool CreateFolder(int Panel) {
	const AnsiString Mask = "\\*.*\0";
	AnsiString Path;
	bool Result;
	char PathChar[256] = "\0";

	if(Panel == LEFT) {                             // ���� ������� ����� ������
		Path = CurrentPathLeft + FormDialogCreateDir->edNewDirName->Text;
		strcpy(PathChar, Path.c_str());             // ������������� AnsiString � char
		Result = CreateDirectoryA(PathChar, 0);
		if (Result == false) {
			MessageBoxA(0, "               ������� �� ������!\n      �������� ������������ ��� -\n                ��������� ����.",
				"MyCommander - ������!", 0);
		}
		else {
			Path = CurrentPathLeft + Mask;
			strcpy(PathChar, Path.c_str());         // ������������� AnsiString � char
			ViewDirectory(PathChar, Panel);
			if (CurrentPathLeft.AnsiCompare(CurrentPathRight) == 0)
				ViewDirectory(PathChar, RIGHT);
		}
	}
	else {                                          // ���� ������� ������ ������
		Path = CurrentPathRight + FormDialogCreateDir->edNewDirName->Text;
		strcpy(PathChar, Path.c_str());             // ������������� AnsiString � char
		Result = CreateDirectoryA(PathChar, 0);
		if (Result == false) {
			MessageBoxA(0, "               ������� �� ������!\n      �������� ������������ ��� -\n                ��������� ����.",
				"MyCommander - ������!", 0);
		}
		else {
			Path = CurrentPathRight + Mask;
			strcpy(PathChar, Path.c_str());         // ������������� AnsiString � char
			ViewDirectory(PathChar, Panel);
			if (CurrentPathRight.AnsiCompare(CurrentPathLeft) == 0)
				ViewDirectory(PathChar, LEFT);
	 	}
	}
	return Result;
}
//------------------------------------------------------------------------------

//��������
void Deleting(int Panel, int Operation) {
	const AnsiString Mask = "\\*.*\0";
	SHFILEOPSTRUCT StructOperation;
	AnsiString Name;
	char PathChar[256] = "\0";
	long int Row = Form1->FileListLeft->Row;
	AnsiString Path;

	StructOperation.hNameMappings = 0;
	StructOperation.lpszProgressTitle = 0;
	StructOperation.hwnd = 0;
	StructOperation.fAnyOperationsAborted = false;
	StructOperation.pTo = NULL;

	switch (Operation) {
		case FO_DELETE:
			StructOperation.wFunc = FO_DELETE;
			StructOperation.fFlags = 0;
			break;
		case FO_DELETER:
			StructOperation.wFunc = FO_DELETE;
			StructOperation.fFlags = FOF_ALLOWUNDO;
			break;
	}

	if (Panel == LEFT) {
		Row = Form1->FileListLeft->Row;
		Name = Form1->FileListLeft->Cells[ColName][Row];
		Path = CurrentPathLeft + Name;
		strcpy(PathChar, Path.c_str());           // ������������� AnsiString � char
		strcat(PathChar, "\0");
		StructOperation.pFrom = (PCZZTSTR)PathChar;
		SHFileOperation(&StructOperation);
		Path = CurrentPathLeft + Mask;
		strcpy(PathChar, Path.c_str());           // ������������� AnsiString � char
		ViewDirectory(PathChar, Panel);
		if (CurrentPathRight.AnsiCompare(CurrentPathLeft) == 0)
			ViewDirectory(PathChar, RIGHT);
	}
	else {
		Row = Form1->FileListRight->Row;
		Name = Form1->FileListRight->Cells[ColName][Row];
		Path = CurrentPathRight+Name;
		strcpy(PathChar, Path.c_str());           // ������������� AnsiString � char
		strcat(PathChar, "\0");
		StructOperation.pFrom = (PCZZTSTR)PathChar;
		SHFileOperation(&StructOperation);
		Path = CurrentPathRight + Mask;
		strcpy(PathChar, Path.c_str());           // ������������� AnsiString � char
		ViewDirectory(PathChar, Panel);
		if (CurrentPathRight.AnsiCompare(CurrentPathLeft) == 0)
			ViewDirectory(PathChar, LEFT);
	}
}
//------------------------------------------------------------------------------

//����������� ��� �����������
void CopyOrRemove(int Panel, int Operation) {
	const AnsiString Mask = "\\*.*\0";
	SHFILEOPSTRUCT StructOperation;
	AnsiString Name;
	char PathCharFrom[256] = "\0";
	char PathCharTo[256] = "\0";
	long int Row = Form1->FileListLeft->Row;
	AnsiString Path;

	if (CurrentPathRight.AnsiCompare(CurrentPathLeft) == 0)
		MessageBoxA(0, "  �������� � �������� �������� ���������!!!", "MyCommander - ������!", 0);
	else {
		StructOperation.hNameMappings = 0;
		StructOperation.lpszProgressTitle = 0;
		StructOperation.hwnd = 0;
		StructOperation.fAnyOperationsAborted = false;
		StructOperation.fFlags = 0;

		switch (Operation) {
			case FO_COPY:
				StructOperation.wFunc = FO_COPY;
				break;
			case FO_MOVE:
				StructOperation.wFunc=FO_MOVE;
				break;
		}

		if (Panel == LEFT) {
			Row = Form1->FileListLeft->Row;
			Name = Form1->FileListLeft->Cells[ColName][Row];
			Path = CurrentPathLeft+Name;
			strcpy(PathCharFrom, Path.c_str());           // ������������� AnsiString � char
			StructOperation.pFrom = (PCZZTSTR)PathCharFrom;
			strcpy(PathCharTo, CurrentPathRight.c_str()); // ������������� AnsiString � char
			StructOperation.pTo = (PCZZTSTR)PathCharTo;
			SHFileOperation(&StructOperation);
			Path = CurrentPathLeft + Mask;
			strcpy(PathCharTo, Path.c_str());             // ������������� AnsiString � char
			ViewDirectory(PathCharTo, Panel);
			Path = CurrentPathRight + Mask;
			strcpy(PathCharTo, Path.c_str());             // ������������� AnsiString � char
			ViewDirectory(PathCharTo, RIGHT);
		}
		else {
			Row = Form1->FileListRight->Row;
			Name = Form1->FileListRight->Cells[ColName][Row];
			Path = CurrentPathRight + Name;
			strcpy(PathCharFrom, Path.c_str());          // ������������� AnsiString � char
			StructOperation.pFrom = (PCZZTSTR)PathCharFrom;
			strcpy(PathCharTo, CurrentPathLeft.c_str()); // ������������� AnsiString � char
			StructOperation.pTo = (PCZZTSTR)PathCharTo;
			SHFileOperation(&StructOperation);
			Path = CurrentPathRight + Mask;
			strcpy(PathCharTo, Path.c_str());            // ������������� AnsiString � char
			ViewDirectory(PathCharTo, Panel);
			Path = CurrentPathLeft + Mask;
			strcpy(PathCharTo, Path.c_str());            // ������������� AnsiString � char
			ViewDirectory(PathCharTo, LEFT);
		}
	}
}
//------------------------------------------------------------------------------

// ��������������
void Formatting(AnsiString RootDir, int type) {
	HWND hwnd;
	SHFormatDrive(hwnd, 2, 0xFFFF, type);        // type = 0 - ����������� ����-�
}                                                // type = 1 - ������� ����-�
//------------------------------------------------------------------------------

// ����� �����
void Searching(AnsiString path, AnsiString name) {
	TSearchRec File;                                          // ��� FindFirst()

	if ((FindFirst(path + "*.*", faAnyFile, File)) == 0)
		do {
			if (File.Attr & faDirectory) {
				if ((File.Name != ".")&&(File.Name != "..")) {
						Searching(path + File.Name + "\\", name);
				}
			}
			if (File.Name == name) {
				ViewDirectory((char*)(path).c_str(), 1);
				Form1->FileListLeft->Cells[ColName][1] = File.Name;
				Form1->FileListLeft->Cells[ColSize][1] = File.Size;
				Form1->FileListLeft->Cells[ColAttrib][1] = File.Attr;
				Form1->FileListLeft->Cells[ColCreTime][1] = File.TimeStamp.FileDateToDateTime(File.Time);
				FindClose(File);
                break;
			}

		} while (FindNext(File) == 0);

	FindClose(File);
}
