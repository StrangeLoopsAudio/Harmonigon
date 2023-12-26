#define Version Trim(FileRead(FileOpen("..\VERSION")))
#define PluginName "Harmonigon"
#define Publisher "StrangeLoops"
#define Year GetDateTimeString("yyyy","","")

; 'Types': What get displayed during the setup
[Types]
Name: "full"; Description: "Full installation"
Name: "custom"; Description: "Custom installation"; Flags: iscustom

; Components are used inside the script and can be composed of a set of 'Types'
[Components]
Name: "standalone"; Description: "Standalone application"; Types: full custom

[Setup]
ArchitecturesInstallIn64BitMode=x64
ArchitecturesAllowed=x64
AppName={#PluginName}
OutputBaseFilename={#PluginName}-{#Version}-Windows
AppCopyright=Copyright (C) {#Year} {#Publisher}
AppPublisher={#Publisher}
AppVersion={#Version}
DefaultDirName="{commonpf64}\{#Publisher}\{#PluginName}\{#PluginName}.exe"
DisableDirPage=yes
LicenseFile="..\LICENSE"
UninstallFilesDir="{commonappdata}\{#PluginName}\uninstall"

; MSVC adds a .ilk when building the plugin. Let's not include that.
[Files]
Source: "..\Builds\Harmonigon_artefacts\Release\{#PluginName}.exe"; DestDir: "{commonpf64}\{#Publisher}\{#PluginName}"; Flags: ignoreversion; Components: standalone

[Icons]
Name: "{autoprograms}\Harmonigon"; Filename: "{commonpf64}\{#Publisher}\{#PluginName}\Harmonigon.exe"; Components: standalone 
Name: "{autoprograms}\Uninstall Harmonigon"; Filename: "{uninstallexe}"
