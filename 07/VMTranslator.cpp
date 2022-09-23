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
            writeArthAddSub("D+M");
        }
        else if (command == "sub")
        {
            writeArthAddSub("M-D");
        }
        else if (command == "neg")
        {
            writeArthNegNot("-M");
        }
        else if (command == "eq")
        {
            writeArthComp("JEQ");
        }
        else if (command == "gt")
        {
            writeArthComp("JGT");
        }
        else if (command == "lt")
        {
            writeArthComp("JLT");
        }
        else if (command == "and")
        {
            writeArthAndOr("M&D");
        }
        else if (command == "or")
        {
            writeArthAndOr("M|D");
        }
        else if (command == "not")
        {
            writeArthNegNot("!M");
        }
    }

    void writePushPop(int command, string segment, int index)
    {
        if (segment == "local")
        {
            if (command == C_PUSH)
            {
                writePushSegments("LCL", index);
            }
            else if (command == C_POP)
            {
                writePopSegments("LCL", index);
            }
        }
        else if (segment == "argument")
        {
            if (command == C_PUSH)
            {
                writePushSegments("ARG", index);
            }
            else if (command == C_POP)
            {
                writePopSegments("ARG", index);
            }
        }
        else if (segment == "this")
        {
            if (command == C_PUSH)
            {
                writePushSegments("THIS", index);
            }
            else if (command == C_POP)
            {
                writePopSegments("THIS", index);
            }
        }
        else if (segment == "that")
        {
            if (command == C_PUSH)
            {
                writePushSegments("THAT", index);
            }
            else if (command == C_POP)
            {
                writePopSegments("THAT", index);
            }
        }
        else if (segment == "temp")
        {
            if (command == C_PUSH)
            {
                writeAInstruction("5");
                writeCInstruction("D", "A");
                writeAInstruction(to_string(index));
                writeCInstruction("A", "D+A");
                writeCInstruction("D", "M");
                writeAInstruction("SP");
                writeCInstruction("A", "M");
                writeCInstruction("M", "D");
                writeAInstruction("SP");
                writeCInstruction("M", "M+1");
            }
            else if (command == C_POP)
            {
                writeAInstruction("5");
                writeCInstruction("D", "A");
                writeAInstruction(to_string(index));
                writeCInstruction("D", "D+A");
                writeAInstruction("addr");
                writeCInstruction("M", "D");
                writeAInstruction("SP");
                writeCInstruction("M", "M-1");
                writeAInstruction("SP");
                writeCInstruction("A", "M");
                writeCInstruction("D", "M");
                writeAInstruction("addr");
                writeCInstruction("A", "M");
                writeCInstruction("M", "D");
            }
        }
        else if (segment == "constant")
        {
            writeAInstruction(to_string(index));
            writeCInstruction("D", "A");
            writeAInstruction("SP");
            writeCInstruction("A", "M");
            writeCInstruction("M", "D");
            writeAInstruction("SP");
            writeCInstruction("M", "M+1");
        }
        else if (segment == "pointer")
        {
            string str = "";
            if (index == 0)
            {
                str = "THIS";
            }
            else
            {
                str = "THAT";
            }
            if (command == C_PUSH)
            {
                writePushSegments2(str);
            }
            else
            {
                writePopSegments2(str);
            }
        }
        else if (segment == "static")
        {
            string str = outputFile + "." + to_string(index);
            if (command == C_PUSH)
            {
                writePushSegments2(str);
            }
            else
            {
                writePopSegments2(str);
            }
        }
    }
    // Closes the output file
    void close()
    {
        oFile.close();
    }

    void writeAInstruction(string str)
    {
        oFile << "@" << str << endl;
    }
    void writeCInstruction(string dest, string comp, string jmp = "")
    {
        if (jmp.empty())
        {
            oFile << dest << "=" << comp << endl;
        }
        else if (dest.empty())
        {
            oFile << comp << ";" << jmp << endl;
        }
        else
        {
            oFile << dest << "=" << comp << ";" << jmp;
        }
    }
    void writeLabel(string label)
    {
        oFile << "(" << label << ")" << endl;
    }
    void writePushSegments(string segment, int index)
    {
        writeAInstruction(segment);
        writeCInstruction("D", "M");
        writeAInstruction(to_string(index));
        writeCInstruction("A", "D+A");
        writeCInstruction("D", "M");
        writeAInstruction("SP");
        writeCInstruction("A", "M");
        writeCInstruction("M", "D");
        writeAInstruction("SP");
        writeCInstruction("M", "M+1");
    }
    void writePopSegments(string segment, int index)
    {
        writeAInstruction(segment);
        writeCInstruction("D", "M");
        writeAInstruction(to_string(index));
        writeCInstruction("D", "D+A");
        writeAInstruction("addr");
        writeCInstruction("M", "D");
        writeAInstruction("SP");
        writeCInstruction("M", "M-1");
        writeAInstruction("SP");
        writeCInstruction("A", "M");
        writeCInstruction("D", "M");
        writeAInstruction("addr");
        writeCInstruction("A", "M");
        writeCInstruction("M", "D");
    }
    void writePushSegments2(string segment)
    {
        writeAInstruction(segment);
        writeCInstruction("D", "M");
        writeAInstruction("SP");
        writeCInstruction("A", "M");
        writeCInstruction("M", "D");
        writeAInstruction("SP");
        writeCInstruction("M", "M+1");
    }
    void writePopSegments2(string segment)
    {
        writeAInstruction("SP");
        writeCInstruction("M", "M-1");
        writeCInstruction("A", "M");
        writeCInstruction("D", "M");
        writeAInstruction(segment);
        writeCInstruction("M", "D");
    }
    void writeArthAndOr(string sign)
    {
        writeAInstruction("SP");
        writeCInstruction("A", "M");
        writeCInstruction("A", "A-1");
        writeCInstruction("D", "M");
        writeCInstruction("A", "A-1");
        writeCInstruction("M", sign);
        writeAInstruction("SP");
        writeCInstruction("M", "M-1");
    }
    void writeArthAddSub(string sign)
    {
        writeAInstruction("SP");
        writeCInstruction("A", "M");
        writeCInstruction("A", "A-1");
        writeCInstruction("D", "M");
        writeCInstruction("A", "A-1");
        writeCInstruction("M", sign);
        writeAInstruction("SP");
        writeCInstruction("M", "M-1");
    }
    void writeArthNegNot(string sign)
    {
        writeAInstruction("SP");
        writeCInstruction("A", "M");
        writeCInstruction("A", "A-1");
        writeCInstruction("M", sign);
    }
    void writeArthComp(string jmp)
    {
        string label1 = "LABEL" + to_string(labelNum);
        string label2 = "LABEL" + to_string(labelNum + 1);
        writeAInstruction("SP");
        writeCInstruction("A", "M");
        writeCInstruction("A", "A-1");
        writeCInstruction("D", "M");
        writeCInstruction("A", "A-1");
        writeCInstruction("D", "M-D");
        writeAInstruction(label1);
        writeCInstruction("", "D", jmp);
        writeAInstruction("SP");
        writeCInstruction("M", "M-1");
        writeCInstruction("A", "M");
        writeCInstruction("A", "A-1");
        writeCInstruction("M", "0");
        writeAInstruction(label2);
        writeCInstruction("", "0", "JMP");
        writeLabel(label1);
        writeAInstruction("SP");
        writeCInstruction("M", "M-1");
        writeCInstruction("A", "M");
        writeCInstruction("A", "A-1");
        writeCInstruction("M", "-1");
        writeLabel(label2);
        labelNum += 2;
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