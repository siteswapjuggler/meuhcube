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

#define REFRESH     0xFF          // refresh frame

//NOTE pour les plans il faudrait aussi les inverses


uint8_t header, command, length;  // tête de lecture
uint8_t buffer[64];               // buffer de lecture

void parseInput(byte input) {
  switch (header) {
        case 0 :  // three basic commands
                  if (input==SOP) header++;
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
                  if (header<length+3) {     
                    buffer[header-3]=input;  // push values in the buffer
                    header++;
                    }
          
                  else {
                    if (input==EOP) {        // when endbyte is received
                      switch (command) {

                        //-------------------------------------------------------------------------------
                        
                        case SET_ALL:
                          if (length==1) {
                            byte v = B1111111 * (buffer[0] > 0);
                            for (int line = 0; line < SIZE; line++) {
                              for (int column = 0; column < SIZE; column++) {
                                bufferFrame[line][column] = v;
                              }
                            }
                          }
                          break;

                        case SET_PIXEL:
                          if (length==4) {
                            //change seulement un pixel
                            byte x,y,z,v;
                            x = buffer[0] < SIZE ? buffer[0] : SIZE-1;
                            y = buffer[1] < SIZE ? buffer[1] : SIZE-1;
                            z = buffer[2] < SIZE ? buffer[2] : SIZE-1;
                            v = (buffer[3] > 0);
                            bitWrite(bufferFrame[x][z],y,v);
                          }
                          break;

                        case SET_LINE: 
                          if (length==4) {
                            //plan, coord1, coord2, valeur
                            byte p, c1, c2, v;
                            p  = buffer[0] < 3    ? buffer[0] : 2;
                            c1 = buffer[1] < SIZE ? buffer[1] : SIZE-1;
                            c2 = buffer[2] < SIZE ? buffer[2] : SIZE-1;
                            v  = buffer[3]&B1111111;
                            switch (p) {
                              case 0 :
                                cubeVoxels[c1][c2] = v;
                                break;
                              case 1 :
                                for (int line = 0; line < SIZE; line++) {
                                  bitWrite(bufferFrame[line][c1],c2,bitRead(v,line));                                
                                }
                                break;
                              case 2 :
                                for (int column = 0; column < SIZE; column++) {
                                  bitWrite(bufferFrame[c1][column],c2,bitRead(v,column));                                
                                }
                                break;
                            }
                          }
                          break;

                        case SET_PLAN: 
                          if (length==4)  {
                            //plan, coord, ligne à répéter
                          }
                          if (length==10) {
                            //plan, coord, 7 lignes valeur
                          }
                          break;
                          
                        case SET_CUBE: 
                          if (length==49) {
                            //chaque octet représente une colonne
                            for (byte i = 0; i < 49; i++) {
                              byte line = i % SIZE;
                              byte column = i / SIZE;
                              bufferFrame[line][column] = buffer[i]&B1111111; 
                            }
                          }
                          break;

                        //-------------------------------------------------------------------------------
                        
                        case REV_ALL:
                          if (length==0) {
                            for (int line = 0; line < SIZE; line++) {
                              for (int column = 0; column < SIZE; column++) {
                                bufferFrame[line][column] = (byte)~bufferFrame[line][column];
                              }
                            }
                          }
                          break;
                          
                        case REV_PIXEL:
                          if (length==4) {
                            //change seulement un pixel
                            byte x,y,z,v;
                            x = buffer[0] < SIZE ? buffer[0] : SIZE-1;
                            y = buffer[1] < SIZE ? buffer[1] : SIZE-1;
                            z = buffer[2] < SIZE ? buffer[2] : SIZE-1;
                            v = bitRead(bufferFrame[x][y],y)^(buffer[3] > 0);
                            bitWrite(bufferFrame[x][z],y,v);
                          }
                          break;

                        case REV_LINE: 
                          if (length==4) {
                            //plan, coord1, coord2, valeur
                            byte p, c1, c2, v, t;
                            p  = buffer[0] < 3    ? buffer[0] : 2;
                            c1 = buffer[1] < SIZE ? buffer[1] : SIZE-1;
                            c2 = buffer[2] < SIZE ? buffer[2] : SIZE-1;
                            v  = buffer[3]&B1111111;
                            switch (p) {
                              case 0 :
                                bufferFrame[c1][c2] = bufferFrame[c1][c2]^v;
                                break;
                              case 1 :
                                for (int line = 0; line < SIZE; line++) {
                                  t = bitRead(bufferFrame[line][c1],c2) ^ bitRead(v,line);
                                  bitWrite(bufferFrame[line][c1],c2,t);                                
                                }
                                break;
                              case 2 :
                                for (int column = 0; column < SIZE; column++) {
                                  t = bitRead(bufferFrame[c1][column],c2) ^ bitRead(v,column);
                                  bitWrite(bufferFrame[c1][column],c2,t);                                
                                }
                                break;
                            }
                          }
                          break;

                        case REV_PLAN: 
                          if (length==4)  {
                            //plan, coord, ligne à répéter
                            }
                          if (length==10) {
                            //plan, coord, 7 lignes valeur
                            }
                          break;
                          
                        case REV_CUBE: 
                          if (length==49) {
                            //chaque octet représente une colonne
                            for (byte i = 0; i < 49; i++) {
                              byte line = i % SIZE;
                              byte column = i / SIZE;
                              bufferFrame[line][column] = bufferFrame[line][column]^(buffer[i]&B1111111); 
                            }
                          }
                          break;

                        //-------------------------------------------------------------------------------
                        
                        case REFRESH: 
                          refreshFrame();
                          break;

                        }                  
                    header=0;
                    length=0;
                    }
                  }
                  break;
  }
}    

