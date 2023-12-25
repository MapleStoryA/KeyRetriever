
#include <Windows.h>
#include <iostream>
#include <iomanip>
#include <fstream>

#define KEY_POS 0x14020
#define KEYGEN_POS 0x1340

std::ofstream outfile("Key.txt");

void DisplayKey(int pModulePos) {

    outfile << "----------- AOB ----------------" << std::endl;
    for (int i = 0; i < 16 * 8; i++) {
        short value = *(int*)(pModulePos + KEY_POS + i);
        value &= 0xFF;

        outfile << "0x" << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << value << ", ";

        if (i % 16 == 15) outfile << std::endl;
    }

    outfile << std::endl;

    outfile << "----------- MapleShark format ----------------" << std::endl;
    for (int i = 0; i < 16 * 8; i += 4) {
        short value = *(int*)(pModulePos + KEY_POS + i);
        value &= 0xFF;

        outfile << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << value;
    }
    outfile << std::endl;
    outfile << std::endl;

    outfile << "----------- OdinMS Format ----------------" << std::endl;
    outfile << "byte key[] = { ";
    for (int i = 0; i < 16 * 8; i += 4 * 4) {
        short value = *(int*)(pModulePos + KEY_POS + i);
        value &= 0xFF;
        outfile << "0x";
        outfile << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << value;
        outfile << ", 0x00, 0x00, 0x00";
        if (i != 112) outfile << ", ";
    }
    outfile << " }";
}

int main(int argc)
{
    if (!outfile)
    {
        std::cerr << "Could't not open/save Key.txt ....." << std::endl;
    }
    else
    {
        HMODULE mod = LoadLibraryA("ZLZ.dll");

        int addr = (int)mod;

        //DisplayKey(addr);
        void (*CAES__ChangeKey)(void) = (void (*)(void))(addr + KEYGEN_POS);
        CAES__ChangeKey();
        DisplayKey(addr);

        FreeLibrary(mod);

        std::cout << "Successful generate Key.txt in your Maplestory folder!" << std::endl;
    }
    getchar();

    return 0;
}