

/******************************************************************************/
/* Functions provided */
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