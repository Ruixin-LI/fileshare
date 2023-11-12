/*
 * Copyright 2002-2020 Intel Corporation.
 * 
 * This software is provided to you as Sample Source Code as defined in the accompanying
 * End User License Agreement for the Intel(R) Software Development Products ("Agreement")
 * section 1.L.
 * 
 * This software and the related documents are provided as is, with no express or implied
 * warranties, other than those that are expressly stated in the License.
 */

#include <iostream>
#include <fstream>
#include "pin.H"
using std::cerr;
using std::endl;
using std::ios;
using std::ofstream;
using std::string;

ofstream OutFile;


// Vector to store the instruction pairs
std::vector<ADDRINT> instructionPairs;

// Output file stream
std::ofstream outFile;

// The range of the main executable
ADDRINT mainImageLow = 0;
ADDRINT mainImageHigh = 0;

bool startInstrumentation = false;
bool startWinmain = false;

// The running count of instructions is kept here
// make it static to help the compiler optimize docount
static UINT64 icount = 0;

// This function is called before every instruction is executed
// VOID docount() { icount++; }

// This function is called before every instruction is executed
// and prints the current and next instruction address
VOID printInstruction(VOID* ip) {
    instructionPairs.push_back((ADDRINT)ip);
}

// Pin calls this function every time a new instruction is encountered
VOID Instruction(INS ins, VOID* v)
{
    // Get the address of the current instruction
    ADDRINT addr = INS_Address(ins);

    //if (addr == 4204925) {
     //   startWinmain = true;
    //}

    //instructionPairs.push_back((ADDRINT)addr);

    // Insert a call to docount before every instruction, no arguments are passed
    if (addr >= mainImageLow && addr <= mainImageHigh && startInstrumentation) {
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)printInstruction, IARG_INST_PTR, IARG_END);
        //instructionPairs.push_back((ADDRINT)addr);
   }
}

// Image load callback - record the range of the main executable
VOID ImageLoad(IMG img, VOID* v) {
    if (IMG_IsMainExecutable(img)) {
        mainImageLow = IMG_LowAddress(img);
        mainImageHigh = IMG_HighAddress(img);
        startInstrumentation = true;
    }
}

// KNOB< string > KnobOutputFile(KNOB_MODE_WRITEONCE, "pintool", "o", "inscount.out", "specify output file name");

// This function is called when the application exits
VOID Fini(INT32 code, VOID* v)
{
    // Write to a file since cout and cerr maybe closed by the application
    //OutFile.setf(ios::showbase);
    //OutFile << "Count " << icount << endl;
    //OutFile.close();

    for (ADDRINT addr : instructionPairs) {
        outFile << std::hex << addr << std::endl;
    }

    outFile.close();
    
    //system("cmd");
}

/* ===================================================================== */
/* Print Help Message                                                    */
/* ===================================================================== */

INT32 Usage()
{
    cerr << "This tool counts the number of dynamic instructions executed" << endl;
    cerr << endl << KNOB_BASE::StringKnobSummary() << endl;
    return -1;
}

/* ===================================================================== */
/* Main                                                                  */
/* ===================================================================== */
/*   argc, argv are the entire command line: pin -t <toolname> -- ...    */
/* ===================================================================== */

int main(int argc, char* argv[])
{
    // Initialize pin
    if (PIN_Init(argc, argv)) return Usage();
    PIN_InitSymbols();

    //OutFile.open(KnobOutputFile.Value().c_str());
    outFile.open("instructionPairs.txt", std::ios::out | std::ios::trunc);

    // Register Instruction to be called to instrument instructions
    INS_AddInstrumentFunction(Instruction, 0);

    // Register ImageLoad to be called when an image is loaded
    IMG_AddInstrumentFunction(ImageLoad, 0);

    // Register Fini to be called when the application exits
    PIN_AddFiniFunction(Fini, 0);

    // Start the program, never returns
    PIN_StartProgram();

    return 0;
}
