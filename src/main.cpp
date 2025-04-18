#include "LawnApp.h"
#include "Resources.h"
#include "Sexy.TodLib/TodStringFile.h"

using namespace Sexy;

bool (*gAppCloseRequest)();				
bool (*gAppHasUsedCheatKeys)();			
SexyString (*gGetCurrentLevelName)();

int main( int argc, char *argv[] )
{
#ifdef _WIN32
	HWND hwnd = GetConsoleWindow();
	if ( hwnd != nullptr )
		ShowWindow( hwnd, SW_HIDE );
#endif

#ifdef _DEBUG
	FILE* fDummy;
	freopen_s(&fDummy, "CONIN$", "r", stdin);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);
	freopen_s(&fDummy, "CONOUT$", "w", stdout);
#endif

	TodStringListSetColors(gLawnStringFormats, gLawnStringFormatCount);
	gGetCurrentLevelName = LawnGetCurrentLevelName;
	gAppCloseRequest = LawnGetCloseRequest;
	gAppHasUsedCheatKeys = LawnHasUsedCheatKeys;

	gLawnApp = new LawnApp();
	gLawnApp->mChangeDirTo = (!Sexy::FileExists("properties\\resources.xml") && Sexy::FileExists("..\\properties\\resources.xml")) ? ".." : ".";
	
	gLawnApp->Init();
	gLawnApp->Start();

	gLawnApp->Shutdown();
	if (gLawnApp)
		delete gLawnApp;

	return 0;
};