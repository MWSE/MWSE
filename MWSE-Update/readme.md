# MWSE Updater
## Description
This project contains the source code for the MWSE-Update executable, which is used to download the latest updates for MWSE from your Morrowind directory. The updater is included in the MWSE install package and does not need to be manually downloaded unless you need to use the updater for a different OS platform.
## Publishing
### Windows
For Windows, run the following command:
```
dotnet publish -c release  -r win-x64 --self-contained -p:PublishSingleFile=true -p:PublishTrimmed=true
```

This will produce a `MWSE-Updater.exe` file in the `publish` folder. This file is self contained and does not require .NET 6 to run on a Windows 64-bit machine. Simply place it into your Morrowind directory to use it.

### Linux
For Linux, run the following command:
```
dotnet publish -c release  -r linux-x64 --self-contained -p:PublishSingleFile=true -p:PublishTrimmed=true
```

This will produce a `MWSE-Updater` file in the `publish` folder. This file is self contained and does not require .NET 6 to run on a Linux 64-bit machine. Simply place it into your Morrowind directory to use it.

### MacOS
For MacOS, run the following command:
```
dotnet publish -c release  -r osx-x64 --self-contained -p:PublishSingleFile=true -p:PublishTrimmed=true
```

This will produce a `MWSE-Updater` file in the `publish` folder. This file is self contained and does not require .NET 6 to run on a MacOS 64-bit machine. Simply place it into your Morrowind directory to use it.
