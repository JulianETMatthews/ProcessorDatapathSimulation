


/******************************************************************************/
/* Given Helper functions */
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


void Extend_Sign16(BIT* Input, BIT* Output)
{
  copy_bits_16(Input, Output);
  for (int i = 16; i < 32; i++){
    Output[i] = Input[15];
  }
}

void Extend_Sign26(BIT* Input, BIT* Output)
{
  copy_bits_26(Input, Output);
  for (int i = 26; i < 32; i++){
    Output[i] = Input[25];
  }
}