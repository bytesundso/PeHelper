#include "PeHelper.hpp"

PeImage::PeImage(VOID* base, BOOL mapped) : base((UINT_PTR)base), mapped(mapped) 
{ 

}

PIMAGE_DOS_HEADER PeImage::dosHeader() CONST
{
    return (IMAGE_DOS_HEADER*)base;
}

PIMAGE_NT_HEADERS PeImage::ntHeaders() CONST
{
    return (PIMAGE_NT_HEADERS)(base + dosHeader()->e_lfanew);
}

PIMAGE_NT_HEADERS32 PeImage::ntHeaders32() CONST
{
    return (PIMAGE_NT_HEADERS32)ntHeaders();
}

PIMAGE_NT_HEADERS64 PeImage::ntHeaders64() CONST
{
    return (PIMAGE_NT_HEADERS64)ntHeaders();
}

BOOL PeHelper::isValid(PeImage image)
{
    if (image.dosHeader()->e_magic != IMAGE_DOS_SIGNATURE)
        return FALSE;

    if (image.ntHeaders()->Signature != IMAGE_NT_SIGNATURE)
        return FALSE;

    if (image.ntHeaders()->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR32_MAGIC
    && image.ntHeaders()->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR64_MAGIC)
        return FALSE;

    return TRUE;
}

PIMAGE_SECTION_HEADER PeHelper::getSection(PeImage image, UINT index)
{
    return &(IMAGE_FIRST_SECTION(image.ntHeaders()))[index];
}

PIMAGE_DATA_DIRECTORY PeHelper::getDataDirectory(PeImage image, UINT index)
{
    switch (image.ntHeaders()->OptionalHeader.Magic)
    {
    case IMAGE_NT_OPTIONAL_HDR32_MAGIC:
        return &image.ntHeaders32()->OptionalHeader.DataDirectory[index];
    case IMAGE_NT_OPTIONAL_HDR64_MAGIC:
        return &image.ntHeaders64()->OptionalHeader.DataDirectory[index];
    default:
        return NULL;
    }
}

PIMAGE_SECTION_HEADER PeHelper::findSection(PeImage image, UINT_PTR rva)
{
    for(UINT i = 0; i < image.ntHeaders32()->FileHeader.NumberOfSections; i++)
    {
        PIMAGE_SECTION_HEADER current = getSection(image, i);
        
        if(rva >= current->VirtualAddress && rva < current->VirtualAddress + current->SizeOfRawData)
            return current;
    }

    return NULL;
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