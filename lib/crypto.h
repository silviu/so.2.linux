/*
 *  crypto.h
 *  
 *
 *  Created by Bogdan Drutu on 3/1/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */


/*
 * Functia cripteaza mesajul cu base_64
 * in - pointer catre zona de memorie unde se afla intrarea
 * len - lungimea pointer
 * out - pointer catre zona de memorie unde se afla output-ul
 * Functia returneaza lungimea mesajului de out
 */
int crypt_base64(void *in, int len, void *out);

/*
 * Functia decripteaza mesajul cu base_64
 * in - pointer catre zona de memorie unde se afla intrarea
 * len - lungimea pointer
 * out - pointer catre zona de memorie unde se afla output-ul
 * Functia returneaza lungimea mesajului de out
 */
int decrypt_base64(void *in, int len, void *out);

/*
 * Functia cripteaza mesajul cu des
 * in - pointer catre zona de memorie unde se afla intrarea
 * len - lungimea pointer
 * out - pointer catre zona de memorie unde se afla output-ul
 * Functia returneaza lungimea mesajului de out
 */
int crypt_des(void *in, int len, void *out);

/*
 * Functia decripteaza mesajul cu des
 * in - pointer catre zona de memorie unde se afla intrarea
 * len - lungimea pointer
 * out - pointer catre zona de memorie unde se afla output-ul
 * Functia returneaza lungimea mesajului de out
 */
int decrypt_des(void *in, int len, void *out);

/*
 * Functia cripteaza mesajul cu bf
 * in - pointer catre zona de memorie unde se afla intrarea
 * len - lungimea pointer
 * out - pointer catre zona de memorie unde se afla output-ul
 * Functia returneaza lungimea mesajului de out
 */
int crypt_bf(void *in, int len, void *out);

/*
 * Functia decripteaza mesajul cu bf
 * in - pointer catre zona de memorie unde se afla intrarea
 * len - lungimea pointer
 * out - pointer catre zona de memorie unde se afla output-ul
 * Functia returneaza lungimea mesajului de out
 */
int decrypt_bf(void *in, int len, void *out);

/*
 * Functia decripteaza mesajul cu rc2
 * in - pointer catre zona de memorie unde se afla intrarea
 * len - lungimea pointer
 * out - pointer catre zona de memorie unde se afla output-ul
 * Functia returneaza lungimea mesajului de out
 */
int crypt_rc2(void *in, int len, void *out);

/*
 * Functia decripteaza mesajul cu rc2
 * in - pointer catre zona de memorie unde se afla intrarea
 * len - lungimea pointer
 * out - pointer catre zona de memorie unde se afla output-ul
 * Functia returneaza lungimea mesajului de out
 */
int decrypt_rc2(void *in, int len, void *out);
