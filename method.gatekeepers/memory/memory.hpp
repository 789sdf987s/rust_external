/**
 * <3 skit#2567
 *
 * don't make this into a shitty p2c
 */

#ifndef _MEMORY_HPP
#define _MEMORY_HPP

/* Includes for this file */
#include <Windows.h>
#include <cstdint>
#include <vector>
#include <map>
#include <string>
#include <mutex>
#include <TlHelp32.h>
#include "offsets.hpp"

/* A class for interacting with process memory */
class Memory
{
public:
    // The handle used to read and write memory.
    HANDLE handle;
private:

    /* grabs a process id */
    int GetPID(const char* process_name)
    {
        PROCESSENTRY32 entry;
        entry.dwSize = sizeof(PROCESSENTRY32);

        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

        // Check if first process is valid.
        if (!Process32First(snapshot, &entry) == TRUE)
        {
            return 0;
        }

        // Iterate until we either find our process or reach the end of the list.
        while (Process32Next(snapshot, &entry) == TRUE)
        {
            if (strcmp(entry.szExeFile, process_name) == 0)
            {
                return (int)entry.th32ProcessID;
            }
        }

        // Return 0, if we reach this point the process was not found.
        return 0;
    }
public:
    // The process ID of the process we're targeting.
    int process_id;

    // Get a handle to the target and store it.
    bool Attach(const char* proc_name)
    {
        // Get the process ID of the target process.
        this->process_id = GetPID(proc_name);

        // Check if process ID is valid
        if (this->process_id == 0)
        {
            return false;
        }

        // Open a handle to the target process with PROCESS_ALL_ACCESS
        HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, false, this->process_id);

        // Check if the handle is valid
        if (handle == INVALID_HANDLE_VALUE)
        {
            return false;
        }

        // Store the handle value.
        this->handle = handle;

        return true;
    }

    // Close a handle to the target and remove it.
    bool Detach()
    {
        CloseHandle(this->handle);
        this->handle = INVALID_HANDLE_VALUE;
        return true;
    }

    // Read value from memory address by type
    template<class T>
    T Read(UINT_PTR address, bool secondary = false)
    {
        T buffer;
        ReadProcessMemory(this->handle, (LPCVOID)address, &buffer, sizeof(buffer), 0);
        return buffer;
    }

    // Read value from memory address by type, specifying a pointer chain to follow
    template<typename Type>
    Type ReadChain(uint64_t address, std::vector<uint64_t> chain)
    {
        uint64_t current = address;
        for (int i = 0; i < chain.size() - 1; i++)
        {
            current = this->Read<uint64_t>(current + chain[i]);
        }
        return this->Read<Type>(current + chain[chain.size() - 1]);
    }

    // Write buffer to address 
    bool Write(uint64_t address, LPVOID buffer, SIZE_T size)
    { 
        return WriteProcessMemory(this->handle, (LPVOID)address, buffer, size, 0);
    }

    // Write value to memory address by type.
    template<typename type>
    bool Write(uint64_t address, type to_write)
    {
        type buffer = to_write;
        return this->Write(address, &buffer, sizeof(buffer));
    }

    // Read memory overload for ReadProcessMemory style calls.
    bool Read(UINT_PTR address, void* buffer, uintptr_t size, bool secondary = false)
    {
        if (ReadProcessMemory(this->handle, (LPCVOID)address, buffer, size, 0))
        {
            return true;
        }
        return false;
    }

    // Read an ASCII string to an std::string
    std::string ReadASCII(uint64_t address)
    {
        // Allocate our buffer with string size.
        char buffer[1024];

        // Read the string at address into the buffer
        this->Read(address, &buffer, 1024);

        // Return buffer string
        return std::string(buffer);
    }

    // Read an ASCII string to an std::string
    std::string ReadMonoString(uint64_t address)
    {
        /* Allocate the buffer for the string */
        char buffer[512];

        /* Read into the ptr */
        address = this->Read<uint64_t>(address);

        /* Read the string into the buffer */
        this->Read(address, buffer, 512);

        /* Return the read string */
        return buffer;
    }

    // Read a unicode string to an std::wstring
    std::wstring ReadUnicode(uint64_t address)
    {
        // Allocate our buffer with string size
        wchar_t buffer[1024 * sizeof(wchar_t)];

        // Read the string at address into the buffer
        this->Read(address, &buffer, 1024 * sizeof(wchar_t));

        // Convert the buffer to a std::wstring and return
        return std::wstring(buffer);
    }

    // Read a native string
    std::string ReadNative(uint64_t address)
    {
        // Allocate our buffer with string size.
        char buffer[1024];

        // Read the string at address into the buffer
        this->Read(this->Read<uint64_t>(address), &buffer, 1024);

        // Return buffer string
        return std::string(buffer);
    }


    // Get the base address of a module loaded into target proc.
    uint64_t GetModuleBase(const char* module_name)
    {
        // Get a snapshot of the target process.
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, (DWORD)this->process_id);

        // Check if our snapshot is valid.
        if (snapshot == INVALID_HANDLE_VALUE)
        {
            return 0;
        }

        // Create our empty moduleentry buffer.
        MODULEENTRY32 mod_entry;
        mod_entry.dwSize = sizeof(MODULEENTRY32);

        // Check the first item in our list is valid
        if (Module32First(snapshot, &mod_entry) == FALSE)
        {
            return 0;
        }

        // Iterate through the list until we find our module.
        while (Module32Next(snapshot, &mod_entry) == TRUE)
        {
            if (strcmp(mod_entry.szModule, module_name) == 0)
            {
                return (uint64_t)mod_entry.modBaseAddr;
            }
        }

        // Return 0 if we reach here.
        return 0;
    }

    // Implementation for this->scan_for_class
    uint64_t IL2CPPScan(const char* class_name)
    {
        uint64_t base = this->GetModuleBase("GameAssembly.dll");
        IMAGE_DOS_HEADER dos_header = this->Read<IMAGE_DOS_HEADER>(base);
        IMAGE_SECTION_HEADER data_header = this->Read<IMAGE_SECTION_HEADER>(base + dos_header.e_lfanew + sizeof(IMAGE_NT_HEADERS64) + (3 * 40));
        IMAGE_SECTION_HEADER next_section = this->Read<IMAGE_SECTION_HEADER>(base + dos_header.e_lfanew + sizeof(IMAGE_NT_HEADERS64) + (4 * 40));
        DWORD data_size = next_section.VirtualAddress - data_header.VirtualAddress;

        // Check if this section is .data, if not, return 0
        if (strcmp((char*)data_header.Name, ".data"))
        {
            return 0;
        }

        // Scan for the requested class
        for (uint64_t offset = data_size; offset > 0; offset -= 8)
        {
            char klass_name[0x100] = { 0 };
            auto klass = this->Read<uint64_t>(base + data_header.VirtualAddress + offset);
            if (klass == 0) { continue; }
            auto name_pointer = this->Read<uint64_t>(klass + 0x10);
            if (name_pointer == 0) { continue; }
            this->Read(name_pointer, klass_name, sizeof(klass_name));
            if (!strcmp(klass_name, class_name))
            {
                return klass;
            }
        }

        return 0;
    }
};

extern Memory* mem;

#endif // _MEMORY_HPP