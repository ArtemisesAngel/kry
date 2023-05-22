#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*\
@@       ------Krypton hashing algorithm------      @@
@@           programmed by ArtemisesAngel           @@
@@                                                  @@
@@    no license, steal the code for all I care     @@
@@          algorithm designed by n0tA1dan          @@
\*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

char *krypton(char *plain, int plain_len, char key[40]);

int main(int argc, char *argv[]){
	if (argc<2){
		printf("krypton 320 bit hashing algorithm");
		printf("usage: ./krypton <COMMAND> [OPTIONS] ...\n");
		printf("options:\n");
		printf("    genkey <OUTPUT_FILE>\n");
		printf("    hash <PLAINTEXT_FILE> <KEY_FILE>\n");
		return 0;
	}
	if (strcmp("hash", argv[1])==0){
		if (argc<4){
			printf("*** krypton 320 bit hashing algorithm ***\n");
			printf("options: hash\n");
			printf("usage: ./krypton hash <PLAINTEXT_FILE> <KEY_FILE>\n");
			printf("hashes the input file with the keygen");
			return 0;
		}
		FILE * plaintext_fp;
		plaintext_fp = fopen(argv[2], "r"); //plaintext file
		FILE * key_fp;
		key_fp = fopen(argv[3], "r"); //key file
		fseek(plaintext_fp, 0L, SEEK_END); //seek end of file
		int plaintext_size = ftell(plaintext_fp); //size of file in bytes
		char *plaintext = (char *) malloc(plaintext_size); //alocate memory to read file
		fgets(plaintext, plaintext_size, plaintext_fp); //read plaintext
		char *key = (char *) malloc(40);
		fgets(key, 	40, key_fp); //read key

		printf("hex digest of hash:\n");
		char *digest = krypton(plaintext, plaintext_size, key);
		for (int i=0;i<8;i++){
			printf("%08x", digest[i]);
		}
	}
	else if (strcmp("genkey", argv[1])==0){
		if (argc<3){
			printf("krypton 320 bit hashing algorithm");
			printf("options: keygen\n");
			printf("usage: ./krypton hgenkey <OUTPUT_FILE>\n");
			printf("generates a key for hashing\n");
			return 0;
		}
		FILE *key_fp;
		key_fp = fopen(argv[2], "w");
		char key_buf[40];
		arc4random_buf(key_buf, 40);
		fputs(key_buf, key_fp);
	}
	return 0;
}


char *krypton(char *plain, int plain_len, char *key){
	char *digest = malloc(8);
	char *temp = malloc(8);
	//plain_len is number of bytes
	if (plain_len%8){ //if plain is not multiple of 8
		for(int i=0; i<8-plain_len%8; i++){ //pad the message to right size with 0 bytes
			plain[plain_len+i] = 0x0;
		}
	}
	int num_blocks = plain_len/8 + ((bool) plain_len%8);
	for (int block=0; block<plain_len/8; block++){ //main algorithm for processing blocks
		int key_section = (5-(block%5));
		for(int i=0; i<8; i++){ //copy plain
			digest[i] ^= plain[block*8+i] ^ key[key_section+i];
		}
		for (int i=0; i<8; i++){ //shift digest right 1 into temp
			temp[(i+1)%8] = digest[i];
		}
		for (int i=0; i<8; i++){ //copy temp back to digest
			digest[i] = temp[i];
		}
	}
	return digest;
}
