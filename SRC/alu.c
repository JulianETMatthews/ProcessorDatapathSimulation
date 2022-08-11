

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