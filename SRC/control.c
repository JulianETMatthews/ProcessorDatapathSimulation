/* 
CSCI-2500 Fall 2021 'The Logical Conclusion'
The Char Stars Group
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <alu.c>
#include <helpers.c>
#include <provided.c>

typedef char BIT;
#define TRUE 1
#define FALSE 0
#define UNDEF -1

BIT ONE[32] = {TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, 
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE};
BIT ZERO[32] = {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE};
BIT FOUR[32] = {FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE};
BIT REG_THIRTY_ONE[5] = {TRUE, TRUE, TRUE, TRUE, TRUE};
BIT THIRTY_TWO[32] = {FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, 
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE};


/******************************************************************************/
/* Function prototypes */
/******************************************************************************/
BIT not_gate(BIT A);
BIT or_gate(BIT A, BIT B);
BIT or_gate3(BIT A, BIT B, BIT C);
BIT or_gate4(BIT A, BIT B, BIT C, BIT D);
BIT or_gate5(BIT A, BIT B, BIT C, BIT D, BIT E);
BIT or_gate6(BIT A, BIT B, BIT C, BIT D, BIT E, BIT F);
BIT and_gate(BIT A, BIT B);
BIT and_gate3(BIT A, BIT B, BIT C);
BIT xor_gate(BIT A, BIT B);
BIT nor_gate(BIT A, BIT B);
BIT nand_gate(BIT A, BIT B);

void decoder2(BIT I0, BIT I1, BIT* O0, BIT* O1, BIT* O2, BIT* O3);
BIT multiplexor2(BIT S, BIT I0, BIT I1);
void multiplexor2_32(BIT S, BIT* I0, BIT* I1, BIT* Output);
BIT multiplexor4(BIT S0, BIT S1, BIT I0, BIT I1, BIT I2, BIT I3);

void copy_bits(BIT* A, BIT* B);
void print_binary(BIT* A);
void convert_to_binary(int a, BIT* A, int length);
void convert_to_binary_char(int a, char* A, int length);
int binary_to_integer(BIT* A);
int binary4_to_integer(BIT* A);
int binary5_to_integer(BIT* A);

int get_instructions(BIT Instructions[][32]);

void Instruction_Memory(BIT* ReadAddress, BIT* Instruction);
void Control(BIT* OpCode,
  BIT* RegDst, BIT* Jump, BIT* Branch, BIT* MemRead, BIT* MemToReg,
  BIT* ALUOp, BIT* MemWrite, BIT* ALUSrc, BIT* RegWrite);
void Read_Register(BIT* ReadRegister1, BIT* ReadRegister2,
  BIT* ReadData1, BIT* ReadData2);
void Write_Register(BIT RegWrite, BIT* WriteRegister, BIT* WriteData);
void ALU_Control(BIT* ALUOp, BIT* funct, BIT* ALUControl);
void ALU(BIT* ALUControl, BIT* Input1, BIT* Input2, BIT* Zero, BIT* Result);
void Data_Memory(BIT MemWrite, BIT MemRead, 
  BIT* Address, BIT* WriteData, BIT* ReadData);
void Extend_Sign16(BIT* Input, BIT* Output);
void Extend_Sign26(BIT* Input, BIT* Output);
void updateState();




/******************************************************************************/
/* Program state - memory spaces, PC, and control */
/******************************************************************************/
BIT PC[32]                  = {FALSE};
BIT MEM_Instruction[32][32] = {FALSE};
BIT MEM_Data[32][32]        = {FALSE};
BIT MEM_Register[32][32]    = {FALSE};

BIT RegDst[2] = {FALSE};
BIT Jump      = FALSE;
BIT Branch    = FALSE;
BIT MemRead   = FALSE;
BIT MemToReg[4] = {FALSE};
BIT ALUOp[2]  = {FALSE};
BIT MemWrite  = FALSE;
BIT ALUSrc    = FALSE;
BIT RegWrite  = FALSE;
BIT Zero      = FALSE;
BIT ALUControl[4] = {FALSE};


void print_instruction()
{
  unsigned pc = binary_to_integer(PC);
  printf("PC: %d\n", pc);
  printf("Instruction: ");
  print_binary(MEM_Instruction[pc]);
  printf("\n");
}

void print_state()
{
  printf("Data: ");
  for (int i = 0; i < 32; ++i) {
    printf("%d ", binary_to_integer(MEM_Data[i]));
  } 
  printf("\n");  
  
  printf("Register: ");
  for (int i = 0; i < 32; ++i) {
    printf("%d ", binary_to_integer(MEM_Register[i]));
  } 
  printf("\n");
}


/******************************************************************************/
/* Control */
/******************************************************************************/
void Instruction_Memory(BIT* ReadAddress, BIT* Instruction)
{
  BIT temp[32] = {FALSE};
  int i = 0;
  decoder5(ReadAddress,temp);
  int j = 0;
  for(i = 0; i < 32; i++){
    for(j = 0; j < 32; j++){
      Instruction[j] = or_gate(and_gate(MEM_Instruction[i][j],temp[i]), and_gate(Instruction[j],not_gate(temp[i])));
    }
  }
}

void Control(BIT* OpCode,
  BIT* RegDst, BIT* Jump, BIT* Branch, BIT* MemRead, BIT* MemToReg,
  BIT* ALUOp, BIT* MemWrite, BIT* ALUSrc, BIT* RegWrite)
{

  BIT is_lw = and_gate6(OpCode[0], OpCode[1], not_gate(OpCode[2]), 
                not_gate(OpCode[3]), not_gate(OpCode[4]), OpCode[5]);
  BIT is_sw = and_gate6((OpCode[0]), (OpCode[1]), not_gate(OpCode[2]), 
                (OpCode[3]), not_gate(OpCode[4]), (OpCode[5]));
  BIT is_beq = and_gate6(not_gate(OpCode[0]), not_gate(OpCode[1]), (OpCode[2]), 
                not_gate(OpCode[3]), not_gate(OpCode[4]), not_gate(OpCode[5]));
  BIT is_j = and_gate6(not_gate(OpCode[0]), (OpCode[1]), not_gate(OpCode[2]), 
                not_gate(OpCode[3]), not_gate(OpCode[4]), not_gate(OpCode[5]));
  BIT is_jal = and_gate6((OpCode[0]), (OpCode[1]), not_gate(OpCode[2]), 
                not_gate(OpCode[3]), not_gate(OpCode[4]), not_gate(OpCode[5]));
  BIT is_R = and_gate6(not_gate(OpCode[0]), not_gate(OpCode[1]), not_gate(OpCode[2]), 
                not_gate(OpCode[3]), not_gate(OpCode[4]), not_gate(OpCode[5]));
  BIT is_addi = and_gate6(not_gate(OpCode[0]), not_gate(OpCode[1]), not_gate(OpCode[2]), 
                (OpCode[3]), not_gate(OpCode[4]), not_gate(OpCode[5]));
  RegDst[1] = not_gate(or_gate3(is_lw, is_addi, is_jal));
  *Jump = or_gate(is_j, is_jal);
  *Branch = is_beq;
  *MemRead = is_lw;
  MemToReg[1] = is_lw;
  ALUOp[0] = is_R;
  ALUOp[1] = is_beq;
  *MemWrite = is_sw;
  *ALUSrc = or_gate3(is_lw, is_sw, is_addi);
  *RegWrite = not_gate(or_gate3(is_sw, is_beq, is_j));
}

void Read_Register(BIT* ReadRegister1, BIT* ReadRegister2,
  BIT* ReadData1, BIT* ReadData2)
{
  BIT temp1[32] = {FALSE};
  decoder5(ReadRegister1,temp1);
  int i = 0;
  int j = 0;
  for(i = 0; i < 32; i++){
    for(j = 0; j < 32; j++){
      ReadData1[j] = or_gate(and_gate(MEM_Register[i][j],temp1[i]), and_gate(ReadData1[j],not_gate(temp1[i])));
    }
  }
  BIT temp2[32] = {FALSE};
  decoder5(ReadRegister2,temp2);
  for(i = 0; i < 32; i++){
    for(j = 0; j < 32; j++){
      ReadData2[j] = or_gate(and_gate(MEM_Register[i][j],temp2[i]), and_gate(ReadData2[j],not_gate(temp2[i])));
    }
  }
}

void Write_Register(BIT RegWrite, BIT* WriteRegister, BIT* WriteData)
{
  BIT temp1[32] = {FALSE};
  decoder5(WriteRegister,temp1);
  int i = 0;
  int j = 0;
  for(i =0; i <32; i ++){
    for(j = 0; j < 32; j++){
      MEM_Register[i][j] = multiplexor2(RegWrite, MEM_Register[i][j], and_gate(RegWrite, or_gate(and_gate(WriteData[j], temp1[i]), and_gate(MEM_Register[i][j], not_gate(temp1[i])))));

    }
  }
}

void Data_Memory(BIT MemWrite, BIT MemRead, 
  BIT* Address, BIT* WriteData, BIT* ReadData)
{
  BIT temp1[32] = {FALSE};
  decoder5(Address,temp1);
  int i = 0;
  int j = 0;
  for(i = 0; i < 32; i++){
    for(j = 0; j < 32; j++){
      ReadData[j] = and_gate(MemRead,or_gate(and_gate(MEM_Data[i][j],temp1[i]), and_gate(ReadData[j],not_gate(temp1[i]))));
    }
  }
  for(i =0; i <32; i ++){
    for(j = 0; j < 32; j++){
      MEM_Data[i][j] = multiplexor2(MemWrite, MEM_Data[i][j], and_gate(MemWrite,or_gate(and_gate(WriteData[j],temp1[i]), and_gate(MEM_Data[i][j],not_gate(temp1[i])))));

    }
  }
}

void updateState()
{
  BIT Instruction[32] = {FALSE};
  Instruction_Memory(PC, Instruction);
  BIT is_jr = and_gate(and_gate6(not_gate(Instruction[26]), not_gate(Instruction[27]), not_gate(Instruction[28]), 
                       not_gate(Instruction[29]), not_gate(Instruction[30]), not_gate(Instruction[31])), 
                       and_gate6(not_gate(Instruction[0]), not_gate(Instruction[1]), not_gate(Instruction[2]), 
                       (Instruction[3]), not_gate(Instruction[4]), not_gate(Instruction[5])));
  Control(Instruction + 26, RegDst, &Jump, &Branch, &MemRead, MemToReg, ALUOp, &MemWrite, &ALUSrc, &RegWrite);
  BIT ReadData1[32] = {FALSE};
  BIT ReadData2[32] = {FALSE};
  Read_Register(Instruction + 21, Instruction + 16, ReadData1, ReadData2);
  BIT ExtendedImm[32] = {FALSE};
  Extend_Sign16(Instruction, ExtendedImm);
  ALU_Control(ALUOp, Instruction, ALUControl);
  BIT ALUSrcMuxOutput[32] = {FALSE};
  multiplexor2_32(ALUSrc, ReadData2, ExtendedImm, ALUSrcMuxOutput);
  BIT ALUResult[32] = {FALSE};
  ALU(ALUControl, ReadData1, ALUSrcMuxOutput, &Zero, ALUResult);
  BIT ReadDataMem[32] = {FALSE};
  Data_Memory(MemWrite, MemRead, ALUResult, ReadData2, ReadDataMem);
  BIT WriteRegister[32] = {FALSE};
  multiplexor4_32(RegDst[0], RegDst[1], Instruction + 16, Instruction + 11, REG_THIRTY_ONE, ZERO, WriteRegister);
  BIT WriteData[32] = {FALSE};
  BIT NewPC[32] = {FALSE};
  lookahead_adder(PC, ONE, NewPC, TRUE);
  multiplexor4_32(MemToReg[0], MemToReg[1], ALUResult, ReadDataMem, NewPC, ZERO, WriteData);
  Write_Register(RegWrite, WriteRegister, WriteData);
  BIT BranchPC[32] = {FALSE};
  lookahead_adder(NewPC, ExtendedImm, BranchPC, TRUE);
  BIT ExtendedAddress[32] = {FALSE};
  Extend_Sign26(Instruction, ExtendedAddress);
  multiplexor2_32(Jump, NewPC, ExtendedAddress, PC);
  multiplexor2_32(is_jr, PC, ReadData1, PC);
  multiplexor2_32(and_gate(Branch, Zero), PC, BranchPC, PC);
}





