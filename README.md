# GMS_BuildToolchain

Allows for building and running games from GameMaker in a command line window.

`Only tested in GameMaker: Studio 1.4.X`
## Attaching to compiled games
Result will look like this:
- you can capture the game's debug console output
<img src="https://user-images.githubusercontent.com/56673835/227980754-0238cf79-cecc-485a-856d-1b251f9c4bdf.png" width="50%" height="50%">

<img src="https://user-images.githubusercontent.com/56673835/227980709-4306edca-faf1-4c11-8c0c-7d6553b7a05f.png" width="50%" height="50%">

## Usage

- For debugging existing executables: Start the game, then the toolchain executable
- For debugging and running games with GameMaker IDE: Make sure the IDE is running, then start the toolchain exec.

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
