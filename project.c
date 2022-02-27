/* 
CSCI-2500 Fall 2021 'The Logical Conclusion'
The Char Stars Group
*/

/******************************************************************************/
/* Usual suspects to include  */
/******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// define BIT type as a char (i.e., one byte)
typedef char BIT;
#define TRUE 1
#define FALSE 0
#define UNDEF -1

// useful constants
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
/* Functions provided for your convenience */
/******************************************************************************/
BIT not_gate(BIT A)
{
  if (A)
    return FALSE;
  else
    return TRUE;
}

BIT or_gate(BIT A, BIT B)
{
  if (A || B)
    return TRUE;
  else
    return FALSE;
}

BIT or_gate3(BIT A, BIT B, BIT C)
{
  return or_gate(A, or_gate(B, C));
}

BIT or_gate4(BIT A, BIT B, BIT C, BIT D)
{
  return or_gate(or_gate(A, or_gate(B, C)), D);
}

BIT or_gate5(BIT A, BIT B, BIT C, BIT D, BIT E)
{
  return or_gate(or_gate(or_gate(A, or_gate(B, C)), D), E);
}

BIT or_gate6(BIT A, BIT B, BIT C, BIT D, BIT E, BIT F)
{
  return or_gate(or_gate(or_gate(or_gate(A, or_gate(B, C)), D), E), F);
}


BIT and_gate(BIT A, BIT B)
{
  if (A && B)
    return TRUE;
  else
    return FALSE;
}

BIT and_gate3(BIT A, BIT B, BIT C)
{
  return and_gate(A, and_gate(B, C));
}

BIT and_gate4(BIT A, BIT B, BIT C, BIT D){
  return and_gate(and_gate(A, B), and_gate(C, D));
}

BIT and_gate6(BIT A, BIT B, BIT C, BIT D, BIT E, BIT F){
  return and_gate3(and_gate(A,B),and_gate(C,D),and_gate(E,F));
}

BIT and_gate8(BIT A, BIT B, BIT C, BIT D, BIT E, BIT F, BIT G, BIT H)
{
  return and_gate(and_gate(and_gate(A, B), and_gate(C, D)), and_gate(and_gate(E, F), and_gate(G, H)));
}

BIT xor_gate(BIT A, BIT B)
{
  if (A ^ B)
    return TRUE;
  else
    return FALSE;
}

BIT nor_gate(BIT A, BIT B)
{
  return not_gate(or_gate(A, B));
}

BIT nand_gate(BIT A, BIT B)
{
  return not_gate(and_gate(A, B));
}

void decoder2(BIT I0, BIT I1, BIT* O0, BIT* O1, BIT* O2, BIT* O3)
{
  *O0 = and_gate(not_gate(I1), not_gate(I0));
  *O1 = and_gate(not_gate(I1), I0);
  *O2 = and_gate(I1, not_gate(I0));
  *O3 = and_gate(I1, I0);
  
  return;
}

BIT multiplexor2(BIT S, BIT I0, BIT I1)
{
  BIT x0 = and_gate(not_gate(S), I0);
  BIT x1 = and_gate(S, I1);
  return or_gate(x0, x1);  
}

void multiplexor2_32(BIT S, BIT* I0, BIT* I1, BIT* Output)
{
  for (int i = 0; i < 32; ++i) {
    BIT x0 = and_gate(not_gate(S), I0[i]);
    BIT x1 = and_gate(S, I1[i]);
    Output[i] = or_gate(x0, x1);
  }
}

BIT multiplexor4(BIT S0, BIT S1, BIT I0, BIT I1, BIT I2, BIT I3)
{
  BIT x0, x1, x2, x3 = FALSE;
  decoder2(S0, S1, &x0, &x1, &x2, &x3);
  
  BIT y0 = and_gate(x0, I0);
  BIT y1 = and_gate(x1, I1);
  BIT y2 = and_gate(x2, I2);
  BIT y3 = and_gate(x3, I3);
  
  BIT z0 = or_gate(y0, y1);
  BIT z1 = or_gate(y2, y3);
  
  return or_gate(z0, z1);  
}


/******************************************************************************/
/* Helper functions */
/******************************************************************************/
void copy_bits(BIT* A, BIT* B)
{
  for (int i = 0; i < 32; ++i)
    B[i] = A[i]; 
}

void copy_bits_16(BIT* A, BIT* B)
{
  for (int i = 0; i < 16; ++i)
    B[i] = A[i]; 
}

void copy_bits_26(BIT* A, BIT* B)
{
  for (int i = 0; i < 26; ++i)
    B[i] = A[i]; 
}

void print_binary(BIT* A)
{
  for (int i = 31; i >= 0; --i)
    printf("%d", A[i]); 
}

void convert_to_binary(int a, BIT* A, int length)
{
  convert_to_binary_char(a, A, 32);

}

void convert_to_binary_char(int a, char* A, int length)
{
  if (a >= 0) {
    for (int i = 0; i < length; ++i) {
      A[i] = (a % 2 == 1 ? '1' : '0');
      a /= 2;
    }
  } else {
    a += 1;
    for (int i = 0; i < length; ++i) {
      A[i] = (a % 2 == 0 ? '1' : '0');
      a /= 2;
    }
  }

}
  
int binary_to_integer(BIT* A)
{
  unsigned a = 0;
  unsigned mult = 1;
  
  for (int i = 0; i < 32; ++i) {
    a += A[i]*mult;
    mult *= 2;
  }
  
  return (int)a;
}

int binary5_to_integer(BIT* A)
{
  unsigned a = 0;
  unsigned mult = 1;
  
  for (int i = 0; i < 5; ++i) {
    a += A[i]*mult;
    mult *= 2;
  }
  
  return (int)a;
}
int binary4_to_integer(BIT* A)
{
  unsigned a = 0;
  unsigned mult = 1;
  
  for (int i = 0; i < 4; ++i) {
    a += A[i]*mult;
    mult *= 2;
  }
  
  return (int)a;
}

/******************************************************************************/
/* Custom Helper functions */
/******************************************************************************/

void lookahead_adder(BIT* A, BIT* B, BIT* SUM, BIT do_it){
    char G[32] = {0};
    char P[32] = {0};
    char C[32]  = {0};

    for (int i = 0; i < 32; i++){
        G[i] = and_gate(A[i], B[i]); // Gi = Ai * Bi
        P[i] = or_gate(A[i], B[i]); // Pi = Ai + Bi
        SUM[i] = multiplexor2(do_it, SUM[i], xor_gate(C[i], xor_gate(A[i], B[i]))); // Writes to output if do_it is 1
        C[i+1] = or_gate(and_gate(P[i], C[i]), G[i]); // Ci+1 = Gi + Pi * Ci
    }
}

void multiplexor4_32(BIT S0, BIT S1, BIT* I0, BIT* I1, BIT* I2, BIT* I3, BIT* Output){
    BIT Mux1Result[32] = {FALSE};
    BIT Mux2Result[32] = {FALSE};
    multiplexor2_32(S1, I0, I1, Mux1Result);
    multiplexor2_32(S1, I2, I3, Mux2Result);
    multiplexor2_32(S0, Mux1Result, Mux2Result, Output);
}

void decoder3(BIT* I, BIT EN, BIT* O){
  if(EN == FALSE){
    int i = 0;
    for(i =0; i< 8; i++){
      O[i] = 0;
    }
  }else{
      O[0] = and_gate(and_gate(not_gate(I[0]), not_gate(I[1])),not_gate(I[2]));
      O[1] = and_gate(and_gate(I[0], not_gate(I[1])),not_gate(I[2]));
      O[2] = and_gate(and_gate(not_gate(I[0]), I[1]),not_gate(I[2]));
      O[3] = and_gate(and_gate(I[0], I[1]),not_gate(I[2]));
      O[4] = and_gate(and_gate(not_gate(I[0]), not_gate(I[1])),I[2]);
      O[5] = and_gate(and_gate(I[0], not_gate(I[1])),I[2]);
      O[6] = and_gate(and_gate(not_gate(I[0]), I[1]),I[2]);
      O[7] = and_gate(and_gate(I[0], I[1]),I[2]);
    }

  return;
}

void decoder5(BIT* I, BIT* O){
  BIT O1 = FALSE;
  BIT O2 = FALSE;
  BIT O3 = FALSE;
  BIT O4 = FALSE;
  decoder2(I[3],I[4],&O1,&O2,&O3,&O4);

  BIT inpot_3[3];
  int i = 0;
  for(i = 0; i < 3; i++){
    inpot_3[i] = I[i];
  }
  BIT out_put1[8] = {FALSE};

  BIT out_put2[8] = {FALSE};

  BIT out_put3[8] = {FALSE};

  BIT out_put4[8] = {FALSE};

  decoder3(inpot_3,O1,out_put1);
  decoder3(inpot_3,O2,out_put2);
  decoder3(inpot_3,O3,out_put3);
  decoder3(inpot_3,O4,out_put4);
  int count = 0;
  for(i = 0; i < 8; i ++){
    O[i] = out_put1[count];
    count++;
  }
  count = 0;
  for(i = 8; i < 16; i ++){
    O[i] = out_put2[count];
    count++;
  }
  count = 0;
  for(i = 16; i < 24; i ++){
    O[i] = out_put3[count];
    count++;
  }
  count = 0;
  for(i = 24; i < 32; i ++){
    O[i] = out_put4[count];
    count++;
  }
  // https://fci.stafpu.bu.edu.eg/Computer%20Science/4887/crs-12801/Files/hw4-solution.pdf
  
  return;
}

/******************************************************************************/
/* Parsing functions */
/******************************************************************************/

void set_register(char* input, char* output)
{
  char *reg = "";
  if (strcmp(input, "zero") == 0)
    reg = "00000";
  else if (strcmp(input, "v0") == 0)
    reg = "01000";
  else if (strcmp(input, "a0") == 0)
    reg = "00100";
  else if (strcmp(input, "t0") == 0)
    reg = "00010";
  else if (strcmp(input, "t1") == 0)
    reg = "10010";
  else if (strcmp(input, "s0") == 0)
    reg = "00001";
  else if (strcmp(input, "s1") == 0)
    reg = "10001";
  else if (strcmp(input, "sp") == 0)
    reg = "10111";
  else if (strcmp(input, "ra") == 0)
    reg = "11111";
  strncpy(output, reg, 5);
}

char instructions_strings[10][30];

int get_instructions(BIT Instructions[][32])
{
  char line[256] = {0};
  int instruction_count = 0;
  while (fgets(line, 256, stdin) != NULL) {
    strcpy(instructions_strings[instruction_count], line);
    // TODO: perform conversion of instructions to binary
    // Input: coming from stdin via: ./a.out < input.txt
    // Output: Convert instructions to binary in the instruction memory
    // Return: Total number of instructions
    // Note: you are free to use if-else and external libraries here
    // Note: you don't need to implement circuits for saving to inst. mem.
    char inst[256] = {0};
    char op1[256] = {0};
    char op2[256] = {0};
    char op3[256] = {0};
    sscanf(line, "%s %s %s %s", inst, op1, op2, op3);
    // inst op1 op2 op3
    
    char temp_output[33] = {0};
    char rs[6] = {0};   // register source
    char rt[6] = {0};   // register one
    char rd[6] = {0};   // register two
    char imm[17] = {0}; // immediate
    char address[27] = {0};

  // I: same except different opcodes
    // op reg1 reg2 offset
    if (strcmp(inst, "lw") == 0 || 
        strcmp(inst, "sw") == 0 ||
        strcmp(inst, "beq") == 0 ||
        strcmp(inst, "addi") == 0) {
      convert_to_binary_char(atoi(op3), imm, 16);
      if (strcmp(inst, "beq") == 0){
        set_register(op1, rs);
        set_register(op2, rt);
      }else{
        set_register(op1, rt);
        set_register(op2, rs);
      }
      strncpy(&temp_output[0], imm, 16);
      strncpy(&temp_output[16], rt, 5);
      strncpy(&temp_output[21], rs, 5);
      char *op__code = "";
      if (strcmp(inst, "lw") == 0)
        op__code = "110001";
      else if (strcmp(inst, "sw") == 0)
        op__code = "110101";
      else if (strcmp(inst, "beq") == 0)
        op__code = "001000";
      else if (strcmp(inst, "addi") == 0)
        op__code = "000100";
      strncpy(&temp_output[26], op__code, 6);
  // R: opcode is 0, different functs, shamt is 0
    // op reg1 reg2 reg3
    } else if (strcmp(inst, "and") == 0 ||
               strcmp(inst, "or") == 0 ||
               strcmp(inst, "add") == 0 ||
               strcmp(inst, "sub") == 0 ||
               strcmp(inst, "slt") == 0) {
      set_register(op1, rd);
      set_register(op2, rs);
      set_register(op3, rt);
      char *__funct = "";
      if (strcmp(inst, "and") == 0)
        __funct = "001001";
      else if (strcmp(inst, "or") == 0)
        __funct = "101001";
      else if (strcmp(inst, "add") == 0)
        __funct = "000001";
      else if (strcmp(inst, "sub") == 0)
        __funct = "010001";
      else if (strcmp(inst, "slt") == 0)
        __funct = "010101";
      strncpy(&temp_output[0], __funct, 6);

      char *__shamt = "00000";
      strncpy(&temp_output[6], __shamt, 5);
      strncpy(&temp_output[11], rd, 5);
      strncpy(&temp_output[16], rt, 5);
      strncpy(&temp_output[21], rs, 5);
      char *op__code = "000000";
      strncpy(&temp_output[26], op__code, 6);
    }else if (strcmp(inst, "jr") == 0){
      set_register(op1, rs);
      char *__funct = "00010";
      strncpy(&temp_output[0], __funct, 6);
      char *__five_zeros = "00000";
      strncpy(&temp_output[6], __five_zeros, 5);
      strncpy(&temp_output[11], __five_zeros, 5);
      strncpy(&temp_output[16], __five_zeros, 5);
      strncpy(&temp_output[21], rs, 5);
      char *op__code = "000000";
      strncpy(&temp_output[26], op__code, 6);
  // J:
    // op address
    } else if (strcmp(inst, "j") == 0 ||
               strcmp(inst, "jal") == 0) {
      convert_to_binary_char(atoi(op1), address, 26);
      strncpy(&temp_output[0], address, 26);
      char *op__code = "";
      if (strcmp(inst, "j") == 0)
        op__code = "010000";
      else if (strcmp(inst, "jal") == 0)
        op__code = "110000";
      strncpy(&temp_output[26], op__code, 6);
    }

    // put in correct line of instruction memory
    // designated by instruction_count
    for (int i = 0; i < 32; ++i)
      Instructions[instruction_count][i] = (temp_output[i] == '1' ? TRUE : FALSE); 
    
    instruction_count++;
  }
  return instruction_count;
}


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
/* Functions that you will implement */
/******************************************************************************/
void Instruction_Memory(BIT* ReadAddress, BIT* Instruction)
{
  // TODO: Implement instruction memory
  // Input: 32-bit instruction address
  // Output: 32-bit binary instruction
  // Note: Useful to use a 5-to-32 decoder here
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
  // TODO: Set control bits for everything
  // Input: opcode field from the instruction
  // OUtput: all control lines get set 
  // Note: Can use SOP or similar approaches to determine bits

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
  // RegDst
  RegDst[1] = not_gate(or_gate3(is_lw, is_addi, is_jal));
  // Jump
  *Jump = or_gate(is_j, is_jal);
  // Branch
  *Branch = is_beq;
  // MemRead 
  *MemRead = is_lw;
  // MemToReg 
  // MemToReg[1]
  MemToReg[1] = is_lw;
  // ALUOp
  ALUOp[0] = is_R;
  ALUOp[1] = is_beq;
  // MemWrite
  *MemWrite = is_sw;
  // ALUSrc
  *ALUSrc = or_gate3(is_lw, is_sw, is_addi);
  // RegWrite
  *RegWrite = not_gate(or_gate3(is_sw, is_beq, is_j));
  // JumpAndLink
  MemToReg[0] = is_jal; // gives PC + 4 to write data
  RegDst[0] = is_jal; // gives 31 to write register
}

void Read_Register(BIT* ReadRegister1, BIT* ReadRegister2,
  BIT* ReadData1, BIT* ReadData2)
{
  // TODO: Implement register read functionality
  // Input: two 5-bit register addresses
  // Output: the values of the specified registers in ReadData1 and ReadData2
  // Note: Implementation will be very similar to instruction memory circuit
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
  // TODO: Implement register write functionality
  // Input: one 5-bit register address, data to write, and control bit
  // Output: None, but will modify register file
  // Note: Implementation will again be similar to those above
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

void ALU_Control(BIT* ALUOp, BIT* funct, BIT* ALUControl)
{
  // TODO: Implement ALU Control circuit
  // Input: 2-bit ALUOp from main control circuit, 6-bit funct field from the
  //        binary instruction
  // Output:4-bit ALUControl for input into the ALU
  // Note: Can use SOP or similar approaches to determine bits
  
  BIT is_lw = and_gate(not_gate(ALUOp[1]), not_gate(ALUOp[0]));
  BIT is_sw = and_gate(not_gate(ALUOp[1]), not_gate(ALUOp[0]));
  BIT is_beq = and_gate((ALUOp[1]), not_gate(ALUOp[0]));
  BIT is_add = and_gate8(not_gate(ALUOp[1]), ALUOp[0], (funct[5]), 
    not_gate(funct[4]), not_gate(funct[3]), not_gate(funct[2]),
    not_gate(funct[1]), not_gate(funct[0]));
  BIT is_sub = and_gate8(not_gate(ALUOp[1]), ALUOp[0], (funct[5]), 
    not_gate(funct[4]), not_gate(funct[3]), not_gate(funct[2]), (funct[1]), 
    not_gate(funct[0]));
  BIT is_or = and_gate8(not_gate(ALUOp[1]), ALUOp[0], funct[5], 
    not_gate(funct[4]), not_gate(funct[3]), (funct[2]), not_gate(funct[1]), 
    funct[0]);
  BIT is_slt = and_gate8(not_gate(ALUOp[1]), (ALUOp[0]), (funct[5]), 
    not_gate(funct[4]), (funct[3]), not_gate(funct[2]), (funct[1]), not_gate(funct[0]));

  ALUControl[0] = 0;
  ALUControl[1] = or_gate3(is_beq, is_sub, is_slt);
  ALUControl[2] = or_gate6(is_lw, is_sw, is_beq, is_add, is_sub, is_slt);
  ALUControl[3] = or_gate(is_or, is_slt);
}

void adder1(BIT A, BIT B, BIT CarryIn, BIT* CarryOut, BIT* Sum)
{
  // TODO: implement a 1-bit adder
  // Note: you can probably copy+paste this from your (or my) Lab 5 solution

  BIT x0 = xor_gate(A, B);
  *Sum = xor_gate(CarryIn, x0);

  BIT y0 = and_gate(x0, CarryIn);
  BIT y1 = and_gate(A, B);
  *CarryOut = or_gate(y0, y1);
}

void ALU1(BIT A, BIT B, BIT Binvert, BIT CarryIn, BIT Less, 
  BIT Op0, BIT Op1, BIT* Result, BIT* CarryOut, BIT* Set)
{
  // TODO: implement a 1-bit ALU 
  // Note: this will need modifications from Lab 5 to account for 'slt'
  // See slide "MSB ALU" in csci2500-f21-ch03a-slides.pdf
  
  BIT x0 = multiplexor2(Binvert, B, not_gate(B));
  
  BIT y0 = and_gate(A, x0);
  BIT y1 = or_gate(A, x0);
  
  BIT y2 = FALSE;
  adder1(A, x0, CarryIn, CarryOut, &y2); 
  *Set = y2;
  
  BIT y3 = Less; 
  
  *Result = multiplexor4(Op0, Op1, y0, y1, y2, y3);
}

void ALU32(BIT* A, BIT* B, BIT Binvert, BIT CarryIn, 
  BIT Op0, BIT Op1, BIT* Result, BIT* CarryOut)
{
  // TODO: implement a 32-bit ALU
  // You'll need to essentially implement a 32-bit ripple adder here
  // See slide "New 32-bit ALU" in csci2500-f21-ch03a-slides.pdf

  BIT Less = FALSE;
  BIT Set = FALSE;
  ALU1(A[0], B[0], Binvert, CarryIn, Less, 
    Op0, Op1, &Result[0], CarryOut, &Set);
  for (int i = 1; i < 32; ++i) {
    ALU1(A[i], B[i], Binvert, *CarryOut, Less, 
      Op0, Op1, &Result[i], CarryOut, &Set);
  }
  
  Less = Set;
  ALU1(A[0], B[0], Binvert, CarryIn, Less, 
    Op0, Op1, &Result[0], CarryOut, &Set);
}

BIT is_zero(BIT* Input){
  BIT Result = TRUE;
  for (int i = 0; i < 32; i++){
    Result = and_gate(Result, not_gate(Input[i]));
  }
  return Result;
}

void ALU(BIT* ALUControl, BIT* Input1, BIT* Input2, BIT* Zero, BIT* Result)
{
  // Input: 4-bit ALUControl, two 32-bit inputs
  // Output: 32-bit result, and zero flag big
  // Note: Can re-use prior implementations (but need new circuitry for zero)
  BIT is_or = and_gate4(not_gate(ALUControl[0]), not_gate(ALUControl[1]), not_gate(ALUControl[2]), (ALUControl[3]));
  BIT is_add = and_gate4(not_gate(ALUControl[0]), not_gate(ALUControl[1]), (ALUControl[2]), not_gate(ALUControl[3]));
  BIT is_sub = and_gate4(not_gate(ALUControl[0]), (ALUControl[1]), (ALUControl[2]), not_gate(ALUControl[3]));
  BIT is_slt = and_gate4(not_gate(ALUControl[0]), (ALUControl[1]), (ALUControl[2]), (ALUControl[3]));

  BIT Binvert = or_gate(is_sub, is_slt);
  BIT Op0 = or_gate(is_or, is_slt);
  BIT Op1 = or_gate3(is_add, is_sub, is_slt);

  BIT CarryIn = or_gate3(is_or, is_slt, is_sub);
  BIT CarryOut = FALSE;
  ALU32(Input1, Input2, Binvert, CarryIn, Op0, Op1, Result, &CarryOut);
  *Zero = is_zero(Result);
}


void Data_Memory(BIT MemWrite, BIT MemRead, 
  BIT* Address, BIT* WriteData, BIT* ReadData)
{
  // Input: 32-bit address, control flags for read/write, and data to write
  // Output: data read if processing a lw instruction
  // Note: Implementation similar as above
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

void Extend_Sign16(BIT* Input, BIT* Output)
{
  // TODO: Implement 16-bit to 32-bit sign extender
  // Copy Input to Output, then extend 16th Input bit to 17-32 bits in Output
  copy_bits_16(Input, Output);
  for (int i = 16; i < 32; i++){
    Output[i] = Input[15];
  }
}

void Extend_Sign26(BIT* Input, BIT* Output)
{
  // TODO: Implement 16-bit to 32-bit sign extender
  // Copy Input to Output, then extend 16th Input bit to 17-32 bits in Output
  copy_bits_26(Input, Output);
  for (int i = 26; i < 32; i++){
    Output[i] = Input[25];
  }
}

void updateState()
{
  // TODO: Implement the full datapath here
  // Essentially, you'll be figuring out the order in which to process each of 
  // the sub-circuits comprising the entire processor circuit. It makes it 
  // easier to consider the pipelined version of the process, and handle things
  // in order of the pipeline. The stages and operations are:
  // Fetch - load instruction from instruction memory
  BIT Instruction[32] = {FALSE};
  Instruction_Memory(PC, Instruction);
  BIT is_jr = and_gate(and_gate6(not_gate(Instruction[26]), not_gate(Instruction[27]), not_gate(Instruction[28]), 
                       not_gate(Instruction[29]), not_gate(Instruction[30]), not_gate(Instruction[31])), 
                       and_gate6(not_gate(Instruction[0]), not_gate(Instruction[1]), not_gate(Instruction[2]), 
                       (Instruction[3]), not_gate(Instruction[4]), not_gate(Instruction[5])));
  // Decode - set control bits and read from the register file
  Control(Instruction + 26, RegDst, &Jump, &Branch, &MemRead, MemToReg, ALUOp, &MemWrite, &ALUSrc, &RegWrite);
  BIT ReadData1[32] = {FALSE};
  BIT ReadData2[32] = {FALSE};
  Read_Register(Instruction + 21, Instruction + 16, ReadData1, ReadData2);
  // Execute - process ALU
  BIT ExtendedImm[32] = {FALSE};
  Extend_Sign16(Instruction, ExtendedImm);
  ALU_Control(ALUOp, Instruction, ALUControl);
  BIT ALUSrcMuxOutput[32] = {FALSE};
  multiplexor2_32(ALUSrc, ReadData2, ExtendedImm, ALUSrcMuxOutput);
  BIT ALUResult[32] = {FALSE};
  ALU(ALUControl, ReadData1, ALUSrcMuxOutput, &Zero, ALUResult);
  // Memory - read/write data memory
  BIT ReadDataMem[32] = {FALSE};
  Data_Memory(MemWrite, MemRead, ALUResult, ReadData2, ReadDataMem);
  // Write Back - write to the register file
  BIT WriteRegister[32] = {FALSE};
  multiplexor4_32(RegDst[0], RegDst[1], Instruction + 16, Instruction + 11, REG_THIRTY_ONE, ZERO, WriteRegister);
  BIT WriteData[32] = {FALSE};
  BIT NewPC[32] = {FALSE};
  lookahead_adder(PC, ONE, NewPC, TRUE);
  multiplexor4_32(MemToReg[0], MemToReg[1], ALUResult, ReadDataMem, NewPC, ZERO, WriteData);
  Write_Register(RegWrite, WriteRegister, WriteData);
  // Update PC - determine the final PC value for the next instruction
  BIT BranchPC[32] = {FALSE};
  lookahead_adder(NewPC, ExtendedImm, BranchPC, TRUE);
  BIT ExtendedAddress[32] = {FALSE};
  Extend_Sign26(Instruction, ExtendedAddress);
  multiplexor2_32(Jump, NewPC, ExtendedAddress, PC);
  multiplexor2_32(is_jr, PC, ReadData1, PC);
  multiplexor2_32(and_gate(Branch, Zero), PC, BranchPC, PC);
}

/******************************************************************************/
/* Main */
/******************************************************************************/

int main()
{
  setbuf(stdout, NULL);
    
  // parse instructions into binary format
  int counter = get_instructions(MEM_Instruction);
  
  // load program and run
  copy_bits(ZERO, PC);
  copy_bits(THIRTY_TWO, MEM_Register[29]);
  
  while (binary_to_integer(PC) < counter) {
    print_instruction();
    updateState();
    print_state();
  }

  return 0;
}



