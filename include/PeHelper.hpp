#pragma once

#include <Windows.h>

class PeImage
{
public:
    PeImage(VOID* base, BOOL mapped);

    const UINT_PTR base;
    const BOOL mapped;

    const IMAGE_DOS_HEADER* dosHeader() const;

    const IMAGE_NT_HEADERS* ntHeaders() const;
    const IMAGE_NT_HEADERS32* ntHeaders32() const;
    const IMAGE_NT_HEADERS64* ntHeaders64() const;

    const IMAGE_FILE_HEADER* fileHeader() const;

    const IMAGE_OPTIONAL_HEADER* optionalHeader() const;
    const IMAGE_OPTIONAL_HEADER32* optionalHeader32() const;
    const IMAGE_OPTIONAL_HEADER64* optionalHeader64() const;
};

class PeHelper
{
public:
    static BOOL isX64(PeImage image);
    static BOOL isX86(PeImage image);
    static BOOL isValid(PeImage image);

    static IMAGE_SECTION_HEADER* getSections(PeImage image);

    static IMAGE_SECTION_HEADER* findSection(PeImage image, UINT_PTR rva);

    static UINT_PTR rvaToVA(PeImage image, UINT_PTR rva);
    static UINT_PTR rvaToFA(PeImage image, UINT_PTR va);

    static IMAGE_DATA_DIRECTORY* getDataDirectory(PeImage image);
};