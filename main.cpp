#include "inject.h"
#include "ExternalCompiler/ProcEnum.h"
#include "ExternalCompiler/WindowHook.h"


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

int main(int argc, char* argv[])
{
	string compiledProgramName = "CheatEngineDummy.exe";
	string directoryOfDLL = "E:\\VisualStudio\\projects\\DLL_redirConsole\\Release";
	string dllFileName = "DLL_redirConsole.dll";
	//string compiledProgramFullPath = ""; 
	// later we dont need compiledprogfullp cause we get it as a struct when we kill the process that got autostarted


	std::vector<std::string> all_args; // Store the arguments as a vector of strings
	string thisExePath = string(argv[0]); // get the current path to this file (including X.exe)
	if (argc > 1) {
		all_args.assign(argv + 1, argv + argc);
	}
	else
	{
		cout << "[i] No startup parameters provided." << endl;
	}

	cout << "[?] Please input the game's executable file name (ex: my_game.exe)" << endl;

	//cin >> compiledProgramName;


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
	processInfo* procInfo_autoStart = nullptr;
	while (procInfo_autoStart == nullptr)
	{

		Sleep(300);
		clear();
		procInfo_autoStart = getProgamProcInfo(compiledProgramName.c_str());
		cout << "[...] Waiting for compilation to finish and game to start..." << endl;
	}
	// Process is found
	clear();
	cout << "[!] Found autostarted game. " << endl;

	// Kill the autostarted game
	cout << "[!] Obtaining process handle with [PROCESS_ALL_ACCESS] rights. " << endl;
	HANDLE apProcHandle = OpenProcess(PROCESS_ALL_ACCESS, TRUE, procInfo_autoStart->pid);
	if (apProcHandle)
	{
		cout << "[!] Terminating autostarted game. " << endl;
		TerminateProcess(apProcHandle, 0);
	}
	else
	{
		cout << "[!] Could not terminate game: Process handle rejected. " << endl << "[i] Try restarting this program with Admin rights." << endl;
		return -1;
	}
	CloseHandle(apProcHandle);

	// Start the process manually with dll onload
	DWORD manualProcID = StartProcessWithDLL(directoryOfDLL, dllFileName, procInfo_autoStart->completeExePath);

	delete procInfo_autoStart; // delete as it is no longer used and up to date (the process is dead)
	std::cout << (manualProcID != NULL ? "[OK] Started executable with DLL attached." : "[!] Could not start and attach DLL.") << std::endl;

	// Maybe "wait for running game to finish?"
}