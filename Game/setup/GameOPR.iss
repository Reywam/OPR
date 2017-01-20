; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "GameOPR"
#define MyAppVersion "1.0"
#define MyAppPublisher "Reywam"
#define MyAppExeName "Game.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{DCACED90-6024-424B-A856-208D1751EF91}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
DefaultDirName={pf}\{#MyAppName}
DisableProgramGroupPage=yes
OutputDir=E:\CppProj\Game\setup
OutputBaseFilename=GameOPRsetup
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "russian"; MessagesFile: "compiler:Languages\Russian.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "E:\CppProj\Game\Release\Game.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\CppProj\Game\Release\openal32.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\CppProj\Game\Release\sfml-audio-2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\CppProj\Game\Release\sfml-audio-d-2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\CppProj\Game\Release\sfml-graphics-2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\CppProj\Game\Release\sfml-graphics-d-2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\CppProj\Game\Release\sfml-network-2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\CppProj\Game\Release\sfml-network-d-2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\CppProj\Game\Release\sfml-system-2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\CppProj\Game\Release\sfml-system-d-2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\CppProj\Game\Release\sfml-window-2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\CppProj\Game\Release\sfml-window-d-2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\CppProj\Game\Game\Res\*"; DestDir: "{app}\Res"; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{commonprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent
