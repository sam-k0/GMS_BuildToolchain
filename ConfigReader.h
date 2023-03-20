#pragma once
#include "nlohmann/json.hpp"
#include <fstream>
#include <vector>
using json = nlohmann::json;

typedef struct YYRunnerConfig {
	std::string CompiledExecutableName;
	std::string DebugRunnerDLLName;
	std::string DebugRunnderDLLDirectory;

	YYRunnerConfig(std::string c, std::string dn, std::string dd)
	{
		CompiledExecutableName = c; DebugRunnerDLLName = dn; DebugRunnderDLLDirectory = dd;
	}
	YYRunnerConfig()
	{
		 CompiledExecutableName = "";
		 DebugRunnerDLLName = "";
		 DebugRunnderDLLDirectory = "";
	}
} YYRunnerConfig;

YYRunnerConfig readConfigFromDebugFile(std::string filepath)
{
	std::ifstream file(filepath);
	json jobj = json::parse(file);
	
	std::vector<std::string> keys = { "CompiledExecutableName", "DebugRunnerDLLName", "DebugRunnerDLLDirectory" };

	bool okay = true;
	for (std::string key : keys)
	{
		if (!jobj.contains(key))
		{
			okay = false;
		}
	}

	if (!okay)
	{
		//err handling
		return YYRunnerConfig("", "", "");
	}
	return YYRunnerConfig(jobj["CompiledExecutableName"], jobj["DebugRunnerDLLName"], jobj["DebugRunnerDLLDirectory"]);
}


bool isValidConfig(YYRunnerConfig c)
{
	if (c.CompiledExecutableName == "" or c.DebugRunnderDLLDirectory == "" or c.DebugRunnerDLLName == "")
	{
		return false;
	}
	return true;
}