# GW-April
April is a 3rd-Party software for _Guild Wars_ that tries to combine essential 
quality-of-life upgrades first implemented in [GWToolbox++](https://github.com/HasKha/GWToolboxpp) 
with additional features for use in The Underworld.

![Random Stock Image](https://images.unsplash.com/photo-1519669159433-cf126d29b97e?ixid=MXwxMjA3fDB8MHxwaG90by1wYWdlfHx8fGVufDB8fHw%3D&ixlib=rb-1.2.1&auto=format&fit=crop&w=740&q=80)


## Installation
Binaries can be found on the [Releases Page](https://github.com/JuliusPunhal/GwApril/releases).


### Dependencies
- [Visual Studio 2019](https://visualstudio.microsoft.com/de/downloads/) 
including the workload for "Desktop development with C++" and Build Tools for v140
- [Windows 8.1 SDK](https://go.microsoft.com/fwlink/p/?LinkId=323507)
- [AutoIt](https://www.autoitscript.com/site/autoit/downloads/) to run the provided Injector

### Clone and Build
```
git clone https://github.com/JuliusPunhal/GW-April.git
cd GW-April
git submodule update --init
```
Open GW-April.sln with Visual Studio, accept the toolset-upgrade and compile. 
The file April.vcxproj.filters has been deliberately deleted; make sure to check 
the "Show all files" option to see the directory-structure in Visual Studio.


## Contributing
Contributions in form of in-game messages with ideas and criticism as well as issues on GitHub are very welcome.


## License
[MIT](https://choosealicense.com/licenses/mit/)
