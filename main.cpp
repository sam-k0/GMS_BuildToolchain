#include "inject.h"
#include "ExternalCompiler/ProcEnum.h"
#include "ExternalCompiler/WindowHook.h"
#include "helper.h"
#include "detours.h"

processInfo* IDE = NULL;


void compile()
{
	// Get a reference to the GMS IDE
	IDE = getProgamProcInfo(IDEname);
	// Check if struct is valid
	bool valid = is_valid(IDE);
	std::cout << (valid == true ? "[OK] Struct seems valid." : "[!] Struct is invalid.") << std::endl;

	if (valid) {
		std::cout << (sendCompileSignal(IDE) == true ? "[OK] Success on sending signal." : "[!] Could not send signal.") << std::endl;
	}

}

int main()
{
	cout << "[?] Please input the game's executable file name (ex: my_game.exe)" << endl;
	string compiledProgramName;
	cin >> compiledProgramName;


/// Get a hook to the GMS window
	IDE = getProgamProcInfo(IDEname);
// Check if struct is valid
	bool valid = is_valid(IDE);
	std::cout << (valid == true ? "[OK] Struct seems valid." : "[!] Struct is invalid.") << std::endl;
// Trigger compilation
	if (valid) {
		std::cout << (sendCompileSignal(IDE) == true ? "[OK] Success on sending signal." : "[!] Could not send signal.") << std::endl;
	}
// Run the generated executable
	processInfo* pI_waitfor = nullptr;
	while (pI_waitfor == nullptr)
	{
		
		Sleep(300);
		clear();
		pI_waitfor = getProgamProcInfo(compiledProgramName.c_str());
		cout << "[...] Waiting for compilation to finish and game to start..." << endl;
	}
	// Process is found
	clear();
	cout << "[!] Found autostarted game. " << endl;

// Kill the autostarted game
	cout << "[!] Obtaining process handle with [PROCESS_ALL_ACCESS] rights. " << endl;
	HANDLE apProcHandle = OpenProcess(PROCESS_ALL_ACCESS, TRUE, pI_waitfor->pid);
	if (apProcHandle)
	{
		cout << "[!] Terminating autostarted game. " << endl;
		TerminateProcess(apProcHandle, 0);
	}
	else
	{
		cout << "[!] Could not terminate game: Process handle rejected. "<<endl << "[i] Try restarting this program with Admin rights." << endl;
		return -1;
	}
	

	delete pI_waitfor;

// Inject the debugging DLL (Maybe even load with DLL)
	
}