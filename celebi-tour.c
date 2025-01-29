#include <gb/gb.h>
#include <gb/hardware.h>
#include <gbdk/console.h>
#include <gbdk/font.h>
#include <rand.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "gen2.h"

#define PARTY_SIZE 6
#define POKE_SIZE 48
#define NAME_LEN 11
#define SELP_LEN 8
#define POKE_LEN POKE_SIZE * PARTY_SIZE
#define ORIG_LEN NAME_LEN * PARTY_SIZE
#define NICK_LEN NAME_LEN * PARTY_SIZE
#define FULL_LEN NAME_LEN + SELP_LEN + (POKE_SIZE * PARTY_SIZE) + (NAME_LEN * PARTY_SIZE) + (NAME_LEN * PARTY_SIZE)

#define SER_REG_DIR (*(uint8_t *)0xFF01)
#define SER_OPT_REG_DIR (*(uint8_t *)0xFF02)

#define Y  0xfc
#define B  0x00
#define W  0xff
#define R  0xe0
#define D  0xf1
#define Br 0xac

#define PREAMBLE 0xFD
#define BLANK 0x00
#define NO_DATA_BYTE 0xFE
#define TERMINATOR 0xFF

uint8_t DATA_BLOCK_GEN_II[PLAYER_LENGTH_GEN_II];
uint8_t MAIL_BLOCK[MAIL_DATA_SIZE_GEN2];

uint8_t DATA_BLOCK_GEN_II_array[PLAYER_LENGTH_GEN_II] ={
pokechar_E, pokechar_U, pokechar_R, pokechar_O, pokechar_P, pokechar_E, 0x50,0x00, 0x00, 0x00, 0x00, // name
0x04, 0xFB, 0xF3, 0xF4, 0xF5, 0xFF, 0xFF, 0xFF, // number of pokemon in party (0x00), pokemon ID (0x01 - 0x06), stop byte (0x07)
0xA9, 0xC8, //trainer ID
// 6 pokemon
//spc Held  mv1   mv2   mv3   mv4   OT ID  exp.  HP_EV  Attack_EV   Def_EV      Speed_EV    Special_EV  IV_Data     m1pp  m2pp  m3pp  m4pp  frnd  virus cght_data   lvl   sat.  N/A   curr_HP     max_HP      attack      defence     speed       spcl_att    spcl_def
0xFB, 0x73 ,0x21, 0x00, 0x00, 0x00, 0xA9, 0xC8, 0x00 ,0x00, 0x00, 0x07 ,0xB4, 0x08, 0x5F, 0x07, 0x8F, 0x09, 0xDF, 0x09, 0x6A, 0x88, 0x51, 0x14 ,0x1E, 0x00, 0x05, 0x9F, 0x00, 0xC3, 0x87, 0x05, 0x00, 0x00, 0x00, 0x2D, 0x00, 0x2D, 0x00, 0x18, 0x00, 0x16, 0x00, 0x29, 0x00, 0x2F, 0x00, 0x15,
0xF3, 0x1A, 0x21, 0x00, 0x00, 0x00, 0xA9, 0xC8, 0x00 ,0x00, 0x00, 0x07 ,0xB4, 0x08, 0x5F, 0x07, 0x8F, 0x09, 0xDF, 0x09, 0x6A, 0x88, 0x51, 0x14 ,0x1E, 0x00, 0x05, 0x9F, 0x00, 0xC3, 0x87, 0x05, 0x00, 0x00, 0x00, 0x2D, 0x00, 0x2D, 0x00, 0x18, 0x00, 0x16, 0x00, 0x29, 0x00, 0x2F, 0x00, 0x15,
0xF4, 0x1A, 0x21, 0x00, 0x00, 0x00, 0xA9, 0xC8, 0x00 ,0x00, 0x00, 0x07 ,0xB4, 0x08, 0x5F, 0x07, 0x8F, 0x09, 0xDF, 0x09, 0x6A, 0x88, 0x51, 0x14 ,0x1E, 0x00, 0x05, 0x9F, 0x00, 0xC3, 0x87, 0x05, 0x00, 0x00, 0x00, 0x2D, 0x00, 0x2D, 0x00, 0x18, 0x00, 0x16, 0x00, 0x29, 0x00, 0x2F, 0x00, 0x15, 
0xF5, 0x1A, 0x21, 0x00, 0x00, 0x00, 0xA9, 0xC8, 0x00 ,0x00, 0x00, 0x07 ,0xB4, 0x08, 0x5F, 0x07, 0x8F, 0x09, 0xDF, 0x09, 0x6A, 0x88, 0x51, 0x14 ,0x1E, 0x00, 0x05, 0x9F, 0x00, 0xC3, 0x87, 0x05, 0x00, 0x00, 0x00, 0x2D, 0x00, 0x2D, 0x00, 0x18, 0x00, 0x16, 0x00, 0x29, 0x00, 0x2F, 0x00, 0x15,
0xFA, 0x1A, 0x21, 0x00, 0x00, 0x00, 0xA9, 0xC8, 0x00 ,0x00, 0x00, 0x07 ,0xB4, 0x08, 0x5F, 0x07, 0x8F, 0x09, 0xDF, 0x09, 0x6A, 0x88, 0x51, 0x14 ,0x1E, 0x00, 0x05, 0x9F, 0x00, 0xC3, 0x87, 0x05, 0x00, 0x00, 0x00, 0x2D, 0x00, 0x2D, 0x00, 0x18, 0x00, 0x16, 0x00, 0x29, 0x00, 0x2F, 0x00, 0x15,
0xF9, 0x1A, 0x21, 0x00, 0x00, 0x00, 0xA9, 0xC8, 0x00 ,0x00, 0x00, 0x07 ,0xB4, 0x08, 0x5F, 0x07, 0x8F, 0x09, 0xDF, 0x09, 0x6A, 0x88, 0x51, 0x14 ,0x1E, 0x00, 0x05, 0x9F, 0x00, 0xC3, 0x87, 0x05, 0x00, 0x00, 0x00, 0x2D, 0x00, 0x2D, 0x00, 0x18, 0x00, 0x16, 0x00, 0x29, 0x00, 0x2F, 0x00, 0x15,
// 6 trainer names
pokechar_E, pokechar_U, pokechar_R, pokechar_O, pokechar_P, pokechar_E, 0x50,0x00, 0x00, 0x00, 0x00,
pokechar_E, pokechar_U, pokechar_R, pokechar_O, pokechar_P, pokechar_E, 0x50,0x00, 0x00, 0x00, 0x00,
pokechar_E, pokechar_U, pokechar_R, pokechar_O, pokechar_P, pokechar_E, 0x50,0x00, 0x00, 0x00, 0x00,
pokechar_E, pokechar_U, pokechar_R, pokechar_O, pokechar_P, pokechar_E, 0x50,0x00, 0x00, 0x00, 0x00,
pokechar_E, pokechar_U, pokechar_R, pokechar_O, pokechar_P, pokechar_E, 0x50,0x00, 0x00, 0x00, 0x00,
pokechar_E, pokechar_U, pokechar_R, pokechar_O, pokechar_P, pokechar_E, 0x50,0x00, 0x00, 0x00, 0x00,
// 6 nicknames
pokechar_C, pokechar_E, pokechar_L, pokechar_E, pokechar_B, pokechar_I, 0x50, 0x00, 0x00, 0x00, 0x00,
pokechar_R, pokechar_A, pokechar_I, pokechar_K, pokechar_O, pokechar_U, 0x50, 0x00, 0x00, 0x00, 0x00,
pokechar_E, pokechar_N, pokechar_T, pokechar_E, pokechar_I, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00,
pokechar_S, pokechar_U, pokechar_I, pokechar_C, pokechar_U, pokechar_N, pokechar_E, 0x50, 0x00, 0x00, 0x00,
0x8C, 0x80, 0x86, 0x88, 0x8A, 0x80, 0x91, 0x8F, 0x50, 0x50, 0x50,
0x8C, 0x80, 0x86, 0x88, 0x8A, 0x80, 0x91, 0x8F, 0x50, 0x50, 0x50,
// 2 useless bytes??
0x00, 0x00, 0x00};

// Celebi image.

const unsigned char celebi_tiles[] =  {
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   7,   7,  24,  28,  23,  31,
      0,   0,   7,   7,   4,   7,   3,   3,   0,   0, 128, 128,  96,  96, 144, 144,
      0,   0, 128, 128,  96, 224, 144, 208, 104, 104,  40,  40,  20,  20,  28,  20,
      0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   7,   6,  31,  24, 254, 225,
      0,   0,  32,  32,  96,  96, 224, 160, 224,  32, 224,  32, 192,  64, 192,  64,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
     40,  56,  40,  56,  48,  48,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    120, 105,  62,  54,  28,  16,  48,  32,  32,  32,  64,   0,  64,  64, 224, 192,
    255, 247,  14,  10,  14,  10,   5,  10,   6,   0,   0,   0,   0,   0,  14,  15,
     91,   2,  39,   4,  79,  24,  15,  97,  23,   3,  39,  13,   6,  50, 138,   2,
    192,  64, 128, 128, 128, 128, 128, 128,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    240, 224, 144, 208, 176, 240,  80, 112, 102,  68,  48,  32,  60,  16,  15,  12,
     31,  63,  51,  59,  41, 109,  97, 125, 115, 115,  63,  62, 127,   1, 254,  14,
     84, 132, 252, 132, 248, 168, 240, 176, 224,  96, 128, 128, 128, 128,   0, 120,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0, 127, 127,  92,  64, 127, 115,  60,  44,  48,  48,
      3,   2,  12,  12, 113, 112, 134, 131,  14,   5, 254,  13, 255, 248, 111, 104,
      6,   3,   1,   1,   4,  24,  12,  56,  22, 108,  63, 198, 255,   3, 254,   2,
      0, 136, 128, 144,  96,  96,  28,  28,   3,   3,   0,   0, 225,   0, 255, 195,
      0,   0,   0,   0,   0,   0,   0,   0, 248, 248, 248,   8, 240,  48, 240, 144,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      1,   1,   1,   1,   1,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    223, 156, 254,  17, 191,  71, 221, 173, 113,  81,  48,  48,   0,   0,   0,   0,
    126, 226,  60,  20, 252,   4, 184,  72, 184,  88, 240, 144,  96,  96,   0,   0,
     60,  60,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    112, 112,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
};


// It has +128 since we are loading Mew sprites starting at position 128
// to prevent collisions with the font sprites.
const unsigned char celebi_map[] = {
0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x80,0x80,
0x8f,0x90,0x91,0x92,0x93,0x80,0x80,0x80,        
0x96,0x97,0x98,0x99,0x9a,0x80,0x80,0x80,   
0x9d,0x9e,0x9f,0xa0,0xa1,0x82,0x80,0x80,
0xa4,0xa5,0xa6,0xa7,0xff,0x82,0x80,0x80,
0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
};

enum connection_state_t connection_state = NOT_CONNECTED;
enum trade_state_t trade_state = INIT;
uint8_t DATA_BLOCK[PARTY_DATA_SIZE_GEN2];
uint8_t PATCH_BLOCK[PATCH_SIZE];
uint8_t scheduled_refill = TRUE;


    
// get a seed to be used for random generation by xoring values from ram which are pseudorandom on startup.
uint16_t get_ram_seed(void) {
    uint16_t *p = (uint16_t*)0xC000;
    uint16_t sum = 0;
    for (uint16_t i = 0; i < 0x1FFF; i++) {
        sum ^= p[i];
    }
    return sum;
}





uint8_t handle_byte(uint8_t in, size_t *counter, clock_t *last_action) {
    // Simple protocol explanation.
    // Steps:
    //    First synchronization
    //    0xFD until the other gameboy is ready and answers 0xFD
    //    Random seed data (Size: 10)
    //    0xFD until the other gameboy is ready and answers 0xFD
    //    Party data (Size: 418 for Gen 1, 444 for Gen 2)
    //    0xFD until the other gameboy is ready and answers 0xFD
    //    Patch set (Size: 197)
    //    End
    // Note: Gen 2 also has mail data at the end (Size: 381).
    static uint8_t out;

    // If the cable is disconnected or the console is not ready for more than 5 seconds, reset trading process.
    // This method will only work if the distribution cartridge is acting as master.
    clock_t current_time = clock();
    if (connection_state != NOT_CONNECTED && (in == LINK_CABLE_EMPTY || in == LINK_CABLE_DISCONNECT)) {
        if (current_time - (*last_action) > 300) {
            connection_state = NOT_CONNECTED;
            trade_state = INIT;
            SC_REG = SIOF_CLOCK_INT;
            out = PKMN_MASTER;
            return out;
        }
    } else {
        (*last_action) = current_time;
    }

    //printf("%d",connection_state);
    switch (connection_state) {
        case NOT_CONNECTED:
            //puts("NOT CONNECTED STATE\n");
            switch (in) {
                // Note: If master, always reply with PKMN_MASTER to 0x01 and 0x02. If slave, always reply with
                // PKMN_SLAVE.
                case PKMN_MASTER:
                    out = PKMN_MASTER;
                    break;
                case PKMN_SLAVE:
                    out = PKMN_MASTER;
                    break;
                case PKMN_BLANK:
                    out = PKMN_BLANK;
                    break;
                case PKMN_CONNECTED_TIME_CAPSULE:
                    in = PKMN_CONNECTED;
                    //puts("CAPSULA DEL TIEMPO\n");
                    connection_state = CONNECTED;
                    out = PKMN_CONNECTED;
                    break;

                case PKMN_CONNECTED:
                    //puts("CAPSULA NORMAL\n");
                    connection_state = CONNECTED;
                    out = PKMN_CONNECTED;
                    break;

                default:
                    // By default, answer constantly with PKMN_MASTER (or PKMN_SLAVE if acting as slave).
                    out = PKMN_MASTER;
                    break;
            }
            break;

        case CONNECTED:
            //puts("CONNECTED STATE\n");
            switch (in) {
                case PKMN_CONNECTED_TIME_CAPSULE:
                    connection_state = TRADE_CENTRE;
                    out = PKMN_TRADE_CENTRE;
                    break;
                case PKMN_CONNECTED:
                    out = PKMN_CONNECTED;
                    break;
                case PKMN_TRADE_CENTRE:
                    connection_state = TRADE_CENTRE;
                    out = PKMN_TRADE_CENTRE;
                    break;
                case PKMN_COLOSSEUM:
                    // Not handled, everything will just be mirrored.
                    connection_state = COLOSSEUM;
                    out = PKMN_COLOSSEUM;
                    break;
                case PKMN_BREAK_LINK:
                    connection_state = NOT_CONNECTED;
                    out = PKMN_BREAK_LINK;
                    break;

                default:
                    out = in;
                    break;
            }
            break;

        case TRADE_CENTRE:
            //puts("TRADE CENTRE STATE \n");
            //printf("%d",trade_state);
            if (trade_state == INIT && in == 0x00) {
                (*counter) = 0;
                trade_state = READY;
                out = 0x00;
            } else if (trade_state == READY && in == TRADE_CENTRE_READY) {
                trade_state = DETECTED;
                out = TRADE_CENTRE_READY;
            } else if (trade_state == DETECTED && in != TRADE_CENTRE_READY) {
                // Here random data seed is sent... Just ignore, we don't need it.
                out = in;
                trade_state = DATA_TX_RANDOM;
            } else if (trade_state == DATA_TX_RANDOM && in == TRADE_CENTRE_READY) {
                trade_state = DATA_TX_WAIT;
                out = TRADE_CENTRE_READY;
            } else if (trade_state == DATA_TX_WAIT && in == TRADE_CENTRE_READY) {
                out = 0x00;
            } else if (trade_state == DATA_TX_WAIT && in != TRADE_CENTRE_READY) {
                (*counter) = 0;
                // send first byte
                out = DATA_BLOCK_GEN_II_array[(*counter)];
                //INPUT_BLOCK[(*counter)] = in;
                trade_state = DATA_TX;
                (*counter)++;
            } else if (trade_state == DATA_TX) {
		    if ((*counter) < PARTY_DATA_SIZE_GEN2) {
			out = DATA_BLOCK_GEN_II_array[(*counter)];
			(*counter)++;
		    }
		    else if ((*counter) < PARTY_DATA_SIZE_GEN2 + MAIL_DATA_SIZE_GEN2) {
			out = MAIL_BLOCK[(*counter) - PARTY_DATA_SIZE_GEN2];
			(*counter)++;
		    } 
		    else {
			trade_state = DATA_TX_PATCH;  // Pasa a la siguiente etapa
			(*counter) = 0;  // Reinicia el contador
		    }
		

            } else if (trade_state == DATA_TX_PATCH && in == TRADE_CENTRE_READY) {
                (*counter) = 0;
                out = TRADE_CENTRE_READY;
            } else if (trade_state == DATA_TX_PATCH && in != TRADE_CENTRE_READY) {
		    // Envía los datos de parche correctamente.
		    out = PATCH_BLOCK[(*counter)];
		    (*counter)++;

		    // Verifica que el contador no exceda PATCH_SIZE
		    if ((*counter) >= PATCH_SIZE) {
			trade_state = TRADE_WAIT;
			(*counter) = 0;  // Reinicia el contador para TRADE_WAIT
		    }
		}
		else if (trade_state == TRADE_WAIT && (in & 0x60) == 0x60) {
		    if (in == 0x6f) {
			trade_state = READY;
			out = 0x6f;  // Confirmación del slot seleccionado
		    } else if (in == 0x61) {
			trade_state = TRADE_WAIT;  // Sigue esperando
			out = in;
		    } else {
			out = 0x60;  // Respuesta por defecto para el slot inicial
		    }
		} else if (trade_state == TRADE_WAIT && in == 0x00) {
                out = 0;
                trade_state = TRADE_DONE;
            } else if (trade_state == TRADE_DONE && (in & 0x60) == 0x60) {
                out = in;
                if (in == 0x61) {
                    trade_state = TRADE_WAIT;
                } else {
                    trade_state = DONE;
                }
            } else if (trade_state == DONE && in == 0x00) {
                // Trade finished, no more data will be sent at this moment, it's safe to refill the Pokémon group
                // in order to regenerate TIDs. If the TID is fixed, this line can be commented out.
                scheduled_refill = TRUE;
                out = 0;
                trade_state = INIT;
            } else {
            	
                out = in;
            }
            break;

        default:
            out = in;
            break;
    }

    return out;
}

uint8_t sio_exchange_master(uint8_t b) {
    SB_REG = b;
    SC_REG = SIOF_XFER_START | SIOF_CLOCK_INT;
    while (SC_REG & SIOF_XFER_START);
    return SB_REG;
}

uint8_t sio_exchange_slave(uint8_t b) {
    SB_REG = b;
    SC_REG = SIOF_XFER_START | SIOF_CLOCK_EXT;
    while (SC_REG & SIOF_XFER_START);
    return SB_REG;
}

void main(void) {
    // Read from RAM to generate the seed (from 0xC000 to 0xDFFF) for later pseudo-random TID generation.
    initrand(get_ram_seed());

    font_init();
    font_set(font_load(font_spect));

    puts("\n    Celebi Tour \n\n\n\n\n\n\n\n\n\n\n\n\n");
    
    puts("    2024 Noozh");

    //Load Celebi tiles starting at position 128.
    set_bkg_data(128, 64, celebi_tiles);
    // Draw Celebi figure in the middle of the screen.
    set_bkg_tiles(7, 7, 8, 8, celebi_map);

    disable_interrupts();

    static uint8_t in = 0xff;
    static size_t trade_counter = 0;
    static clock_t last_action = 0;
    
    //PlayerData player = create_player_data();
    
    PlayerData player = {
        .name = {pokechar_E, pokechar_U, pokechar_R, pokechar_O, pokechar_P, pokechar_E, 0x50, 0x00, 0x00, 0x00, 0x00},
        .partyCount = 0x01,
        .partyIDs = {0xFB, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        .stopByte = 0x00,
        .trainerID = 0xA9C8,
        .party = {
            { // Miembro 1
                .species = 0xFB,
                .heldItem = 0x73,
                .moves = {0x47, 0x00, 0x00, 0x00},
                .otID = 0xA9C8,
                .exp_1 = 0x00,
                .exp_2 = 0x00,
                .exp_3 = 0x00,
                .evHP = 0x0000,
                .evAttack = 0x0000,
                .evDefense = 0x0000,
                .evSpeed = 0x0000,
                .evSpecial = 0x0000,
                .ivData = 0x0000,
                .movePP = {0x10, 0x00, 0x00, 0x00},
                .friendship = 0x9F,
                .pokerus = 0x00,
                .captureData = 0xC3,
                .level = 0x05,
                .status = 0x05,
                .currentHP = 0x002D,
                .maxHP = 0x002D,
                .attack = 0x0001,
                .defense = 0x0001,
                .speed = 0x0001,
                .specialAttack = 0x0001,
                .specialDefense = 0x0001
            },{ // Miembro 2
                .species = 0xFB,
                .heldItem = 0x73,
                .moves = {0x47, 0x00, 0x00, 0x00},
                .otID = 0xA9C8,
                .exp_1 = 0x00,
                .exp_2 = 0x00,
                .exp_3 = 0x00,
                .evHP = 0x0000,
                .evAttack = 0x0000,
                .evDefense = 0x0000,
                .evSpeed = 0x0000,
                .evSpecial = 0x0000,
                .ivData = 0x0000,
                .movePP = {0x10, 0x00, 0x00, 0x00},
                .friendship = 0x9F,
                .pokerus = 0x00,
                .captureData = 0xC3,
                .level = 0x05,
                .status = 0x05,
                .currentHP = 0x002D,
                .maxHP = 0x002D,
                .attack = 0x0001,
                .defense = 0x0001,
                .speed = 0x0001,
                .specialAttack = 0x0001,
                .specialDefense = 0x0001
            },{ // Miembro 3
                 .species = 0xFB,
                .heldItem = 0x73,
                .moves = {0x47, 0x00, 0x00, 0x00},
                .otID = 0xA9C8,
                .exp_1 = 0x01,
                .exp_2 = 0x00,
                .exp_3 = 0x00,
                .evHP = 0x0000,
                .evAttack = 0x0000,
                .evDefense = 0x0000,
                .evSpeed = 0x0000,
                .evSpecial = 0x0000,
                .ivData = 0x0000,
                .movePP = {0x10, 0x00, 0x00, 0x00},
                .friendship = 0x9F,
                .pokerus = 0x00,
                .captureData = 0xC3,
                .level = 0x05,
                .status = 0x05,
                .currentHP = 0x002D,
                .maxHP = 0x002D,
                .attack = 0x0001,
                .defense = 0x0001,
                .speed = 0x0001,
                .specialAttack = 0x0001,
                .specialDefense = 0x0001
            },{ // Miembro 4
                .species = 0xFB,
                .heldItem = 0x73,
                .moves = {0x47, 0x00, 0x00, 0x00},
                .otID = 0xA9C8,
                .exp_1 = 0x01,
                .exp_2 = 0x00,
                .exp_3 = 0x00,
                .evHP = 0x0000,
                .evAttack = 0x0000,
                .evDefense = 0x0000,
                .evSpeed = 0x0000,
                .evSpecial = 0x0000,
                .ivData = 0x0000,
                .movePP = {0x10, 0x00, 0x00, 0x00},
                .friendship = 0x9F,
                .pokerus = 0x00,
                .captureData = 0xC3,
                .level = 0x05,
                .status = 0x05,
                .currentHP = 0x002D,
                .maxHP = 0x002D,
                .attack = 0x0001,
                .defense = 0x0001,
                .speed = 0x0001,
                .specialAttack = 0x0001,
                .specialDefense = 0x0001
            },{ // Miembro 5
                .species = 0xFB,
                .heldItem = 0x73,
                .moves = {0x47, 0x00, 0x00, 0x00},
                .otID = 0xA9C8,
                .exp_1 = 0x01,
                .exp_2 = 0x00,
                .exp_3 = 0x00,
                .evHP = 0x0000,
                .evAttack = 0x0000,
                .evDefense = 0x0000,
                .evSpeed = 0x0000,
                .evSpecial = 0x0000,
                .ivData = 0x0000,
                .movePP = {0x10, 0x00, 0x00, 0x00},
                .friendship = 0x9F,
                .pokerus = 0x00,
                .captureData = 0xC3,
                .level = 0x05,
                .status = 0x05,
                .currentHP = 0x002D,
                .maxHP = 0x002D,
                .attack = 0x0001,
                .defense = 0x0001,
                .speed = 0x0001,
                .specialAttack = 0x0001,
                .specialDefense = 0x0001
            },{ // Miembro 6
                .species = 0xFB,
                .heldItem = 0x73,
                .moves = {0x47, 0x00, 0x00, 0x00},
                .otID = 0xA9C8,
                .exp_1 = 0x01,
                .exp_2 = 0x00,
                .exp_3 = 0x00,
                .evHP = 0x0000,
                .evAttack = 0x0000,
                .evDefense = 0x0000,
                .evSpeed = 0x0000,
                .evSpecial = 0x0000,
                .ivData = 0x0000,
                .movePP = {0x10, 0x00, 0x00, 0x00},
                .friendship = 0x9F,
                .pokerus = 0x00,
                .captureData = 0xC3,
                .level = 0x05,
                .status = 0x05,
                .currentHP = 0x002D,
                .maxHP = 0x002D,
                .attack = 0x0001,
                .defense = 0x0001,
                .speed = 0x0001,
                .specialAttack = 0x0001,
                .specialDefense = 0x0001
            }
        },
        .trainerNames = {
            {pokechar_E, pokechar_U, pokechar_R, pokechar_O, pokechar_P, pokechar_E, 0x50, 0x00, 0x00, 0x00, 0x00},
            {pokechar_E, pokechar_U, pokechar_R, pokechar_O, pokechar_P, pokechar_E, 0x50, 0x00, 0x00, 0x00, 0x00},
            {pokechar_E, pokechar_U, pokechar_R, pokechar_O, pokechar_P, pokechar_E, 0x50, 0x00, 0x00, 0x00, 0x00},
            {pokechar_E, pokechar_U, pokechar_R, pokechar_O, pokechar_P, pokechar_E, 0x50, 0x00, 0x00, 0x00, 0x00},
            {pokechar_E, pokechar_U, pokechar_R, pokechar_O, pokechar_P, pokechar_E, 0x50, 0x00, 0x00, 0x00, 0x00},
            {pokechar_E, pokechar_U, pokechar_R, pokechar_O, pokechar_P, pokechar_E, 0x50, 0x00, 0x00, 0x00, 0x00}
        },
        .nicknames = {
            {pokechar_C, pokechar_E, pokechar_L, pokechar_E, pokechar_B, pokechar_I, 0x50, 0x00, 0x00, 0x00, 0x00},
            {pokechar_C, pokechar_E, pokechar_L, pokechar_E, pokechar_B, pokechar_I, 0x50, 0x00, 0x00, 0x00, 0x00},
            {pokechar_C, pokechar_E, pokechar_L, pokechar_E, pokechar_B, pokechar_I, 0x50, 0x00, 0x00, 0x00, 0x00},
            {pokechar_C, pokechar_E, pokechar_L, pokechar_E, pokechar_B, pokechar_I, 0x50, 0x00, 0x00, 0x00, 0x00},
            {pokechar_C, pokechar_E, pokechar_L, pokechar_E, pokechar_B, pokechar_I, 0x50, 0x00, 0x00, 0x00, 0x00},
            {pokechar_C, pokechar_E, pokechar_L, pokechar_E, pokechar_B, pokechar_I, 0x50, 0x00, 0x00, 0x00, 0x00}
        },
        .unusedBytes = {0x00, 0x00, 0x00}
    };
    
    // Flatten player struct into array
    //memcpy(DATA_BLOCK_GEN_II, &player, PLAYER_LENGTH_GEN_II);
    //memset(PATCH_BLOCK, 0, sizeof(PATCH_BLOCK));
    
    // Write the bytes to *out.
    uint8_t tmp_patch_list_1[100];
    uint8_t tmp_patch_list_2[100];
    size_t tmp_list_1_counter = 0;
    size_t tmp_list_2_counter = 0;
    for (size_t i = 0; i < PARTY_DATA_SIZE_GEN2; i++) {
        // Replace 0xFE with 0xFF (they will later be replaced again by the receiving game using the data we send in
        // the patch section).
        if (i >= 19 && DATA_BLOCK_GEN_II_array[i] == 0xFE) {
            DATA_BLOCK_GEN_II_array[i] = 0xFF;
            if (i < 0xFC) {
                tmp_patch_list_1[tmp_list_1_counter++] = i - 19 + 1;
            } else {
                tmp_patch_list_2[tmp_list_2_counter++] = i - 0xFC - 19 + 1;
            }
        }
    }
    tmp_patch_list_1[tmp_list_1_counter++] = 0xFF;
    tmp_patch_list_2[tmp_list_2_counter++] = 0xFF;

    size_t tmp_list_1_set_counter = 0;
    size_t tmp_list_2_set_counter = 0;
    // Now fill patch data.
    for (size_t i = PATCH_DATA_START_POS; i < PATCH_SIZE; i++) {
        if (tmp_list_1_counter > 0) {
            PATCH_BLOCK[i] = tmp_patch_list_1[tmp_list_1_set_counter++];
            tmp_list_1_counter--;
        } else if (tmp_list_2_counter > 0) {
            PATCH_BLOCK[i] = tmp_patch_list_2[tmp_list_2_set_counter++];
            tmp_list_2_counter--;
        }

        if (tmp_list_1_counter == 0 && tmp_list_2_counter == 0) {
            break;
        }
    }
    
    SC_REG = SIOF_CLOCK_INT;
    

    while (TRUE) {

        delay(8);

        in = sio_exchange_master(handle_byte(in, &trade_counter, &last_action));
    }
}
