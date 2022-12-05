#include "PeHelper.hpp"

BOOL PeHelper::isX64(PeImage image)
{
    if(image.fileHeader()->Machine == IMAGE_FILE_MACHINE_AMD64)
        return TRUE;

    return FALSE;
}

BOOL PeHelper::isX86(PeImage image)
{
    if(image.fileHeader()->Machine == IMAGE_FILE_MACHINE_I386)
        return TRUE;

    return FALSE;
}

BOOL PeHelper::isValid(PeImage image)
{
    return TRUE;
}

IMAGE_SECTION_HEADER* PeHelper::getSections(PeImage image)
{
    return (IMAGE_SECTION_HEADER*)(image.base + image.dosHeader()->e_lfanew + (isX64(image) ? sizeof(IMAGE_NT_HEADERS64) : 0) + (isX86(image) ? sizeof(IMAGE_NT_HEADERS32) : 0));
}

IMAGE_SECTION_HEADER* PeHelper::findSection(PeImage image, UINT_PTR rva)
{
    IMAGE_SECTION_HEADER* sections = getSections(image);

    for(int i = 0; i < image.fileHeader()->NumberOfSections; i++)
        if(sections[i].VirtualAddress <= rva && sections[i].VirtualAddress + sections[i].SizeOfRawData >= rva)
            return &sections[i];

    return nullptr;
}

UINT_PTR PeHelper::rvaToVA(PeImage image, UINT_PTR rva)
{
    return image.base + rva;
}

UINT_PTR PeHelper::rvaToFA(PeImage image, UINT_PTR rva)
{
    IMAGE_SECTION_HEADER* section;

    if(!(section = findSection(image, rva)))
        return 0;

    return rva - section->VirtualAddress + section->PointerToRawData;
}

IMAGE_DATA_DIRECTORY* PeHelper::getDataDirectory(PeImage image)
{
    if(isX86(image))
        return (IMAGE_DATA_DIRECTORY*)rvaToVA(image, image.mapped ? image.optionalHeader32()->DataDirectory->VirtualAddress : rvaToFA(image, image.optionalHeader32()->DataDirectory->VirtualAddress));
    else if(isX64(image))
        return (IMAGE_DATA_DIRECTORY*)rvaToVA(image, image.mapped ? image.optionalHeader64()->DataDirectory->VirtualAddress : rvaToFA(image, image.optionalHeader64()->DataDirectory->VirtualAddress));

    return nullptr;
}

PeImage::PeImage(VOID* base, BOOL mapped) : base((UINT_PTR)base), mapped(mapped) { }

const IMAGE_DOS_HEADER* PeImage::dosHeader() const
{
    return (IMAGE_DOS_HEADER*)base;
}

const IMAGE_NT_HEADERS* PeImage::ntHeaders() const
{
    return (IMAGE_NT_HEADERS*)(base + dosHeader()->e_lfanew);
}

const IMAGE_NT_HEADERS32* PeImage::ntHeaders32() const
{
    return (IMAGE_NT_HEADERS32*)(base + dosHeader()->e_lfanew);
}

const IMAGE_NT_HEADERS64* PeImage::ntHeaders64() const
{
    return (IMAGE_NT_HEADERS64*)(base + dosHeader()->e_lfanew);
}

const IMAGE_FILE_HEADER* PeImage::fileHeader() const
{
    return (IMAGE_FILE_HEADER*)(base + dosHeader()->e_lfanew);
}

const IMAGE_OPTIONAL_HEADER* PeImage::optionalHeader() const
{
    return &ntHeaders()->OptionalHeader;
}

const IMAGE_OPTIONAL_HEADER32* PeImage::optionalHeader32() const
{
    return &ntHeaders32()->OptionalHeader;
}

const IMAGE_OPTIONAL_HEADER64* PeImage::optionalHeader64() const
{
    return &ntHeaders64()->OptionalHeader;
}