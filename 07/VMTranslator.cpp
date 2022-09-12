#include <iostream>
#include <fstream>
#include <string>

using namespace std;

enum commands
{
    C_ARITHMETIC,
    C_PUSH,
    C_POP,
    C_LABEL,
    C_GOTO,
    C_IF,
    C_FUNCTION,
    C_RETRUN,
    C_CALL
};
class Parser
{
private:
    ifstream iFile;
    string line = "", temp = "";
    string _arg1 = "", _arg0 = "";
    int _arg2 = 0;
    bool flag = true;

public:
    // Constructor Opens the input file
    Parser(string inFile)
    {
        // Opens input file
        iFile.open(inFile);

        if (!iFile.is_open())
        {
            cout << "Error opening input file Parser!" << endl;
            return;
        }
        while (line.empty() || line.find('/') != -1)
        {
            flag = (bool)getline(iFile, line);
        }
    }
    // Are there more commands in the input
    bool hasMoreCommands()
    {
        if (flag)
        {
            return true;
        }
        else
            return false;
    }
    // Returns the type of the command
    int commandType()
    {
        if (line.find("push") != -1)
        {
            return C_PUSH;
        }
        else if (line.find("pop") != -1)
        {
            return C_POP;
        }
        else if (line.find("label") != -1)
        {
            return C_LABEL;
        }
        else if (line.find("if-goto") != -1)
        {
            return C_IF;
        }
        else if (line.find("goto") != -1)
        {
            return C_GOTO;
        }
        else if (line.find("function") != -1)
        {
            return C_FUNCTION;
        }
        else if (line.find("call") != -1)
        {
            return C_CALL;
        }
        else if (line.find("return") != -1)
        {
            return C_RETRUN;
        }
        else
        {
            return C_ARITHMETIC;
        }
    }
    // Returns the first argument of the current command
    string arg1()
    {
        if (commandType() == C_ARITHMETIC)
        {
            _arg1 = line;
        }
        else
        {
            int found1 = line.find(' ');
            int found2 = line.find_last_of(' ');
            _arg1 = line.substr(found1 + 1, found2 - found1 - 1);
        }
        return _arg1;
    }
    // Returns the second argument of the current command.
    int arg2()
    {
        _arg2 = 0;
        int found1 = line.find_last_of(' ');
        _arg2 = stoi(line.substr(found1 + 1));
        return _arg2;
    }

    void advance()
    {
        // Ignores Inline Comments
        flag = (bool)getline(iFile, line);
    }
};

class CodeWriter
{
private:
    ofstream oFile;
    int labelNum = 0;
    string outputFile = "";

public:
    // Opens the output file
    CodeWriter(string outFile)
    {
        int found = outFile.find_last_of('.');
        outputFile = outFile.substr(0, found);
        found = outFile.find_last_of('/');
        if (found != -1)
        {
            outputFile = outputFile.substr(found + 1);
        }
        oFile.open(outFile);
        if (!oFile.is_open())
        {
            cout << "Error opening the output file CodeWriter" << endl;
            return;
        }
    }

    void writeArithmetic(string command)
    {
        if (command == "add")
        {
            oFile << "@SP\nA=M\nA=A-1\nD=M\nA=A-1\nM=D+M\n@SP\nM=M-1\n";
        }
        else if (command == "sub")
        {
            oFile << "@SP\nA=M\nA=A-1\nD=M\nA=A-1\nM=M-D\n@SP\nM=M-1\n";
        }
        else if (command == "neg")
        {
            oFile << "@SP\nA=M\nA=A-1\nM=-M\n";
        }
        else if (command == "eq")
        {
            oFile << "@SP\nA=M\nA=A-1\nD=M\nA=A-1\nD=M-D\n@LABEL" << labelNum << "\nD;JEQ\n";
            oFile << "@SP\nM=M-1\nA=M\nA=A-1\nM=0\n@LABEL" << labelNum + 1 << "\n0;JMP\n";
            oFile << "(LABEL" << labelNum << ")" << endl;
            oFile << "@SP\nM=M-1\nA=M\nA=A-1\nM=-1\n";
            oFile << "(LABEL" << labelNum + 1 << ")" << endl;
            labelNum += 2;
        }
        else if (command == "gt")
        {
            oFile << "@SP\nA=M\nA=A-1\nD=M\nA=A-1\nD=M-D\n@LABEL" << labelNum << "\nD;JGT\n";
            oFile << "@SP\nM=M-1\nA=M\nA=A-1\nM=0\n@LABEL" << labelNum + 1 << "\n0;JMP\n";
            oFile << "(LABEL" << labelNum << ")" << endl;
            oFile << "@SP\nM=M-1\nA=M\nA=A-1\nM=-1\n";
            oFile << "(LABEL" << labelNum + 1 << ")" << endl;
            labelNum += 2;
        }
        else if (command == "lt")
        {
            oFile << "@SP\nA=M\nA=A-1\nD=M\nA=A-1\nD=M-D\n@LABEL" << labelNum << "\nD;JLT\n";
            oFile << "@SP\nM=M-1\nA=M\nA=A-1\nM=0\n@LABEL" << labelNum + 1 << "\n0;JMP\n";
            oFile << "(LABEL" << labelNum << ")" << endl;
            oFile << "@SP\nM=M-1\nA=M\nA=A-1\nM=-1\n";
            oFile << "(LABEL" << labelNum + 1 << ")" << endl;
            labelNum += 2;
        }
        else if (command == "and")
        {
            oFile << "@SP\nA=M\nA=A-1\nD=M\nA=A-1\nM=M&D\n";
            oFile << "@SP\nM=M-1\n";
        }
        else if (command == "or")
        {
            oFile << "@SP\nA=M\nA=A-1\nD=M\nA=A-1\nM=M|D\n";
            oFile << "@SP\nM=M-1\n";
        }
        else if (command == "not")
        {
            oFile << "@SP\nA=M\nA=A-1\nM=!M\n";
        }
    }

    void writePushPop(int command, string segment, int index)
    {
        if (segment == "local")
        {
            if (command == C_PUSH)
            {
                oFile << "@LCL\nD=M\n@" << index << "\nA=D+A\nD=M\n"; // addr = segmentPointer + i
                oFile << "@SP\nA=M\nM=D\n";                           // *SP = *addr
                oFile << "@SP\nM=M+1\n";                              // SP++
            }
            else if (command == C_POP)
            {
                oFile << "@LCL\nD=M\n@" << index << "\nD=D+A\n@addr\nM=D\n"; // addr = segmentPointer + i
                oFile << "@SP\nM=M-1\n";
                oFile << "@SP\nA=M\nD=M\n@addr\nA=M\nM=D\n";
            }
        }
        else if (segment == "argument")
        {
            if (command == C_PUSH)
            {
                oFile << "@ARG\nD=M\n@" << index << "\nA=D+A\nD=M\n"; // addr = segmentPointer + i
                oFile << "@SP\nA=M\nM=D\n";                           // *SP = *addr
                oFile << "@SP\nM=M+1\n";                              // SP++
            }
            else if (command == C_POP)
            {
                oFile << "@ARG\nD=M\n@" << index << "\nD=D+A\n@addr\nM=D\n"; // addr = segmentPointer + i
                oFile << "@SP\nM=M-1\n";
                oFile << "@SP\nA=M\nD=M\n@addr\nA=M\nM=D\n";
            }
        }
        else if (segment == "this")
        {
            if (command == C_PUSH)
            {
                oFile << "@THIS\nD=M\n@" << index << "\nA=D+A\nD=M\n"; // addr = segmentPointer + i
                oFile << "@SP\nA=M\nM=D\n";                            // *SP = *addr
                oFile << "@SP\nM=M+1\n";                               // SP++
            }
            else if (command == C_POP)
            {
                oFile << "@THIS\nD=M\n@" << index << "\nD=D+A\n@addr\nM=D\n"; // addr = segmentPointer + i
                oFile << "@SP\nM=M-1\n";
                oFile << "@SP\nA=M\nD=M\n@addr\nA=M\nM=D\n";
            }
        }
        else if (segment == "that")
        {
            if (command == C_PUSH)
            {
                oFile << "@THAT\nD=M\n@" << index << "\nA=D+A\nD=M\n"; // addr = segmentPointer + i
                oFile << "@SP\nA=M\nM=D\n";                            // *SP = *addr
                oFile << "@SP\nM=M+1\n";                               // SP++
            }
            else if (command == C_POP)
            {
                oFile << "@THAT\nD=M\n@" << index << "\nD=D+A\n@addr\nM=D\n"; // addr = segmentPointer + i
                oFile << "@SP\nM=M-1\n";                                      // SP--
                oFile << "@SP\nA=M\nD=M\n@addr\nA=M\nM=D\n";                  // *addr = *SP
            }
        }
        else if (segment == "temp")
        {
            if (command == C_PUSH)
            {
                oFile << "@5\nD=A\n@" << index << "\nA=D+A\nD=M\n"; // addr = segmentPointer + i
                oFile << "@SP\nA=M\nM=D\n";                         // *SP = *addr
                oFile << "@SP\nM=M+1\n";                            // SP++
            }
            else if (command == C_POP)
            {
                oFile << "@5\nD=A\n@" << index << "\nD=D+A\n@addr\nM=D\n"; // addr = segmentPointer + i
                oFile << "@SP\nM=M-1\n";
                oFile << "@SP\nA=M\nD=M\n@addr\nA=M\nM=D\n";
            }
        }
        else if (segment == "constant")
        {
            oFile << "@" << index << "\nD=A\n@SP\nA=M\nM=D\n"; // *SP = i
            oFile << "@SP\nM=M+1\n";                           // SP++;
        }
        else if (segment == "pointer")
        {
            if (index == 0)
            {
                if (command == C_PUSH)
                {
                    oFile << "@THIS\nD=M\n@SP\nA=M\nM=D\n";
                    oFile << "@SP\nM=M+1\n";
                }
                else
                {
                    oFile << "@SP\nM=M-1\nA=M\nD=M\n";
                    oFile << "@THIS\nM=D\n";
                }
            }
            else
            {
                if (command == C_PUSH)
                {
                    oFile << "@THAT\nD=M\n@SP\nA=M\nM=D\n";
                    oFile << "@SP\nM=M+1\n";
                }
                else
                {
                    oFile << "@SP\nM=M-1\nA=M\nD=M\n";
                    oFile << "@THAT\nM=D\n";
                }
            }
        }
        else if (segment == "static")
        {
            if (command == C_PUSH)
            {
                oFile << "@" << outputFile << "." << index << "\nD=M\n@SP\nA=M\nM=D\n";
                oFile << "@SP\nM=M+1\n";
            }
            else
            {
                oFile << "@SP\nM=M-1\nA=M\nD=M\n";
                oFile << "@" << outputFile << "." << index << "\nM=D\n";
            }
        }
    }
    // Closes the output file
    void close()
    {
        oFile.close();
    }

    void a_Instruction(string str)
    {
        oFile << "@" << str << endl;
    }
    void a_Instruction(int i)
    {
        oFile << "@" << i << endl;
    }
};

int main(int argc, char *argv[])
{
    string line = argv[1];
    string iFile = argv[1];
    int found1 = line.find_last_of('.');
    line = line.substr(0, found1);
    line += ".asm";
    string oFile = line;
    cout << iFile << endl
         << oFile << endl;
    Parser parser(iFile);
    CodeWriter codeWriter(oFile);
    while (parser.hasMoreCommands())
    {
        if (parser.commandType() == C_ARITHMETIC)
        {
            codeWriter.writeArithmetic(parser.arg1());
        }
        else if (parser.commandType() == C_PUSH || parser.commandType() == C_POP)
        {
            codeWriter.writePushPop(parser.commandType(), parser.arg1(), parser.arg2());
        }
        parser.advance();
    }
    codeWriter.close();
}