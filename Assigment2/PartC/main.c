/* Main source file for assignment 2 part A, which implements shitty homebrewn crypto together with
 * a nasty little stack based buffer overflow
 *
 * Copyright 2018 TUM
 * Created: 2018-11-15 Florian Wilde <florian.wilde@tum.de>
 **************************************************************************************************/
#include "packetizer.h"
#include "randombytes_salsa20XMC_random.h"
#include "sodium.h"
#include "VirtualSerial.h"

int main(void) {
  uint8_t key[32] = {
    0xc1,    0x00,    0x8a,    0x0c,    0x14,    0x80,    0x32,    0xeb,
    0x82,    0x06,   0x00,    0x10,    0x0a,    0x00,    0x00,    0x00,  //Unique chip id g_chip
    0xc1,    0x00,    0x8a,    0x0c,    0x14,    0x80,    0x32,    0xeb,
    0x82,    0x06,    0x00,    0x10,    0x0a,    0x00,    0x00,    0x00,
  };

pt_s plaintext = { 0 };
ct_s ciphertext = { 0 };

  USB_Init();

  randombytes_set_implementation(&randombytes_salsa20XMC_implementation);

  if (sodium_init() < 0) {
      /* panic! the library couldn't be initialized; it is not safe to use */
  }

  while(1) {
    // Consumes incoming data and parses it until we received a valid packet.
    //If so, it will fill the plaintextJob struct with the appropriate information. 
    if(packetizerReceive(&plaintext) == 0) {
      // If the sender did not sent a nonce, generate one 
      if(plaintext.nonce == NULL) {
        plaintext.nonce = malloc(CRYPTO_NONCE_BYTES);
        for(size_t i = 0; i < CRYPTO_NONCE_BYTES; i++) {
          plaintext.nonce[i] = rand() & UINT8_MAX;
        }
      }
      // Now encrypt the plaintext
      ciphertext.text = malloc(crypto_secretbox_MACBYTES + plaintext.textLen);
      if(crypto_secretbox_easy(ciphertext.text, plaintext.text, plaintext.textLen, plaintext.nonce,
                 key) == 0) {
        ciphertext.textLen = crypto_secretbox_MACBYTES + plaintext.textLen;
        //Finally, if encryption was successful, send ciphertext back in a packet 
        packetizerSend(&ciphertext);
      }
      // Free all buffers that were allocated in the meantime 
      free(plaintext.text);
      plaintext.text = NULL;
      free(ciphertext.text);
      ciphertext.text = NULL;
    }
  }
}