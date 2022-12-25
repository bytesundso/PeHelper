#pragma once

#include <Windows.h>

class PeImage
{
public:
    PeImage(VOID* base, BOOL mapped = TRUE);

    CONST UINT_PTR base;
    CONST BOOL mapped;

    PIMAGE_DOS_HEADER dosHeader() CONST;

    PIMAGE_NT_HEADERS ntHeaders() CONST;
    PIMAGE_NT_HEADERS32 ntHeaders32() CONST;
    PIMAGE_NT_HEADERS64 ntHeaders64() CONST;
};

class PeHelper
{
public:
    static BOOL isValid(PeImage image);

    static PIMAGE_SECTION_HEADER getSection(PeImage image, UINT index);
    static PIMAGE_DATA_DIRECTORY getDataDirectory(PeImage image, UINT index);

    static PIMAGE_SECTION_HEADER findSection(PeImage image, UINT_PTR rva);

    static UINT_PTR rvaToVA(PeImage image, UINT_PTR rva);
    static UINT_PTR rvaToFA(PeImage image, UINT_PTR rva);

    static PIMAGE_EXPORT_DIRECTORY getExportDirectory(PeImage image);
    static PIMAGE_IMPORT_DESCRIPTOR getImportDesctriptor(PeImage image);
};