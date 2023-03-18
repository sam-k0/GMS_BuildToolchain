#include "inject.h"
#include "ExternalCompiler/ProcEnum.h"
#include "ExternalCompiler/WindowHook.h"
#include "helper.h"

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
		cout << "Waiting for Process to start..." << endl;
	}
	// Process is found
	clear();
	cout << "Found autostarted game" << endl;



// Inject the debugging DLL (Maybe even load with DLL)
	
}