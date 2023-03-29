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
`
## Without using a runnerconfig file
Not recommended but possible and probably still useful for testing. <br>
If you are not using a `runnerconfig` file to launch the Tool, you can pass startup arguments like the ones mentioned below using the command line.

## Runnerconfig files
These files contain the name of the compiled executable, the debug DLL directory and filename.<br>
Create a new file and call it `runnerconfig`<br>
It's essentially a json file with config information for the Toolchain executable to process. <br>
Valid entries are:
- CompiledExecutableName: The name of the executable file of your game
- DebugRunnerDLLName: The name of the DLL file to inject to debug (You probably don't need to change this)
- DebugRunnerDLLDirectory: The directory that contains above mentioned DLL file
- OtherArguments: Can be a combination of different things (more information below)


Structure is like this:
```json
{
    "CompiledExecutableName":"CheatEngineDummy.exe",
    "DebugRunnerDLLName":"DLL_redirConsole.dll",
    "DebugRunnerDLLDirectory":"E:\\VisualStudio\\projects\\ProcRedirStdout\\Release",
    "OtherArguments":"-show -intel -vanillaGFX"
}
```
It makes sense to have one file per project, in your GM-Project directory.<br>
To use it, drag and drop the `runnerconfig` onto the executable.
<br>
You should see GameMaker Studio compiling and running the game.

### OtherArguments
Valid are combinations of following:
- `-show` : Does not hide the main game window
- `-noaudio` : This switches off all audio in your game, no matter whether you are using the legacy sound functions or the new audio ones.
- `-inawindow` : Forces the game to start in a window, even when set to run in fullscreen mode.
- `-software` : Will force the game to use Software Vertex Processing instead of hardware for rendering the game graphics. This is especially useful for those PCs that are using an on-board Intel GFX chipset or show display issues on older machines.
- `-intel` : This will switch on a fix for bad Intel drivers, no matter what GPU is being used. This work around for those bad drivers will slow things down, giving a noticeable performance hit for your game, so if you do not need the fix do not use it.
- `-vanillaGFX` : Using this switches off any check for driver manufacturer, and can be very useful for testing your game "as is".