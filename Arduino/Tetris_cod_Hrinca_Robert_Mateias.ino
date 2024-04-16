#include <TM1637Display.h>

#define CLK 13
#define DIN 11
#define SS  10
#define BP 21
#define Y_SEGMENTS   4
#define X_SEGMENTS   2
#define NUM_SEGMENTS (X_SEGMENTS * Y_SEGMENTS)

#define W 7
#define A 4
#define S 5
#define D 6

#define Display_CLK 20
#define Display_IN 19

TM1637Display display(Display_CLK, Display_IN);

char coloane[Y_SEGMENTS][8 * X_SEGMENTS];
char matrice_next_piesa[8];

bool valori[32][16];

struct Punct {
  int x, y;
};

struct Piesa {  
  Punct centru;
  Punct offset[4];
  int rotatie;
  char tip;
};

void assignPunct(Punct& thisPunct, const Punct& altul) {
  thisPunct.x = altul.x;
  thisPunct.y = altul.y;
}

Punct addPuncte(const Punct& punct1, const Punct& punct2) {
  Punct result;
  result.x = punct1.x + punct2.x;
  result.y = punct1.y + punct2.y;
  return result;
}


void assignPiesa(Piesa& thisPiesa, const Piesa& alta) {
  thisPiesa.centru = alta.centru;
  for(int i = 0; i < 4; ++i) {
    thisPiesa.offset[i] = alta.offset[i];
  }
  thisPiesa.rotatie = alta.rotatie;
  thisPiesa.tip = alta.tip;
}

void assignTip(Piesa& thisPiesa, const Piesa& alta) {

  for(int i = 0; i < 4; ++i) {
    thisPiesa.offset[i] = alta.offset[i];
  }

}

Piesa rotestePiesa(Piesa thisPiesa) {
  Piesa rez = thisPiesa;
  rez.centru = thisPiesa.centru;
  if(thisPiesa.tip == 'I')
  {
    assignTip(rez,I_block(rez.rotatie));
  }
  if(thisPiesa.tip == 'J')
  {
    assignTip(rez,J_block(rez.rotatie));
  }

  if(thisPiesa.tip == 'L')
  {
    assignTip(rez,L_block(rez.rotatie));
  }

  if(thisPiesa.tip == 'O')
  {
    assignTip(rez,O_block(rez.rotatie));
  }

  if(thisPiesa.tip == 'S')
  {
    assignTip(rez,S_block(rez.rotatie));
  }
  if(thisPiesa.tip == 'T')
  {
    assignTip(rez,T_block(rez.rotatie));
  }

  if(thisPiesa.tip == 'Z')
  {
    assignTip(rez,Z_block(rez.rotatie));
  }



  if(thisPiesa.rotatie == 3){
    rez.rotatie = 0;
  }

  rez.rotatie ++;
  thisPiesa.rotatie = rez.rotatie;
  
  return rez;
}

Piesa piesa;
Piesa nextPiesa;

int scor = 0;

 
void show() {
  for (char index = 7; index >= 0; --index) {
    digitalWrite(SS, LOW);
    shiftOut(DIN, CLK, MSBFIRST, 8 - index);
    shiftOut(DIN, CLK, MSBFIRST, matrice_next_piesa[index]);

    for (char value = 0; value < Y_SEGMENTS; ++value) {
      shiftOut(DIN, CLK, MSBFIRST, index + 1);
      shiftOut(DIN, CLK, LSBFIRST, coloane[value][8 + index]);
    }
    for (char value = Y_SEGMENTS - 1; value >= 0; --value) {
      shiftOut(DIN, CLK, MSBFIRST, 8 - index);
      shiftOut(DIN, CLK, MSBFIRST, coloane[value][index]);
    }
    digitalWrite(SS, HIGH);
  }
}


void reset() {
  for (int i = 0; i < 8; ++i) {
    matrice_next_piesa[i] = 0x00;
  }
  for (int i = 0; i < Y_SEGMENTS; ++i) {
    for (int j = 0; j < 8 * X_SEGMENTS; ++j) {
      coloane[i][j] = 0x00;
    }
  }
}

void arataPiesa() {
  for (int i = 0; i < 4; i++) {
    Punct pozitie;
    assignPunct(pozitie, addPuncte(piesa.centru, piesa.offset[i]));
    coloane[(31 - pozitie.y) / 8][pozitie.x] |= 1 << (pozitie.y % 8);
  }
}

void arataNextPiesa() {
  Punct centruModificat = {3, 3};
  for (int i = 0; i < 4; i++) {
    Punct pozitie;
    assignPunct(pozitie, addPuncte(centruModificat, nextPiesa.offset[i]));
    matrice_next_piesa[ pozitie.y] |= 1 << (7 - pozitie.x % 8);
  }
}

void arataTabla() {
  reset();
  arataPiesa();
  arataNextPiesa();
  for (int i = 0; i < 32; ++i) {
    for (int j = 0; j < 16; ++j) {
      if (valori[i][j]) {
        coloane[(31 - i) / 8][j] |= 1 << (i % 8);
      }
    }
  }
}


Piesa piesaNoua() {
  static Piesa i_block {{7, 1} ,{{-1, 0}, {0, 0}, {1, 0}, {2, 0}}}; 
  i_block.tip = 'I';
  static Piesa j_block {{7, 1}, {{-1, 0}, {-1, 1}, {0, 1}, {1, 1}} };
  j_block.tip = 'J';
  static Piesa l_block {{7, 1} ,{{0, 1}, {-1, 1}, {1, 0}, {1, 1}}};
  l_block.tip = 'L';
  static Piesa o_block {{7, 1} ,{{0, 0}, {1, 0}, {0, 1}, {1, 1}}};
  o_block.tip = 'O';
  static Piesa s_block {{7, 1},{{0, 0}, {1, 0}, {-1, 1}, {0, 1}}};
  s_block.tip= 'S';
  static Piesa t_block {{7, 1},{{0, 0}, {-1, 1}, {0, 1}, {1, 1}}};
  t_block.tip = 'T';
  static Piesa z_block {{7, 1},{{0, 0}, {-1, 0}, {0, 1}, {1, 1}}};
  z_block.tip = 'Z';

  static Piesa piese[] = {j_block, l_block, o_block, s_block, t_block, z_block, i_block};
  
  return piese[random(6)];
}

Piesa I_block(int x){

  static Piesa i_block_0 {{7,1}, {{-1, 0}, {0, 0}, {1, 0}, {2, 0}} };
  i_block_0.tip = 'I';
  static Piesa i_block_90 {{7,1} ,{{0, -1}, {0, 0}, {0, 1}, {0, 2}} };
  i_block_90.tip = 'I';
  static Piesa i_block_180 {{7,1}, {{-1, 0}, {0, 0}, {1, 0}, {2, 0}} };
  i_block_180.tip = 'I';
  static Piesa i_block_270 {{7,1} , {{0, -1}, {0, 0}, {0, 1}, {0, 2}} };
  i_block_270.tip = 'I';

  static Piesa piese[] = {i_block_90 , i_block_180, i_block_270, i_block_0};

  return piese[x];
}

Piesa J_block(int x){

  static Piesa j_block_0 {{7,1}, {{-1, 0}, {-1, 1}, {0, 1}, {1, 1}}, 'J'};
  static Piesa j_block_90 {{7,1} ,{{0, 1}, {1, 1}, {1, 0}, {1, -1}}, 'J'};
  static Piesa j_block_180 {{7,1} , {{1, 0}, {1, -1}, {0, -1}, {-1, -1}}, 'J'};
  static Piesa j_block_270 {{7,1} , {{0, -1}, {-1, -1}, {-1, 0}, {-1, 1}}, 'J'};

  static Piesa piese[] = {j_block_90 , j_block_180, j_block_270, j_block_0};

  return piese[x];
}

Piesa L_block(int x){

  static Piesa l_block_0 {{7,1},{{0, 1}, {-1, 1}, {1, 0}, {1, 1}}, 'L' };
  static Piesa l_block_90 {{7,1}, {{1, 0}, {1, -1}, {0, 1}, {1, 1}}, 'L' };
  static Piesa l_block_180 {{7,1}, {{0, -1}, {1, -1}, {-1, 0}, {-1, -1}}, 'L' };
  static Piesa l_block_270 {{7,1} ,{{-1, 0}, {-1, 1}, {0, -1}, {-1, -1}}, 'L' };

  static Piesa piese[] = {l_block_90 , l_block_180, l_block_270, l_block_0};

  return piese[x];
}

Piesa O_block(int x){

  static Piesa o_block_0 {{7,1}, {{0, 0}, {1, 0}, {0, 1}, {1, 1}},'O'};
  static Piesa o_block_90 {{7,1} , {{0, 0}, {1, 0}, {0, 1}, {1, 1}},'O'};
  static Piesa o_block_180 {{7,1} , {{0, 0}, {1, 0}, {0, 1}, {1, 1}},'O'};
  static Piesa o_block_270 {{7,1} ,{{0, 0}, {1, 0}, {0, 1}, {1, 1}},'O'};

  static Piesa piese[] = {o_block_90 , o_block_180, o_block_270, o_block_0};

  return piese[x];
}

Piesa S_block(int x){

  static Piesa s_block_0 {{7,1},{{0, 0}, {1, 0}, {-1, 1}, {0, 1}}, 'S'};
  static Piesa s_block_90 {{7,1}, {{0, 0}, {0, -1}, {1, 0}, {1, 1}}, 'S'};
  static Piesa s_block_180 {{7,1}, {{0, 0}, {1, 0}, {-1, 1}, {0, 1}}, 'S'};
  static Piesa s_block_270 {{7,1}, {{0, 0}, {0, -1}, {1, 0}, {1, 1}}, 'S'};

  static Piesa piese[] = {s_block_90 , s_block_180, s_block_270, s_block_0};

  return piese[x];
}

Piesa T_block(int x){

  static Piesa t_block_0 {{7,1},{{0, 0}, {-1, 1}, {0, 1}, {1, 1}}, 'T'};
  static Piesa t_block_90 {{7,1}, {{0, 0}, {1, -1}, {1, 0}, {1, 1}}, 'T'};
  static Piesa t_block_180 {{7,1}, {{0, 0}, {1, -1}, {0, -1}, {-1, -1}}, 'T'};
  static Piesa t_block_270 {{7,1},{{0, 0}, {-1, 1}, {-1, 0}, {-1, -1}}, 'T'};

  static Piesa piese[] = {t_block_90 , t_block_180, t_block_270, t_block_0};

  return piese[x];
}

Piesa Z_block(int x){

  static Piesa z_block_0 {{7,1}, {{-1, 0}, {0, 0}, {0, 1}, {1, 1}},'Z'};
  static Piesa z_block_90 {{7,1}, {{0, -1}, {0, 0}, {1, 0}, {1, 1}},'Z'};
  static Piesa z_block_180 {{7,1}, {{-1, 0}, {0, 0}, {0, 1}, {1, 1}},'Z'};
  static Piesa z_block_270 {{7,1},{{0, -1}, {0, 0}, {1, 0}, {1, 1}},'Z'};

  static Piesa piese[] = {z_block_90 , z_block_180, z_block_270, z_block_0};

  return piese[x];
}


int valideazaMiscare(Piesa const& piesa, int miscare) {
  
  for (int i = 0; i < 4; ++i) {

    Punct pozitie;
    assignPunct(pozitie, addPuncte(piesa.centru, piesa.offset[i]));

    if (miscare == 1 && (pozitie.y == 32 || valori[pozitie.y][pozitie.x])) {
      return 0;
    }

    if (miscare != 1 && (pozitie.y == 32 || valori[pozitie.y][pozitie.x])) {
      return -1;
    }
    
    if (pozitie.x < 0 || pozitie.x > 15 || pozitie.y < 0 || pozitie.y > 31) {
      return -1;
    }
  }
  return 1;
}


int detecteazaButon() {
  Piesa copie;
  int mutata = false;
  int miscare = 0;
  if (digitalRead(W)) {

    copie = rotestePiesa(piesa);
    tone(BP, 10, 1000);
    mutata = true;
  } else {

    assignPiesa(copie,piesa);
    if (digitalRead(D)) {
      copie.centru.x --;
      mutata = true;
    } 
    if (digitalRead(A)) {
      copie.centru.x ++;
      mutata = true;
    }
    if (digitalRead(S)) {
      copie.centru.y ++;
      mutata = true;
      miscare = 1;
    }
  }

  int miscareValida = valideazaMiscare(copie, miscare);
  if (mutata && miscareValida > 0) {
    assignPiesa(piesa,copie);
  }

  return miscareValida;
}

void setup() {
  Serial.begin(115200);

  display.setBrightness(7);
  randomSeed(analogRead(0));

  pinMode(CLK, OUTPUT);
  pinMode(DIN, OUTPUT);
  pinMode(SS, OUTPUT);
  pinMode(BP, OUTPUT);
  

  Piesa new_piesa = piesaNoua();
  new_piesa.rotatie = 0;
  assignPiesa(piesa, new_piesa);
  

  Piesa new_piesa1 = piesaNoua();
  new_piesa1.rotatie = 0 ;
  assignPiesa(nextPiesa, new_piesa1);

  for (int j = 0; j < 13; ++j){ 
    valori[31][j] = 1;
  }

}

void verificaRanduri() {
  for (int i = 31; i >= 0; i --) {
    int j;
    for (j = 0; j < 16; j ++) {
      if (!valori[i][j]) {
        break;
      }
    }
    if (j >= 16) {
      tone(BP, 30, 1000);
      scor += 100;
      for (int k = i; k > 0; k --) {
        for (j = 0; j < 16; j++) {
          valori[k][j] = valori[k - 1][j];
        }
      }
      i ++;
    }
  }
}


void afiseazaScor() {
  static const char digits[] = {
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,
    SEG_B | SEG_C,
    SEG_A | SEG_B | SEG_D | SEG_E | SEG_G,
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_G,
    SEG_B | SEG_C | SEG_F,
    SEG_A | SEG_C | SEG_D | SEG_F,
    SEG_A | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G,
    SEG_A | SEG_B | SEG_C,
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G,
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G
  };
  char numbers[4] = {digits[0], digits[0], digits[0], digits[0]};
  int cscor = scor;
  int i = 3;
  while (cscor) {
    numbers[i--]  = digits[cscor % 10];
    cscor /= 10;
  }
  display.setSegments(numbers, 4, 0);
}

long long int ultimaCadere;
long long int ultimaMutare;


void loop(){
  if (millis() - ultimaMutare >= 100) {
    int miscareValida = detecteazaButon();
    if (miscareValida == 0) {
      for (int i = 0; i < 4; i++) {
        Punct pozitie;
        assignPunct(pozitie, addPuncte(piesa.centru, piesa.offset[i]));
        valori[pozitie.y][pozitie.x] = 1;
      }
      assignPiesa(piesa, nextPiesa);
      Piesa new_piesa = piesaNoua();
      assignPiesa(nextPiesa, new_piesa);
    }
    ultimaMutare = millis();
  }
  if (millis() - ultimaCadere >= 500) {
    Piesa copie;
    assignPiesa(copie, piesa);
    copie.centru.y ++;
    if (valideazaMiscare(copie, 1) == 0) {
      for (int i = 0; i < 4; i++) {
        Punct pozitie;
        assignPunct(pozitie, addPuncte(piesa.centru, piesa.offset[i]));
        valori[pozitie.y][pozitie.x] = 1;
      }
      assignPiesa(piesa,nextPiesa);
      Piesa new_piesa = piesaNoua();
      assignPiesa(nextPiesa, new_piesa);
    } else {
   
      assignPiesa(piesa,copie);
    }
    ultimaCadere = millis();
  }
  verificaRanduri();
  afiseazaScor();
  arataTabla();
  show();
}