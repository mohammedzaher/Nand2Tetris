#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <bitset>

using namespace std;

map<string, int> symbolTable;
map<string, string> mnemonicsTable;
map<string, string> mnemonicsTable1;
string inFile = "D:/Courses/Nand2Tetris_I/nand2tetris/projects/06/add/Add.asm";
// string inFile = "D:/Courses/Nand2Tetris_I/nand2tetris/projects/06/max/Max.asm";
// string inFile = "D:/Courses/Nand2Tetris_I/nand2tetris/projects/06/max/MaxL.asm";
// string inFile = "D:/Courses/Nand2Tetris_I/nand2tetris/projects/06/pong/Pong.asm";
// string inFile = "D:/Courses/Nand2Tetris_I/nand2tetris/projects/06/pong/PongL.asm";
// string inFile = "D:/Courses/Nand2Tetris_I/nand2tetris/projects/06/rect/Rect.asm";
// string inFile = "D:/Courses/Nand2Tetris_I/nand2tetris/projects/06/rect/RectL.asm";
string outFile = "RectL.hack";

bool isNumber(const string &str)
{
    for (char const &c : str)
    {
        if (isdigit(c) == 0)
            return false;
    }
    return true;
}

void ignoreWhitespaces()
{
    ifstream iFile;
    ofstream oFile;
    string line;
    string inputfile = inFile;      // The Input of the Assembler
    string outputfile = "temp.txt"; // The input of handleSymbols

    // Open input file
    iFile.open(inputfile);
    if (!iFile.is_open())
    {
        cout << "Error opening input file ignoreWhitespaces!" << endl;
        return;
    }

    // Open temp out file
    oFile.open(outputfile);
    if (!oFile.is_open())
    {
        cout << "Error opening temp output file ignoreWhitesapces!" << endl;
        return;
    }

    while (getline(iFile, line))
    {
        string temp;
        // cout << line << endl;

        // Check the end of the file
        if (line == "-1")
        {
            cout << "ignoreWhitespaces Done!" << endl;
            iFile.close();
            oFile.close();
            return;
        }
        else
        {
            // ignoring comments
            for (int i = 0; i < line.size(); i++)
            {
                if (line[i] == ' ')
                {
                    continue;
                }
                if (line[i] == '/')
                {
                    break;
                }
                else
                {
                    temp.push_back(line[i]);
                }
            }

            // Check if the line is empty
            if (temp.empty())
            {
                continue;
            }
            else
            {
                oFile << temp << endl;
            }
        }
    }
}

void initializingSymbolTable()
{
    symbolTable["R0"] = 0;
    symbolTable["R1"] = 1;
    symbolTable["R2"] = 2;
    symbolTable["R3"] = 3;
    symbolTable["R4"] = 4;
    symbolTable["R5"] = 5;
    symbolTable["R6"] = 6;
    symbolTable["R7"] = 7;
    symbolTable["R8"] = 8;
    symbolTable["R9"] = 9;
    symbolTable["R10"] = 10;
    symbolTable["R11"] = 11;
    symbolTable["R12"] = 12;
    symbolTable["R13"] = 13;
    symbolTable["R14"] = 14;
    symbolTable["R15"] = 15;
    symbolTable["SCREEN"] = 16384;
    symbolTable["KBD"] = 24576;
    symbolTable["SP"] = 0;
    symbolTable["LCL"] = 1;
    symbolTable["ARG"] = 2;
    symbolTable["THIS"] = 3;
    symbolTable["THAT"] = 4;
}

void initializingMnemonicsTable()
{
    // computation function codes
    mnemonicsTable["0"] = "0101010";
    mnemonicsTable["1"] = "0111111";
    mnemonicsTable["-1"] = "0111010";
    mnemonicsTable["D"] = "0001100";
    mnemonicsTable["A"] = "0110000";
    mnemonicsTable["M"] = "1110000";
    mnemonicsTable["!D"] = "0001101";
    mnemonicsTable["!A"] = "0110001";
    mnemonicsTable["!M"] = "1110001";
    mnemonicsTable["-D"] = "0001111";
    mnemonicsTable["-A"] = "0110011";
    mnemonicsTable["-M"] = "1110011";
    mnemonicsTable["D+1"] = "0011111";
    mnemonicsTable["A+1"] = "0110111";
    mnemonicsTable["M+1"] = "1110111";
    mnemonicsTable["D-1"] = "0001110";
    mnemonicsTable["A-1"] = "0110010";
    mnemonicsTable["M-1"] = "1110010";
    mnemonicsTable["D+A"] = "0000010";
    mnemonicsTable["D+M"] = "1000010";
    mnemonicsTable["D-A"] = "0010011";
    mnemonicsTable["D-M"] = "1010011";
    mnemonicsTable["A-D"] = "0000111";
    mnemonicsTable["M-D"] = "1000111";
    mnemonicsTable["D&A"] = "0000000";
    mnemonicsTable["D&M"] = "1000000";
    mnemonicsTable["D|A"] = "0010101";
    mnemonicsTable["D|M"] = "1010101";

    // computation result storage codes
    mnemonicsTable1["none"] = "000";
    mnemonicsTable1["M"] = "001";
    mnemonicsTable1["D"] = "010";
    mnemonicsTable1["DM"] = "011";
    mnemonicsTable1["MD"] = "011";
    mnemonicsTable1["A"] = "100";
    mnemonicsTable1["AM"] = "101";
    mnemonicsTable1["MA"] = "101";
    mnemonicsTable1["AD"] = "110";
    mnemonicsTable1["DA"] = "110";
    mnemonicsTable1["ADM"] = "111";
    mnemonicsTable1["AMD"] = "111";
    mnemonicsTable1["DAM"] = "111";
    mnemonicsTable1["DMA"] = "111";
    mnemonicsTable1["MAD"] = "111";
    mnemonicsTable1["ADA"] = "111";

    // branch condition codes
    mnemonicsTable["none"] = "000";
    mnemonicsTable["JGT"] = "001";
    mnemonicsTable["JEQ"] = "010";
    mnemonicsTable["JGE"] = "011";
    mnemonicsTable["JLT"] = "100";
    mnemonicsTable["JNE"] = "101";
    mnemonicsTable["JLE"] = "110";
    mnemonicsTable["JMP"] = "111";
}

void handleInstructions()
{
    ifstream iFile;
    ofstream oFile;
    string line, temp;
    string inputfile = "temp1.txt"; // The output of handleSymbols
    string outputfile = outFile;    // The output of the Assembler
    bitset<16> bsetA;
    int variables = 16;

    // Open input file
    iFile.open(inputfile);
    if (!iFile.is_open())
    {
        cout << "Error opening input file handleInstructions!" << endl;
        return;
    }

    // Open temp out file
    oFile.open(outputfile);
    if (!oFile.is_open())
    {
        cout << "Error opening temp output file handleInstructions!" << endl;
        return;
    }

    // Reading the input file
    while (getline(iFile, line))
    {
        temp = "";
        // cout << line << endl;

        // Check the end of the file
        if (line == "-1")
        {
            cout << "handleInstructions Done!" << endl;
            iFile.close();
            oFile.close();
            return;
        }
        else
        {
            // Handling Instructions
            int found = line.find("@", 0);
            // Handling A-Instruction
            if (found != -1)
            {
                string subA = line.substr(found + 1);

                if (isNumber(subA))
                {
                    int num = stoi(subA);
                    oFile << bitset<16>(num) << endl;
                }
                else
                {

                    if (symbolTable.find(subA) != symbolTable.end())
                    {
                        int num = symbolTable[subA];
                        oFile << bitset<16>(num) << endl;
                    }
                    else
                    {
                        symbolTable[subA] = variables;
                        oFile << bitset<16>(variables) << endl;
                        variables += 1;
                    }
                }
            }
            // Handling C-Instruction
            else
            {
                string a = "", b = "", c = "";
                int founda = line.find("=");
                int foundb = line.find(";");
                if (founda != -1)
                {
                    a = line.substr(0, founda);
                }
                else
                {
                    a = "none";
                }
                if (foundb == -1)
                {
                    b = line.substr(founda + 1);
                    c = "none";
                }
                else
                {
                    b = line.substr(founda + 1, foundb);
                    c = line.substr(foundb + 1);
                }
                temp = "111" + mnemonicsTable[b] + mnemonicsTable1[a] + mnemonicsTable[c];
                oFile << temp << endl;
            }
        }
    }
}

void handleSymbols()
{
    ifstream iFile;
    ofstream oFile;
    string line, temp;
    string inputfile = "temp.txt";   // The output of ignoreWhitespaces
    string outputfile = "temp1.txt"; // the input of handleInstructions

    // Open input file
    iFile.open(inputfile);
    if (!iFile.is_open())
    {
        cout << "Error opening input file handleSymbols!" << endl;
        return;
    }

    // Open temp out file
    oFile.open(outputfile);
    if (!oFile.is_open())
    {
        cout << "Error opening temp output file handleSymbols!" << endl;
        return;
    }

    int i = 0;
    while (getline(iFile, line))
    {
        if (line == "-1")
        {
            cout << "handleSymbols Done!" << endl;
            iFile.close();
            oFile.close();
            return;
        }
        else
        {
            int found = line.find("(");
            int found2 = line.find(")");
            // Symbol found
            if (found != -1)
            {
                string a = line.substr(found + 1, found2 - 1);
                symbolTable[a] = i;
            }
            // Symbol not found
            else
            {
                i += 1;
                oFile << line << endl;
            }
        }
    }
}

int main()
{
    initializingSymbolTable();
    initializingMnemonicsTable();
    ignoreWhitespaces();
    handleSymbols();
    handleInstructions();
}