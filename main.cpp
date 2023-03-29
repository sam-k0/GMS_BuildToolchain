#include "inject.h"
#include "ExternalCompiler/ProcEnum.h"
#include "ExternalCompiler/WindowHook.h"
#include "ConfigReader.h"

processInfo* IDE = NULL;
//string compiledProgramName = "";
//string directoryOfDLL = "";
//string dllFileName = "";
YYRunnerConfig RunnerConfig;

void setConfigManually(string otherArgs)
{
	cout << "[?] Please input the game's executable file name (ex: my_game.exe)" << endl;
	std::getline(std::cin, RunnerConfig.CompiledExecutableName);
	cout << "[!] WARNING: This method of using the toolchain requires the DebugDLL to be in the same directory as this launcher." << endl;
	RunnerConfig.DebugRunnderDLLDirectory = getCurrentDir();
	RunnerConfig.DebugRunnerDLLName = "DLL_redirConsole.dll";
	cout << "[!] Expecting the DLL to be at: " << RunnerConfig.DebugRunnderDLLDirectory + "\\" + RunnerConfig.DebugRunnerDLLName << endl;

	if (!fileExists(RunnerConfig.DebugRunnderDLLDirectory + "\\" + RunnerConfig.DebugRunnerDLLName))
	{
		cerr << "[!] The DLL fil could not be found at path: " << RunnerConfig.DebugRunnderDLLDirectory + "\\" + RunnerConfig.DebugRunnerDLLName << endl;
		getchar();
		exit(-1);
	}

	RunnerConfig.otherArguments = otherArgs;
}

void loadConfig(string configpath)
{
	
	if (fileExists(configpath) && configpath != "")
	{
		cout << "[i] Reading file from: " << configpath << endl;
		RunnerConfig = readConfigFromDebugFile(configpath);
		if (!isValidConfig(RunnerConfig))
		{
			cerr << "[!] There's an error in the configuration file. Please regenerate the configuration file and restart the program" << endl;
			getchar();
			exit(-1);
		}
	}
}

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
	bool CompileAgain = true;
	bool RunAgain = true;

	// args to vector
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
	// Check if it was passed as an argument
	if (all_args.size() != 0)
	{
		string amogu = vector_find_substring(all_args, RUNNERCONFIG_FILE_NAME );
		cout << amogu << endl;
		loadConfig(amogu);
	}
	
	if (!isValidConfig(RunnerConfig))
	{
		cout << "[!] Config in file 'runnerconfig' invalid! (Does it exist?)" << endl;
		setConfigManually(Join(all_args));
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
			procInfo_autoStart = getProgamProcInfo(RunnerConfig.CompiledExecutableName.c_str());
			cout << "[...] Waiting for compilation to finish and game ("<< RunnerConfig.CompiledExecutableName <<")to start..." << endl;
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
			StartProcessData manualProcessData = StartProcessWithDLL(RunnerConfig.DebugRunnderDLLDirectory, RunnerConfig.DebugRunnerDLLName, compiledProgramPath, RunnerConfig.otherArguments);

			std::cout << (manualProcessData.pid != NULL ? "[OK] Started executable with DLL attached." : "[!] Could not start and attach DLL.") << std::endl;

			if (manualProcessData.hProc == NULL)
			{
				// failed to launch...
				cout << "Failed to launch executable. Check the DLL's presence in the folder: " << RunnerConfig.DebugRunnderDLLDirectory << endl;
				getchar();
				exit(-1);
			}

			cout << "[i] Waiting for the game to finish." << endl;
			WaitForSingleObject(manualProcessData.hProc, INFINITE);
			/// Ask to run again?
			cout << "--------" << endl;
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