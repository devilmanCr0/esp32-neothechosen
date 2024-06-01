Prime generation for large numbers becomes somewhat of a dart throw.
you're essentially picking a random large number to then perform a Rabin Miller Primality Test
for the number's likelihood of being prime

you're given a number n as your prime candidate

let n-1 be 2^s * d where s is a positive integer and d is an odd positive integer.

we make a new integer called "a", which will be a coprime to n.

n is a strong probable prime to base "a" if (let =- denote congruence evaluation)

a^d =- 1 (mod n)
a^((2^r)*d) =- -1 (mod n)

You first check if the first expression is true, then repeat the second expression for every successive value
this does not gaurantee an evaluation of prime, as there exists strong liars that are pseudo-prime. Increasing iteration
count for successive runs yields higher accuracy, but for our project we don't need too many.
