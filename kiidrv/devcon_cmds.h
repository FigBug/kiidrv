#pragma once
#include "devcon.h"

int cmdClasses(_In_ LPCTSTR BaseName, _In_opt_ LPCTSTR Machine, _In_ DWORD Flags, _In_ int argc, _In_reads_(argc) PTSTR argv[]);
int cmdListClass(_In_ LPCTSTR BaseName, _In_opt_ LPCTSTR Machine, _In_ DWORD Flags, _In_ int argc, _In_reads_(argc) PTSTR argv[]);
int cmdFind(_In_ LPCTSTR BaseName, _In_opt_ LPCTSTR Machine, _In_ DWORD Flags, _In_ int argc, _In_reads_(argc) PTSTR argv[]);
int cmdFindAll(_In_ LPCTSTR BaseName, _In_opt_ LPCTSTR Machine, _In_ DWORD Flags, _In_ int argc, _In_reads_(argc) PTSTR argv[]);
int cmdStatus(_In_ LPCTSTR BaseName, _In_opt_ LPCTSTR Machine, _In_ DWORD Flags, _In_ int argc, _In_reads_(argc) PTSTR argv[]);
int cmdResources(_In_ LPCTSTR BaseName, _In_opt_ LPCTSTR Machine, _In_ DWORD Flags, _In_ int argc, _In_reads_(argc) PTSTR argv[]);
int cmdDriverFiles(_In_ LPCTSTR BaseName, _In_opt_ LPCTSTR Machine, _In_ DWORD Flags, _In_ int argc, _In_reads_(argc) PTSTR argv[]);
int cmdDriverNodes(_In_ LPCTSTR BaseName, _In_opt_ LPCTSTR Machine, _In_ DWORD Flags, _In_ int argc, _In_reads_(argc) PTSTR argv[]);
int cmdHwIds(_In_ LPCTSTR BaseName, _In_opt_ LPCTSTR Machine, _In_ DWORD Flags, _In_ int argc, _In_reads_(argc) PTSTR argv[]);
int cmdStack(_In_ LPCTSTR BaseName, _In_opt_ LPCTSTR Machine, _In_ DWORD Flags, _In_ int argc, _In_reads_(argc) PTSTR argv[]);
int cmdEnable(_In_ LPCTSTR BaseName, _In_opt_ LPCTSTR Machine, _In_ DWORD Flags, _In_ int argc, _In_reads_(argc) PTSTR argv[]);
int cmdDisable(_In_ LPCTSTR BaseName, _In_opt_ LPCTSTR Machine, _In_ DWORD Flags, _In_ int argc, _In_reads_(argc) PTSTR argv[]);
int cmdRestart(_In_ LPCTSTR BaseName, _In_opt_ LPCTSTR Machine, _In_ DWORD Flags, _In_ int argc, _In_reads_(argc) PTSTR argv[]);
int cmdReboot(_In_ LPCTSTR BaseName, _In_opt_ LPCTSTR Machine, _In_ DWORD Flags, _In_ int argc, _In_reads_(argc) PTSTR argv[]);
int cmdUpdate(_In_ LPCTSTR BaseName, _In_opt_ LPCTSTR Machine, _In_ DWORD Flags, _In_ int argc, _In_reads_(argc) PTSTR argv[]);
int cmdInstall(_In_ LPCTSTR BaseName, _In_opt_ LPCTSTR Machine, _In_ DWORD Flags, _In_ int argc, _In_reads_(argc) PTSTR argv[]);
int cmdUpdateNI(_In_ LPCTSTR BaseName, _In_opt_ LPCTSTR Machine, _In_ DWORD Flags, _In_ int argc, _In_reads_(argc) PTSTR argv[]);
int cmdRemove(_In_ LPCTSTR BaseName, _In_opt_ LPCTSTR Machine, _In_ DWORD Flags, _In_ int argc, _In_reads_(argc) PTSTR argv[]);
int cmdRemoveAll(_In_ LPCTSTR BaseName, _In_opt_ LPCTSTR Machine, _In_ DWORD Flags, _In_ int argc, _In_reads_(argc) PTSTR argv[]);
int cmdRescan(_In_ LPCTSTR BaseName, _In_opt_ LPCTSTR Machine, _In_ DWORD Flags, _In_ int argc, _In_reads_(argc) PTSTR argv[]);
int cmdClassFilter(_In_ LPCTSTR BaseName, _In_opt_ LPCTSTR Machine, _In_ DWORD Flags, _In_ int argc, _In_reads_(argc) PTSTR argv[]);
int cmdSetHwid(_In_ LPCTSTR BaseName, _In_opt_ LPCTSTR Machine, _In_ DWORD Flags, _In_ int argc, _In_reads_(argc) PTSTR argv[]);
int cmdDPAdd(_In_ LPCTSTR BaseName, _In_opt_ LPCTSTR Machine, _In_ DWORD Flags, _In_ int argc, _In_reads_(argc) PTSTR argv[]);
int cmdDPDelete(_In_ LPCTSTR BaseName, _In_opt_ LPCTSTR Machine, _In_ DWORD Flags, _In_ int argc, _In_reads_(argc) PTSTR argv[]);
int cmdDPEnumLegacy(_In_ LPCTSTR BaseName, _In_opt_ LPCTSTR Machine, _In_ DWORD Flags, _In_ int argc, _In_reads_(argc) PTSTR argv[]);