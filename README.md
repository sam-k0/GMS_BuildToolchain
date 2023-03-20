# GMS_BuildToolchain

Allows for building and running games from GameMaker in a command line window.

`Only tested in GameMaker: Studio 1.4.X`

## Usage

- You must have GMS open with your project loaded.

There are 2 ways:
1. Running the executable and entering the name of the built game executable in runtime
2. Creating a 'runnerconfig' file

## Runnerconfig files
These files contain the name of the compiled executable, the debug DLL directory and filename.<br>
Create a new file and call it `runnerconfig`<br>
Structure is like this:
```json
{
    "CompiledExecutableName":"CheatEngineDummy.exe",
    "DebugRunnerDLLName":"DLL_redirConsole.dll",
    "DebugRunnerDLLDirectory":"E:\\VisualStudio\\projects\\ProcRedirStdout\\Release"
}
```
It makes sense to have one file per project, in your GM-Project directory.<br>
To use it, drag and drop the `runnerconfig` onto the executable.
<br>
You should see GameMaker Studio compiling and running the game.