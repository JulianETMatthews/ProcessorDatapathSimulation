


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