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
int DirSel = 0;                                // Признак совершения выбора в ListDevices
AnsiString RootDirLeft, RootDirRight;          // Текущая корневая директория
AnsiString CurrentPathLeft, CurrentPathRight;  // Текущий путь
int Panel = LEFT;

struct _FileProperty{                 //Структура вывода

	char    FileAttrib[12];           // атрибуты файла
	char    CreateTime[25];           // время создания файла
	char    AccessTime[25];           // время доступа
	char    WriteTime[25];            // время записи
	char    FileSizeBy[21];           // размер файла в Б
	char    FileSizeKb[18];           // размер файла в КБ
	TCHAR   FileName[MAX_PATH];       // имя файла
	TCHAR   AlterFileName[14];        // альтернативное имя файла
}FileProperty;

deque<WIN32_FIND_DATAA> DequeFindLeft; //Очередь результатов поиска
deque<WIN32_FIND_DATAA> DequeFindRight;

WIN32_FIND_DATAA StructFind;           //Структура результатов поиска

int ColName = 0,                       //Столбцы таблицы вывода
	ColSize = 1,
	ColAttrib = 2,
	ColCreTime = 5,
	ColAccTime = 4,
	ColWriTime = 3;

//Множество атрибутов директории
set<unsigned long, less<unsigned long> > DirAttribSet;
void CreateDirAttribSet(){            //Создание множества атрибутов директории
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
//Создание списка доступных устройств
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

//Преобразование из структуры результатов поиска в структуру вывода
void StrucFindToFileProp(WIN32_FIND_DATAA StructFind, _FileProperty &FileProperty) {
	//Преобразование имени
	strcpy((char*)FileProperty.FileName, (const char *)StructFind.cFileName);
	strcpy((char*)FileProperty.AlterFileName, (const char *)StructFind.cAlternateFileName);
	char  Temp[10];

	//Преобразование атрибутов
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

	//Преобразование размера
	if (StructFind.nFileSizeHigh != 0){                   // Если старшее слово не равно нулю
		char TempStr[20];
		strcpy(FileProperty.FileSizeBy, ultoa(StructFind.nFileSizeHigh, TempStr, 10));
		strcat(FileProperty.FileSizeBy, ultoa(StructFind.nFileSizeLow, TempStr, 10));
		strcat(FileProperty.FileSizeBy, "\0");
		strcpy(FileProperty.FileSizeKb, ">4ГБ\0");
	}
	else {
		char TempStr[20];                                 // Если старшее слово равно нулю
		strcpy(FileProperty.FileSizeBy, ultoa(StructFind.nFileSizeLow, TempStr, 10));
		strcat(FileProperty.FileSizeBy, "\0");
		strcpy(FileProperty.FileSizeKb, ultoa((StructFind.nFileSizeLow>>10), TempStr, 10));
		strcat(FileProperty.FileSizeKb, "\0");
	}

	if (DirAttribSet.count(StructFind.dwFileAttributes)){ // Если папка, то ее размер
		strcpy(FileProperty.FileSizeBy, "DIR\0");         // не отображается
	 	strcpy(FileProperty.FileSizeKb, "DIR\0");
	}
	else if ((StructFind.nFileSizeLow>>10) == 0){         // Если размер файла меньше килобайта,
		strcpy(FileProperty.FileSizeKb, FileProperty.FileSizeBy); // то отображается его размер в байтах
		strcat(FileProperty.FileSizeKb, " Б   ");
	}

	//Преобрзование времени...
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

//Сканирование содержимого каталога
void ScanDir(const char *Path, int Panel) {
	HANDLE HWNDFind;
	bool Flag = true;

	if (Panel == LEFT) {                            // Если активна левая панель
		if (!DequeFindLeft.empty())
			DequeFindLeft.clear();                  // Очистка текущей очереди
		HWNDFind = FindFirstFileA(Path, &StructFind);
		while (Flag) {
			Flag = FindNextFileA(HWNDFind, &StructFind);
			if (Flag)
				DequeFindLeft.push_back(StructFind);
		}
		FindClose(HWNDFind);
	}
	else {                                          // Если активна правая панель
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

//Отображение содержимого каталога
void ViewDirectory(const char *Path, int Panel) {
	ScanDir(Path, Panel);
	deque<WIN32_FIND_DATAA>::iterator iter;
	WIN32_FIND_DATAA StructFindTemp;

	int i = 0;                                 // Заполнение заголовков столбцов
	Form1->FileListLeft->Cells[ColName][i] = "Имя";
	Form1->FileListLeft->Cells[ColSize][i] = "Размер КБ";
	Form1->FileListLeft->Cells[ColAttrib][i] = "Атрибуты";
	Form1->FileListLeft->Cells[ColCreTime][i] = "Создан";
	Form1->FileListLeft->Cells[ColAccTime][i] = "Открыт";
	Form1->FileListLeft->Cells[ColWriTime][i] = "Изменен";

	Form1->FileListRight->Cells[ColName][i] = "Имя";
	Form1->FileListRight->Cells[ColSize][i] = "Размер КБ";
	Form1->FileListRight->Cells[ColAttrib][i] = "Атрибуты";
	Form1->FileListRight->Cells[ColCreTime][i] = "Создан";
	Form1->FileListRight->Cells[ColAccTime][i] = "Открыт";
	Form1->FileListRight->Cells[ColWriTime][i] = "Изменен";
	i++;

	if (Panel == LEFT){                        // Если активна левая панель
		iter = DequeFindLeft.begin();
		Form1->FileListLeft->RowCount = 2;     // Очистка содержимого таблицы,
											   // куда производится отображение
		while (iter != DequeFindLeft.end()) {
			StructFindTemp =* iter;

			if (DirAttribSet.count(StructFindTemp.dwFileAttributes)){// Отображение директорий
				StrucFindToFileProp(StructFindTemp, FileProperty);    // (в таблице выше файлов)
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
		while (iter != DequeFindLeft.end()){ // Отображение файлов (в таблице после директорий)
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

		if (i == 1){                 // Если нет файлов и паппок для отображения
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
	else { 										   // Если активна правая панель
		iter = DequeFindRight.begin();             // (аналогично левой панели)
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

//Обработчик открытия папки или файла
AnsiString HandlerOfOpen(AnsiString RootDir, int Panel, int DirSel) {
	const AnsiString Mask = "\\*.*\0";
	char PathChar[256];
	AnsiString Path;
	static AnsiString CurrentPathLeft = RootDir;
	static AnsiString CurrentPathRight = RootDir;
	long int Row = Form1->FileListLeft->Row;
	AnsiString Name = Form1->FileListLeft->Cells[ColName][Row];

	if (Panel == LEFT) {
		if (DirSel != 0){                                       // Если сменилась корневая директория
			switch (DirSel) {
				case 1:                                         // Если выбор в ListDevices
					if (RootDir.AnsiCompare(RootDirRight) == 0) // Если при смене корневой дирректории
					{                                           // пользователь выбрал то же устройство, что
						CurrentPathLeft = CurrentPathRight;     // в другой панели,  то отображеется
						Path = CurrentPathLeft + Mask;          // папка открытая в той панели
						strcpy(PathChar, Path.c_str());
						ViewDirectory(PathChar, LEFT);
						Form1->LabelCurrentPathLeft->Caption = CurrentPathLeft;
					}
					else {                   // Если выбрано устройсво, отличное от устройсва во второй панели
						CurrentPathLeft = RootDir + "\\";
						Path = RootDir + Mask;
						strcpy(PathChar, Path.c_str());         //Преобрзование AnsiString в char
						ViewDirectory(PathChar, Panel);
						Form1->LabelCurrentPathLeft->Caption = CurrentPathLeft;
					}
					break;
				case 2:                                         //Если нажатие на bCurrentRootDir
					CurrentPathLeft = RootDir + "\\";
					Path = RootDir + Mask;
					strcpy(PathChar, Path.c_str());             //Преобрзование AnsiString в char
					ViewDirectory(PathChar, Panel);
					Form1->LabelCurrentPathLeft->Caption = CurrentPathLeft;
					break;
			}
		}
		else {
			if ((Form1->FileListLeft->Cells[ColAttrib][Row] == "Dir\0") ||     //Если двойной клик по
				(Form1->FileListLeft->Cells[ColAttrib][Row] == "HideDir\0") || //директории
				(Form1->FileListLeft->Cells[ColAttrib][Row] == "HideSysDir\0"))
				if (Name == "..\0"){                            //Если переход на один уровень вверх
					int Pos, Len = CurrentPathLeft.Length();
					for (int i = Len - 2; i >= 3; i--) {
						if (CurrentPathLeft[i] == '\\') {
							Pos = i;
							break;
						}
					}
					CurrentPathLeft.Delete(Pos, (Len - Pos));
					Path = CurrentPathLeft + Mask;
					strcpy(PathChar, Path.c_str());             //Преобрзование AnsiString в char
					ViewDirectory(PathChar, LEFT);
					Form1->LabelCurrentPathLeft->Caption = CurrentPathLeft;
				}
				else {                                          //Если открытие подкаталога
					CurrentPathLeft += Name;
					Path = CurrentPathLeft + Mask;
					CurrentPathLeft += "\\";
					strcpy(PathChar, Path.c_str());             //Преобрзование AnsiString в char
					Form1->LabelCurrentPathLeft->Caption = CurrentPathLeft;
					ViewDirectory(PathChar, LEFT);
				}
			else {                                              //Если открытие файла (не папки)
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
	else {                                                      //Если активна правая панель
		long int Row = Form1->FileListRight->Row;
		AnsiString Name = Form1->FileListRight->Cells[ColName][Row];

		if (DirSel != 0){//Если сменилась корневая директория
			switch (DirSel) {
				case 1:
					if (RootDir.AnsiCompare(RootDirLeft) == 0)  // Если при смене корневой дирректории
					{                                           // пользователь выбрал то же устройство, что
						CurrentPathRight = CurrentPathLeft;     // в другой панели,  то отображеется
						Path = CurrentPathRight + Mask;         // папка открытая в той панели
						strcpy(PathChar, Path.c_str());
						ViewDirectory(PathChar, RIGHT);
						Form1->LabelCurrentPathRight->Caption = CurrentPathRight;
					}
					else {
						CurrentPathRight = RootDir + '\\';
						Path = RootDir + Mask;
						strcpy(PathChar, Path.c_str());         //Преобрзование AnsiString в char
						ViewDirectory(PathChar, Panel);
						Form1->LabelCurrentPathRight->Caption = CurrentPathRight;
					}
					break;
				case 2:
					CurrentPathRight = RootDir + '\\';
					Path = RootDir + Mask;
					strcpy(PathChar, Path.c_str());             // Преобрзование AnsiString в char
					ViewDirectory(PathChar, Panel);
					Form1->LabelCurrentPathRight->Caption = CurrentPathRight;
					break;
			}
		}
		else {
			if ((Form1->FileListRight->Cells[ColAttrib][Row] == "Dir\0") ||     // Если двойной клик по
				(Form1->FileListRight->Cells[ColAttrib][Row] == "HideDir\0") || // по директории
				(Form1->FileListRight->Cells[ColAttrib][Row] == "HideSysDir\0"))
				if (Name == "..\0") {                            // Переход на один уровень вверх
					int Pos, Len = CurrentPathRight.Length();
					for (int i = Len - 2; i >= 3; i--) {
						if (CurrentPathRight[i] == '\\') {
							Pos = i;
							break;
						}
					}
					CurrentPathRight.Delete(Pos, (Len - Pos));
					Path = CurrentPathRight + Mask;
					strcpy(PathChar, Path.c_str()); 			 // Преобрзование AnsiString в char
					ViewDirectory(PathChar, RIGHT);
					Form1->LabelCurrentPathRight->Caption = CurrentPathRight;
				}
				else {                                           // Открытие подкаталога
					CurrentPathRight += Name;
					Path = CurrentPathRight + Mask;
					CurrentPathRight += "\\";
					strcpy(PathChar, Path.c_str());              // Преобрзование AnsiString в char
					ViewDirectory(PathChar, RIGHT);
					Form1->LabelCurrentPathRight->Caption = CurrentPathRight;
				}
			else {
				if (Name != "\0") {                              // Если двойной клик по файлу (не по папке)
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
//Создание каталога
bool CreateFolder(int Panel) {
	const AnsiString Mask = "\\*.*\0";
	AnsiString Path;
	bool Result;
	char PathChar[256] = "\0";

	if(Panel == LEFT) {                             // Если активна левая панель
		Path = CurrentPathLeft + FormDialogCreateDir->edNewDirName->Text;
		strcpy(PathChar, Path.c_str());             // Преобрзование AnsiString в char
		Result = CreateDirectoryA(PathChar, 0);
		if (Result == false) {
			MessageBoxA(0, "               Каталог не создан!\n      Возможно неправильное имя -\n                повторите ввод.",
				"MyCommander - Ошибка!", 0);
		}
		else {
			Path = CurrentPathLeft + Mask;
			strcpy(PathChar, Path.c_str());         // Преобрзование AnsiString в char
			ViewDirectory(PathChar, Panel);
			if (CurrentPathLeft.AnsiCompare(CurrentPathRight) == 0)
				ViewDirectory(PathChar, RIGHT);
		}
	}
	else {                                          // Если активна правая панель
		Path = CurrentPathRight + FormDialogCreateDir->edNewDirName->Text;
		strcpy(PathChar, Path.c_str());             // Преобрзование AnsiString в char
		Result = CreateDirectoryA(PathChar, 0);
		if (Result == false) {
			MessageBoxA(0, "               Каталог не создан!\n      Возможно неправильное имя -\n                повторите ввод.",
				"MyCommander - Ошибка!", 0);
		}
		else {
			Path = CurrentPathRight + Mask;
			strcpy(PathChar, Path.c_str());         // Преобрзование AnsiString в char
			ViewDirectory(PathChar, Panel);
			if (CurrentPathRight.AnsiCompare(CurrentPathLeft) == 0)
				ViewDirectory(PathChar, LEFT);
	 	}
	}
	return Result;
}
//------------------------------------------------------------------------------

//Удаление
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
		strcpy(PathChar, Path.c_str());           // Преобрзование AnsiString в char
		strcat(PathChar, "\0");
		StructOperation.pFrom = (PCZZTSTR)PathChar;
		SHFileOperation(&StructOperation);
		Path = CurrentPathLeft + Mask;
		strcpy(PathChar, Path.c_str());           // Преобрзование AnsiString в char
		ViewDirectory(PathChar, Panel);
		if (CurrentPathRight.AnsiCompare(CurrentPathLeft) == 0)
			ViewDirectory(PathChar, RIGHT);
	}
	else {
		Row = Form1->FileListRight->Row;
		Name = Form1->FileListRight->Cells[ColName][Row];
		Path = CurrentPathRight+Name;
		strcpy(PathChar, Path.c_str());           // Преобрзование AnsiString в char
		strcat(PathChar, "\0");
		StructOperation.pFrom = (PCZZTSTR)PathChar;
		SHFileOperation(&StructOperation);
		Path = CurrentPathRight + Mask;
		strcpy(PathChar, Path.c_str());           // Преобрзование AnsiString в char
		ViewDirectory(PathChar, Panel);
		if (CurrentPathRight.AnsiCompare(CurrentPathLeft) == 0)
			ViewDirectory(PathChar, LEFT);
	}
}
//------------------------------------------------------------------------------

//Копирование или перемещение
void CopyOrRemove(int Panel, int Operation) {
	const AnsiString Mask = "\\*.*\0";
	SHFILEOPSTRUCT StructOperation;
	AnsiString Name;
	char PathCharFrom[256] = "\0";
	char PathCharTo[256] = "\0";
	long int Row = Form1->FileListLeft->Row;
	AnsiString Path;

	if (CurrentPathRight.AnsiCompare(CurrentPathLeft) == 0)
		MessageBoxA(0, "  Конечный и исходный каталоги совпадают!!!", "MyCommander - Ошибка!", 0);
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
			strcpy(PathCharFrom, Path.c_str());           // Преобрзование AnsiString в char
			StructOperation.pFrom = (PCZZTSTR)PathCharFrom;
			strcpy(PathCharTo, CurrentPathRight.c_str()); // Преобрзование AnsiString в char
			StructOperation.pTo = (PCZZTSTR)PathCharTo;
			SHFileOperation(&StructOperation);
			Path = CurrentPathLeft + Mask;
			strcpy(PathCharTo, Path.c_str());             // Преобрзование AnsiString в char
			ViewDirectory(PathCharTo, Panel);
			Path = CurrentPathRight + Mask;
			strcpy(PathCharTo, Path.c_str());             // Преобрзование AnsiString в char
			ViewDirectory(PathCharTo, RIGHT);
		}
		else {
			Row = Form1->FileListRight->Row;
			Name = Form1->FileListRight->Cells[ColName][Row];
			Path = CurrentPathRight + Name;
			strcpy(PathCharFrom, Path.c_str());          // Преобрзование AnsiString в char
			StructOperation.pFrom = (PCZZTSTR)PathCharFrom;
			strcpy(PathCharTo, CurrentPathLeft.c_str()); // Преобрзование AnsiString в char
			StructOperation.pTo = (PCZZTSTR)PathCharTo;
			SHFileOperation(&StructOperation);
			Path = CurrentPathRight + Mask;
			strcpy(PathCharTo, Path.c_str());            // Преобрзование AnsiString в char
			ViewDirectory(PathCharTo, Panel);
			Path = CurrentPathLeft + Mask;
			strcpy(PathCharTo, Path.c_str());            // Преобрзование AnsiString в char
			ViewDirectory(PathCharTo, LEFT);
		}
	}
}
//------------------------------------------------------------------------------

// Форматирование
void Formatting(AnsiString RootDir, int type) {
	HWND hwnd;
	SHFormatDrive(hwnd, 2, 0xFFFF, type);        // type = 0 - стандартное форм-е
}                                                // type = 1 - быстрое форм-е
//------------------------------------------------------------------------------

// Поиск файла
void Searching(AnsiString path, AnsiString name) {
	TSearchRec File;                                          // для FindFirst()

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
