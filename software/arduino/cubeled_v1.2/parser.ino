#define SOP         0xE7          // start of packet
#define EOP         0x7E          // end of packet

#define SET_ALL     0x00          // set all values
#define SET_PIXEL   0x01          // set pixel value
#define SET_LINE    0x02          // set line values
#define SET_PLAN    0x03          // set plan values   
#define SET_CUBE    0x04          // set cube values

#define REV_ALL     0x10          // reverse all values
#define REV_PIXEL   0x11          // set pixel value
#define REV_LINE    0x12          // set line values
#define REV_PLAN    0x13          // set plan values
#define REV_CUBE    0x14          // set cube values

#define MOVE_XYZ    0x21          // bouger l'ensemble des pixels dans l'axe choisi (trouver un moyen mémotechnique pour que ce soit facile à retenir, comme pour les lignes
#define MODU_XYZ    0x22          // mouvement torique
#define ROTA_XYZ    0x23          // rotation du plan
#define MIRO_XYZ    0x24          // mirroir sur un axe

#define VERSION     0xE0          // get version number
#define DEFAULT     0xE1          // load default parameters
#define LOAD        0xE2          // load last saved parameters
#define SAVE        0xE3          // save current parameters

#define REFRESH     0xFF          // refresh frame

uint8_t header, command, length;  // tête de lecture
uint8_t buffer[64];               // buffer de lecture

void parse(byte input) {
  switch (header) {
    case 0 :  // three basic commands
      if (input == SOP) header++;
      break;

    case 1 :  // byte 1 = command
      command = input;
      header++;
      break;

    case 2 :  // byte 2 = data MSB
      length = input;
      header++;
      break;

    default : // datas reading
      if (header < length + 3) {
        buffer[header - 3] = input; // push values in the buffer
        header++;
      }

      else {
        if (input == EOP) execute();
        else Serial.print("Erreur de commande");
        header = 0;
        length = 0;
      }
      break;
  }
}

void execute() {

  if (command < VERSION) changed = true;            // indique la modification de l'affichage

  switch (command) {

    //-------------------------------------------------------------------------------

    case SET_ALL:
      if (length == 1) {
        byte v = buffer[0] &  mask;
        for (int x = 0; x < SIZE; x++) {
          for (int y = 0; y < SIZE; y++) {
            bufferFrame[x][y] = v;
          }
        }
      }
      break;

    case SET_PIXEL:
      if (length == 4) {
        //change seulement un pixel
        byte x, y, z, v;
        x = buffer[0] < SIZE ? buffer[0] : SIZE - 1;
        y = buffer[1] < SIZE ? buffer[1] : SIZE - 1;
        z = buffer[2] < SIZE ? buffer[2] : SIZE - 1;
        v = (buffer[3] > 0);
        bitWrite(bufferFrame[x][y], z, v);
      }
      break;

    case SET_LINE:
      if (length == 4) {
        //plan, coord1, coord2, valeur
        byte p, c1, c2, v;
        p  = buffer[0] % 3;
        c1 = buffer[1] % SIZE;
        c2 = buffer[2] % SIZE;
        v  = buffer[3] & mask;
        switch (p) {
          // axe X, c1 = z, c2 = y
          case 0 :
            for (int x = 0; x < SIZE; x++) {
              bitWrite(bufferFrame[x][c1], c2, bitRead(v, x));
            }
            break;
          // axe Z, c1 = x, c2 = y
          case 1 :
            for (int y = 0; y < SIZE; y++) {
              bitWrite(bufferFrame[c1][y], c2, bitRead(v, y));
            }
            break;
          // axe Y, c1 = x, c2 = z
          case 2 :
            bufferFrame[c1][c2] = v;
            break;
        }
      }
      break;

    case SET_PLAN:
      if (length == 3)  {
        //plan, coordonée, ligne à répéter

        byte p, c, v;
        p  = buffer[0] % 3;
        c  = buffer[1] % SIZE;
        v  = buffer[2] & mask;

        switch (p) {
          case 0 :
            for (int x = 0; x < SIZE; x++) {
              bufferFrame[x][c] = v;
            }
            break;
          case 1 :
            for (int x = 0; x < SIZE; x++) {
              for (int y = 0; y < SIZE; y++) {
                bufferFrame[x][y] = bufferFrame[x][y] ^ (1 << c) * bitRead(v, x);
              }
            }
            break;
          case 2 :
            for (int y = 0; y < SIZE; y++) {
              bufferFrame[c][y] = v;
            }
            break;
        }
      }
      if (length == 9) {
        //plan, coord, 7 lignes valeur TODO
        byte p, c;
        p  = buffer[0] % 3;
        c  = buffer[1] % SIZE;

        switch (p) {
          case 0 :
            for (int x = 0; x < SIZE; x++) {
              bufferFrame[x][c] = buffer[x + 2] & mask;
            }
            break;
          case 1 :
            for (int x = 0; x < SIZE; x++) {
              for (int y = 0; y < SIZE; y++) {
                byte v = buffer[y + 2] & mask;
                bufferFrame[x][y] = bufferFrame[x][y] ^ (1 << c) * bitRead(v, x);
              }
            }
            break;
          case 2 :
            for (int y = 0; y < SIZE; y++) {
              bufferFrame[c][y] = buffer[y + 2] & mask;
            }
            break;
        }
      }
      break;

    case SET_CUBE:
      if (length == 4) {
        //plan, >>répète un ligne sur tout le cube
      }
      if (length == 49) {
        //chaque octet représente une colonne
        for (byte i = 0; i < 49; i++) {
          byte x = i % SIZE;
          byte y = i / SIZE;
          bufferFrame[x][y] = buffer[i] & mask;
        }
      }
      break;

    //-------------------------------------------------------------------------------

    case REV_ALL:
      if (length == 0) {
        for (int x = 0; x < SIZE; x++) {
          for (int y = 0; y < SIZE; y++) {
            bufferFrame[x][y] = (byte)~bufferFrame[x][y];
          }
        }
      }
      break;

    case REV_PIXEL:
      if (length == 4) {
        //change seulement un pixel
        byte x, y, z, v;
        x = buffer[0] < SIZE ? buffer[0] : SIZE - 1;
        y = buffer[1] < SIZE ? buffer[1] : SIZE - 1;
        z = buffer[2] < SIZE ? buffer[2] : SIZE - 1;
        v = bitRead(bufferFrame[x][y], y) ^ (buffer[3] > 0);
        bitWrite(bufferFrame[x][y], z, v);
      }
      break;

    case REV_LINE:
      if (length == 4) {
        //plan, coord1, coord2, valeur
        byte p, c1, c2, v, t;
        p  = buffer[0] < 3    ? buffer[0] : 2;
        c1 = buffer[1] < SIZE ? buffer[1] : SIZE - 1;
        c2 = buffer[2] < SIZE ? buffer[2] : SIZE - 1;
        v  = buffer[3] & mask;
        switch (p) {
          case 0 :
            bufferFrame[c1][c2] = bufferFrame[c1][c2] ^ v;
            break;
          case 1 :
            for (int x = 0; x < SIZE; x++) {
              t = bitRead(bufferFrame[x][c1], c2) ^ bitRead(v, x);
              bitWrite(bufferFrame[x][c1], c2, t);
            }
            break;
          case 2 :
            for (int y = 0; y < SIZE; y++) {
              t = bitRead(bufferFrame[c1][y], c2) ^ bitRead(v, y);
              bitWrite(bufferFrame[c1][y], c2, t);
            }
            break;
        }
      }
      break;

    case REV_PLAN:
      break;

    case REV_CUBE:
      if (length == 49) {
        //chaque octet représente une colonne
        for (byte i = 0; i < 49; i++) {
          byte x = i % SIZE;
          byte y = i / SIZE;
          bufferFrame[x][y] = bufferFrame[x][y] ^ (buffer[i] & mask);
        }
      }
      break;

    //-------------------------------------------------------------------------------
    case MOVE_XYZ:
      if (length == 1) {
        //plan [0-5], default offset 1
        MOVE(buffer[0], 1, false);
      }
      if (length == 2) {
        //plan [0-5], offset
        MOVE(buffer[0], buffer[1], false);
      }
      break;

    case MODU_XYZ:
      if (length == 1) {
        //plan [0-5], default offset 1
        MOVE(buffer[0], 1, true);
      }
      if (length == 2) {
        //plan [0-5], offset
        MOVE(buffer[0], buffer[1], true);
      }
      break;

    case ROTA_XYZ:
      //TODO
      break;

    case MIRO_XYZ:
      //TODO
      break;

    //-------------------------------------------------------------------------------

    case VERSION:
      Serial.println("v1.2");
      break;

    case DEFAULT:
      defaultParams();
      Serial.println("Default parameters");
      break;

    case LOAD:
      loadParams();
      Serial.println("Parameters loaded");
      break;

    case SAVE:
      saveParams();
      Serial.println("Parameters saved");
      break;

    //-------------------------------------------------------------------------------

    case FRAMERATE:
      if (length == 1) interFrame = buffer[0];
      break;

    case LAYERRATE:
      if (length == 2) interLayer = buffer[0] << 8 | buffer[1];
      break;

    case EXTCLOCK:
      if (length == 1) refresh = buffer[0] != 0;
      break;

    case REFRESH:
      refreshFrame();
      break;

  }
}

void MOVE(byte plan, byte offset, bool loop) {

  byte newFrame[SIZE][SIZE] = {{0}, {0}};

  switch (plan) {
    case 0: // décale d'une case vers le fond
      for (int x = 0; x < SIZE; x++) {
        for (int y = loop ? 0 : offset; y < SIZE; y++) {
          newFrame[x][y] = bufferFrame[x][y - offset < 0 ? SIZE + y - offset : y - offset];
        }
      }
      break;
    case 1: // décale d'une case vers le haut
      for (int x = 0; x < SIZE; x++) {
        for (int y = 0; y < SIZE; y++) {
          byte val = bufferFrame[x][y] << offset;
          if (loop) val |= bufferFrame[x][y] >> (SIZE - offset);
          newFrame[x][y] = val & mask;
        }
      }
      break;
    case 2: // décale d'une case vers le droite
      for (int x = loop ? 0 : offset; x < SIZE; x++) {
        for (int y = 0; y < SIZE; y++) {
          newFrame[x][y] = bufferFrame[x - offset < 0 ? SIZE + x - offset : x - offset][y];
        }
      }
      break;
  }

  copyFrame(newFrame, bufferFrame);
}
