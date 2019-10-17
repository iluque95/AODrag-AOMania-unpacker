#include <iostream>
#include <fstream>
#include <stdio.h>
#include <inttypes.h>
#include <chrono>
#include <thread>
#include <string>

#define GPINDEX "gpindex.txt"
#define BMP_ID 0x4D42

using namespace std;

int main(uint32_t p_Argc, char* p_Arg[])
{

	if (p_Argc != 3)
	{
		printf("\"Usage: Unpacker.exe <compressed file> <destination>\n\"");
		return -1;
	}

    uint16_t head;

    FILE *fp, *fw;

    fp = fopen(p_Arg[1], "rb");

    ofstream myfile(GPINDEX);

    uint32_t size;

    int16_t files = 0;

    int32_t actual = 0;
    int32_t next = 1;

    int ferr = 1;

    // Buscamos el primer BMP en el archivo
    bool found = false;

    while (!found && fread(&head, sizeof(head), 1, fp) != EOF)
    {
        if (head == BMP_ID)
            found = true;
    }

    actual = ftell(fp) - 2;

    // Retrocedemos 2 bytes por haber avazando al leer el header.
    ferr = fseek(fp, actual, SEEK_SET);

    while (!feof(fp) && next != actual && found)
    {

        ++files;

        actual = ftell(fp);

        // Leemos cabecera
        fread(&head, 2, 1, fp);

        // Leemos tamaño BMP
        fread(&size, 4, 1, fp);

        // Calculamos siguiente salto
        next = actual + size;

        if (myfile.is_open())
        {
            myfile << "(" << files << ") " << size << "->" << actual + 1 << "\n";
        }

        // Apuntamos al siguiente BMP
        ferr = fseek(fp, next, SEEK_SET);

        size = 0;
    }

    std::cout << "Number of BMP's: " << files << endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    myfile.close();

    // Procesamos el fichero gpindex

    ifstream pindexFile;

    pindexFile.open(GPINDEX, ifstream::in);

    if (!pindexFile.is_open())
    {
        cout << GPINDEX << " does not exist" << endl;
        return 0;
    }

    auto r_File = std::ifstream(p_Arg[1], std::ifstream::binary);
    bool first_time = false;

    while (!pindexFile.eof())
    {
        string line;

        std::getline(pindexFile, line);

        // Buscamos lineas donde aparezca la flechita
        if (line.find("->") == string::npos)
            continue;

        int seekOffset = 0;
        int size = 0;
        int fileNumber = 0;

        sscanf(line.c_str(), "(%d) %d->%d", &fileNumber, &size, &seekOffset);

        // Nos situamos en la posición que toca

        if (! first_time)
        {
            r_File.seekg(seekOffset-1);
            first_time = true;
        }

        string filename = std::to_string(fileNumber) + ".bmp";

        printf("Extracting %s - Offset: %d / Length: %d\n", filename.c_str(), seekOffset, size);

        auto r_Bytes = new char[size];
        r_File.read(r_Bytes, size);

        auto r_File_Output = std::ofstream(p_Arg[2] + ("/"+filename), ofstream::binary | ofstream::out | fstream::trunc);
        r_File_Output.write(r_Bytes, size);
        r_File_Output.close();

        delete r_Bytes;

    }
}