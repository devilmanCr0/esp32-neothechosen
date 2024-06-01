The premise of this type of attack is that we're given the mechanism to decrypt and encrypt any message we want (except the flag directly)

We have: 
cf = the ciphertext of the flag
pt = plaintext that we know
ct = ciphertext from the plaintext we know

let's redefine the ciphertext flag as c

c = m^e mod p where m is the flag plaintext

We can't directly decrypt c as the mechanism that we have access to doesn't allow that, so instead the premise
is to send another message to retreive the flag!

now imagine we encrypt the message 2 (literally the number 2), but we multiply it with the ciphertext message

c = c * 2^e 

Here's the full equation that shows how this lets us obtain the flag ( let =- denote the congruency equals and %| denote phi)

(c * 2^e)^d mod n =- (c^d * 2^(ed)) mod n (c^d * 2^(ed)) mod n =- ((c^d mod n) * (2^(ed) mod n)) mod n ((c^d mod n) * (2^ed mod n)) mod n =- (m * (2^(1+k*%|(n)) mod n)) mod n (m*(2^1 + k*%|(n)) mod n) =- (m * 2 * (2^(k*%|(n)) mod n)) mod n

A holy mess, but it ends up to look something like this

2(m * (2^(k%|(n) mod n)) mod n =- 2 * m mod n

To get a better sense of understanding, we simply just need to send the following input

c * (2^e mod n) mod n

which should just take that output, divide it by 2, and get the plaintext flag


Pseudo code - derived from "masterpessimistaa" 
https://masterpessimistaa.wordpress.com/2018/03/04/pragyan-ctf-rsas-quest/

Everything you need to learn about modulus arithmetic
https://pi.math.cornell.edu/~morris/135/mod.pdf
https://www.math.utah.edu/~fguevara/ACCESS2013/Euclid.pdf

More on chosen cipher attacks
https://crypto.stackexchange.com/questions/89269/chosen-ciphertext-attack-textbook-rsa



