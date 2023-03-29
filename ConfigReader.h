#pragma once
#include "nlohmann/json.hpp"
#include <fstream>
#include <vector>
using json = nlohmann::json;
#define RUNNERCONFIG_FILE_NAME "runnerconfig"

typedef struct YYRunnerConfig {
	std::string CompiledExecutableName;
	std::string DebugRunnerDLLName;
	std::string DebugRunnderDLLDirectory;
	std::string otherArguments;

	YYRunnerConfig(std::string c, std::string dn, std::string dd, std::string oa)
	{
		CompiledExecutableName = c; DebugRunnerDLLName = dn; DebugRunnderDLLDirectory = dd; otherArguments = oa;
	}
	YYRunnerConfig()
	{
		 CompiledExecutableName = "";
		 DebugRunnerDLLName = "";
		 DebugRunnderDLLDirectory = "";
		 otherArguments = "";
	}
} YYRunnerConfig;

YYRunnerConfig readConfigFromDebugFile(std::string filepath)
{
	std::ifstream file(filepath);
	json jobj = json::parse(file);
	
	std::vector<std::string> keys = { "CompiledExecutableName", "DebugRunnerDLLName", "DebugRunnerDLLDirectory"};

	bool okay = true;
	for (std::string key : keys)
	{
		if (!jobj.contains(key))
		{
			okay = false;
			//cout << "could not find key " << key << endl;
		}
	}

	if (!okay)
	{
		//err handling
		return YYRunnerConfig("", "", "", "");
	}
	std::string oargs = "";
	if (jobj.contains("OtherArguments"))
	{
		oargs = jobj["OtherArguments"];
	}

	return YYRunnerConfig(jobj["CompiledExecutableName"], jobj["DebugRunnerDLLName"], jobj["DebugRunnerDLLDirectory"],oargs );
}


bool isValidConfig(YYRunnerConfig c)
{
	if (c.CompiledExecutableName == "" or c.DebugRunnderDLLDirectory == "" or c.DebugRunnerDLLName == "")
	{
		return false;
	}
	return true;
}