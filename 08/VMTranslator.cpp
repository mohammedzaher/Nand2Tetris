#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <dirent.h>
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
    string _arg1 = "", _arg0 = "", inFile = "";
    int _arg2 = 0;
    bool flag = true;
    vector<string> v;
    string line;

public:
    // Constructor Opens the input file
    Parser(vector<string> &inFiles)
    {
        v = inFiles;
        // Opens input file
        inFile = v.back();
        v.pop_back();
        iFile.open(inFile);
        if (!iFile.is_open())
        {
            cout << "Error opening input file Parser!" << endl;
            return;
        }
        advance();
    }
    ~Parser()
    {
        iFile.close();
    }
    // Are there more commands in the input?
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
        else if (line == "add" || line == "sub" || line == "neg" || line == "eq" || line == "gt" || line == "lt" || line == "and" || line == "or" || line == "not")
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
            int found2 = line.find(' ', found1 + 1);
            _arg1 = line.substr(found1 + 1, found2 - found1 - 1);
            if (found2 == -1)
            {
                _arg1 = line.substr(found1 + 1);
            }
        }
        return _arg1;
    }
    // Returns the second argument of the current command.
    int arg2()
    {
        _arg2 = 0;
        int found1 = line.find(' ');
        int found2 = line.find(' ', found1 + 1);
        _arg2 = stoi(line.substr(found2 + 1));
        return _arg2;
    }

    void advance()
    {
        // Ignores Inline Comments
        flag = (bool)getline(iFile, line);
        if (!flag)
        {
            if (hasMoreFiles())
            {
                inFile = v.back();
                v.pop_back();
                if (iFile.is_open())
                {
                    iFile.close();
                    iFile.open(inFile);
                    flag = true;
                }
            }
        }
        int found2 = line.find_first_not_of(' ');
        if (found2 != -1)
        {
            if (line[found2] == '/')
            {
                line.erase(0, found2 - 1);
            }
        }
        int found = line.find("/");
        while ((found == 0 || line.empty()) && flag)
        {
            flag = (bool)getline(iFile, line);
            found2 = line.find_first_not_of(' ');
            if (found2 != -1)
            {
                if (line[found2] == '/')
                {
                    line.erase(0, found2 - 1);
                }
            }
            found = line.find("/");
        }
        if (found != -1)
        {
            line = line.substr(0, found);
            found = line.find_last_not_of(' ');
            line = line.substr(0, found + 1);
        }
        else
        {
            found = line.find_last_not_of(' ');
            line = line.substr(0, found + 1);
        }
    }

    bool hasMoreFiles()
    {
        if (v.size() > 0)
            return true;
        else
            return false;
    }
};

class CodeWriter
{
private:
    ofstream oFile;
    int labelNum = 0;
    string outputFile = "";
    string functionName = "";
    int returnNum = 1;

public:
    /* Begin of CodeWriter Constructor */
    CodeWriter(string outFile)
    {
        // Extracts output file name
        int found = outFile.find_last_of('.');
        outputFile = outFile.substr(0, found);
        found = outFile.find_last_of('/');
        if (found != -1)
        {
            outputFile = outputFile.substr(found + 1);
        }
        // Opens the output file
        oFile.open(outFile);
        if (!oFile.is_open())
        {
            cout << "Error opening the output file CodeWriter" << endl;
            return;
        }
    }
    ~CodeWriter()
    {
        oFile.close();
    }
    /* End of Constructor
     *  Begin of Main Functions */
    void writeArithmetic(const string command)
    {
        if (command.find("add") != -1)
        {
            writeArthAddSub("D+M");
        }
        else if (command.find("sub") != -1)
        {
            writeArthAddSub("M-D");
        }
        else if (command.find("neg") != -1)
        {
            writeArthNegNot("-M");
        }
        else if (command.find("eq") != -1)
        {
            writeArthComp("JEQ");
        }
        else if (command.find("gt") != -1)
        {
            writeArthComp("JGT");
        }
        else if (command.find("lt") != -1)
        {
            writeArthComp("JLT");
        }
        else if (command.find("and") != -1)
        {
            writeArthAndOr("M&D");
        }
        else if (command.find("or") != -1)
        {
            writeArthAndOr("M|D");
        }
        else if (command.find("not") != -1)
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
            writePushConstants(to_string(index), "A");
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

    void writeLabel(string label)
    {
        if (label.back() == 13)
        {
            label.pop_back();
        }
        oFile << '(' << functionName << '&' << label << ')' << "\n";
    }

    void writeGoto(string label)
    {
        if (label != functionName)
        {
            string temp = functionName + "&";
            label = temp + label;
        }
        writeAInstruction(label);
        writeCInstruction("", "0", "JMP");
    }

    void writeIf(string label)
    {
        string temp = functionName + "&";
        label = temp + label;

        writeAInstruction("SP");
        writeCInstruction("AM", "M-1");
        writeCInstruction("D", "M");
        writeAInstruction(label);
        writeCInstruction("", "D", "JNE");
    }

    void writeFunction(string function, int numVars)
    {
        functionName = function;
        oFile << '(' << function << ')' << endl;
        while (numVars > 0)
        {
            writePushConstants("0", "A");
            numVars--;
        }
    }

    void writeCall(string function, int numArgs)
    {
        string returnAddr = "RETURN" + to_string(returnNum);
        returnNum++;

        writePushConstants(returnAddr, "A");
        writePushConstants("LCL");  // saves LCL of the caller
        writePushConstants("ARG");  // saves ARG of the caller
        writePushConstants("THIS"); // saves THIS of the caller
        writePushConstants("THAT"); // saves THAT of the caller
        // repositioning ARG
        writeAInstruction(to_string(numArgs));
        writeCInstruction("D", "A");
        writeAInstruction("SP");
        writeCInstruction("D", "M-D");
        writeAInstruction(to_string(5));
        writeCInstruction("D", "D-A");
        writeAInstruction("ARG");
        writeCInstruction("M", "D");
        // repositioning LCL
        writeAInstruction("SP");
        writeCInstruction("D", "M");
        writeAInstruction("LCL");
        writeCInstruction("M", "D");
        // goto function
        writeAInstruction(function);
        writeCInstruction("", "0", "JMP");
        // declares a label for the return Address
        oFile << "(" << returnAddr << ")" << endl;
    }

    void writeReturn()
    {
        // endframe is temp variable
        writeAInstruction("LCL");
        writeCInstruction("D", "M");
        writeAInstruction("endframe");
        writeCInstruction("M", "D");
        // gets the return address
        writeAInstruction(to_string(5));
        writeCInstruction("D", "A");
        writeAInstruction("endframe");
        writeCInstruction("A", "M-D");
        writeCInstruction("D", "M");
        writeAInstruction("ret");
        writeCInstruction("M", "D");
        // repositions the return value for caller
        writeAInstruction("SP");
        writeCInstruction("M", "M-1");
        writeAInstruction("SP");
        writeCInstruction("A", "M");
        writeCInstruction("D", "M");
        writeAInstruction("ARG");
        writeCInstruction("A", "M");
        writeCInstruction("M", "D");
        // repositions SP for the caller
        writeAInstruction("ARG");
        writeCInstruction("D", "M");
        writeCInstruction("D", "D+1");
        writeAInstruction("SP");
        writeCInstruction("M", "D");
        // restores segments of the caller
        writeReturnRestores("THAT", 1);
        writeReturnRestores("THIS", 2);
        writeReturnRestores("ARG", 3);
        writeReturnRestores("LCL", 4);
        // got to return address
        writeAInstruction("ret");
        writeCInstruction("A", "M");
        writeCInstruction("", "0", "JMP");
    }

    void writeInit()
    {
        writeAInstruction("256");
        writeCInstruction("D", "A");
        writeAInstruction("SP");
        writeCInstruction("M", "D");
        writeCall("Sys.init", 0);
    }
    /* End of Main Functions
     *  Begin of Helper Functions */
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
        string label1 = functionName + "&LABEL" + to_string(labelNum);
        string label2 = functionName + "&LABEL" + to_string(labelNum + 1);
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
        oFile << "(" << label1 << ")" << endl;
        writeAInstruction("SP");
        writeCInstruction("M", "M-1");
        writeCInstruction("A", "M");
        writeCInstruction("A", "A-1");
        writeCInstruction("M", "-1");
        oFile << "(" << label2 << ")" << endl;
        // writeLabel(label2);
        labelNum += 2;
    }

    void writePushConstants(string constant, string option = "M")
    {
        writeAInstruction(constant);
        writeCInstruction("D", option);
        writeAInstruction("SP");
        writeCInstruction("A", "M");
        writeCInstruction("M", "D");
        writeAInstruction("SP");
        writeCInstruction("M", "M+1");
    }

    void writeReturnRestores(string segment, int num)
    {
        writeAInstruction(to_string(num));
        writeCInstruction("D", "A");
        writeAInstruction("endframe");
        writeCInstruction("A", "M-D");
        writeCInstruction("D", "M");
        writeAInstruction(segment);
        writeCInstruction("M", "D");
    }
};

int main(int argc, char *argv[])
{
    string line = argv[1], outFile;
    int found = line.find_last_of(".");
    vector<string> v; // the input files
    bool flag = false;
    if (found == -1)
    {
        flag = true;
        string word = argv[1];
        int found2 = 0;
        line.push_back('/');
        word += ".asm";
        line += word;
        outFile = line;
        // open directory

        struct dirent *pDirent;
        DIR *pDir;

        // Ensure we can open directory.

        pDir = opendir(argv[1]);
        if (pDir == NULL)
        {
            cout << "Cannot open directory" << endl;
            return 1;
        }

        // Process each entry.

        while ((pDirent = readdir(pDir)) != NULL)
        {
            string inputfile = argv[1];
            if (inputfile.back() != '/')
            {
                inputfile.push_back('/');
            }
            string file = pDirent->d_name;
            if (file.find(".vm") != -1)
            {
                cout << inputfile << file << endl;
                inputfile = inputfile.append(file);
                v.push_back(inputfile);
            }
        }

        // close directory
        closedir(pDir);
    }
    else
    {
        v.push_back(line);
        line = line.substr(0, found + 1);
        line = line.append("asm");
        outFile = line;
    }
    Parser parser(v);
    CodeWriter codeWriter(outFile);
    if (flag)
    {
        codeWriter.writeInit();
    }
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
        else if (parser.commandType() == C_GOTO)
        {
            codeWriter.writeGoto(parser.arg1());
        }
        else if (parser.commandType() == C_IF)
        {
            codeWriter.writeIf(parser.arg1());
        }
        else if (parser.commandType() == C_LABEL)
        {
            codeWriter.writeLabel(parser.arg1());
        }
        else if (parser.commandType() == C_FUNCTION)
        {
            codeWriter.writeFunction(parser.arg1(), parser.arg2());
        }
        else if (parser.commandType() == C_CALL)
        {
            codeWriter.writeCall(parser.arg1(), parser.arg2());
        }
        else if (parser.commandType() == C_RETRUN)
        {
            codeWriter.writeReturn();
        }
        parser.advance();
    }
}