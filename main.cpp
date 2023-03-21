#include "inject.h"
#include "ExternalCompiler/ProcEnum.h"
#include "ExternalCompiler/WindowHook.h"
#include "ConfigReader.h"

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
	YYRunnerConfig RunnerConfig;

	bool CompileAgain = true;
	bool RunAgain = true;
	bool doNotHide = false;

	std::vector<std::string> all_args; // Store the arguments as a vector of strings
	string thisExePath = string(argv[0]); // get the current path to this file (including X.exe)
	if (argc > 1) {
		all_args.assign(argv + 1, argv + argc);
	}
	else
	{
		cout << "[i] No startup parameters provided." << endl;
	}

/// Read the configfile
	// Check if it was passed as an argument (1)
	if (all_args.size() != 0)
	{
		if(endsWith(all_args.at(0), "runnerconfig"))
		{ 
			if (fileExists(all_args.at(0)))
			{
				cout << "[i] Reading file from: " << all_args.at(0) << endl;
				RunnerConfig = readConfigFromDebugFile(all_args.at(0));
				if (!isValidConfig(RunnerConfig))
				{
					cerr << "[!] There's an error in the configuration file. Please regenerate the configuration file and restart the program" << endl;
					getchar();
					exit(-1);
				}
			}
		}
		else
		if (std::find(all_args.begin(), all_args.end(), "-show") != all_args.end()) // contains -show flag
		{
			doNotHide = true;
		}
	}
	else {
		cout << "[?] Please input the game's executable file name (ex: my_game.exe)" << endl;
		cin >> compiledProgramName;
		cout << "[!] WARNING: This method of using the toolchain requires the DebugDLL to be in the same directory as this launcher." << endl;
		directoryOfDLL = getCurrentDir();

		cout << directoryOfDLL + "\\" + dllFileName << endl;

		if (!fileExists(directoryOfDLL + "\\" + dllFileName))
		{
			cerr << "[!] The DLL fil could not be found at path: " << directoryOfDLL + "\\" + dllFileName << endl;
			getchar();
			exit(-1);
		}
	}

	
	while (CompileAgain) // Main program loop
	{
		// Reset flags
		CompileAgain = true;
		RunAgain = true;
		if (IDE) // Get a new hook to the IDE window if the old one exists
		{
			delete IDE;
		}
		
		IDE = getProgamProcInfo(IDEname); /// Get a hook to the GMS window
		
		bool valid = is_valid(IDE);// Check if struct is valid
		std::cout << (valid == true ? "[OK] Struct seems valid." : "[!] Struct is invalid.") << std::endl;
		// Trigger compilation
		if (valid) {
			std::cout << (sendCompileSignal(IDE) == true ? "[OK] Success on sending signal." : "[!] Could not send signal.") << std::endl;
		}
		else
		{
			cerr << "[!] The IDE does not seem to be running." << endl;
			//getchar();
			//exit(-1);
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
		
		string compiledProgramPath = procInfo_autoStart->completeExePath;
		delete procInfo_autoStart; // delete as it is no longer used and up to date (the process is dead)
		
		while (RunAgain)
		{
			StartProcessData manualProcessData = StartProcessWithDLL(directoryOfDLL, dllFileName, compiledProgramPath);

			std::cout << (manualProcessData.pid != NULL ? "[OK] Started executable with DLL attached." : "[!] Could not start and attach DLL.") << std::endl;

			if (manualProcessData.hProc == NULL)
			{
				// failed to launch...
				continue;
			}

			cout << "[i] Waiting for the game to finish." << endl;
			WaitForSingleObject(manualProcessData.hProc, INFINITE);
			/// Ask to run again?
			clear(); // clear console
			cout << "[?] >> Enter a command: [r]un again, [c]ompile again, [q]uit" << endl;
			char c;
			cin >> c;
			// evaluate cmd
			switch (c)
			{
			case 'r': // rerun
				RunAgain = true;
				continue;
				break;

			case 'q': //quit
				RunAgain = false;
				CompileAgain = false;
				break;

			case 'c': // compile
				RunAgain = false;
				CompileAgain = true;
				break;
			}
		}
	}
}