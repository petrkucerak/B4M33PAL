# Hamming dictionary distance

Let $D = \{d_1,..., d_m\}$ be a dictionary of $m$ words, whose length is at least $F$ (where $F ≥ 3$), over an alphabet $A$. For two words $u$, $v$ of the same length, let $HD(u, v)$ denote their Hamming distance. For a text $T$ over $A$, we say that $T = w_1 ... w_k$ is a feasible factorization of $T$ with respect to $D$ and $F$ if and only if

- for all $i = 1,..., k,$ either $1 ≤ |w_i| < F$, or $|w_i| = |d_j|$ for some $j ∈ {1,..., m}$,
- there is no $1 ≤ i < k$ such that $|w_i| < F$ and $|w_i+1| < F$.

For a feasible factorization $T = w_1...w_k$, we define its Hamming distance to $D$ as the sum $C(w_1) + ... + C(w_k)$, where
- $C(w_i) = 1$, if $|w_i| < F$,
- $C(w_i) = min\{HD(w_i, d) : d ∈ D ∧ |d| = |w_i|\}$, otherwise.

We define the Hamming dictionary distance $HDD(T, D, F)$ of $T$ with respect to $D$ and $F$ as the as the minimum Hamming distance of $T$ to $D$ over all feasible factorizations of $T$.

## The task

For a given $T$, $D$ and $F$, calculate $HDD(T, D, F)$.

## Input

The first input line contains integers $N$, $F$ separated by space. The next $N$ lines represent $N$ words of a dictionary $D$, each line contains one word over $A = \{'a',..., 'z'\}$. Note that these words are not necessarily different from each other. One more line follows, containing a text $T$ over $A$.
The following limits hold: $1 ≤ N ≤ 130$; $3 ≤ F ≤ 20$; $1 ≤ |T| ≤ 140000$; each word of $D$ is of length at least $F$ and not greater than $130$.
It is ensured that there is at least one feasible factorization of $T$ with respect to $D$ and $F$.

## Output
The output consists of one line containing integer $H$ which is equal to $HDD(T, D, F)$.

## Example 1
### Input
```
3 3
abab
aaa
bbbaa
ababbbbbaaaaacc
```

### Output
```
2
```
`abab|b|bbbaa|aaa|cc` is an optimal feasible factorization of the text, consisting of three words from the dictionary and two substrings of length less than $F = 3$.

## Example 2
### Input
```
2 4
abab
cdcd
cddcdababbcddabcdcd
```
### Output
```
5
```

An optimal feasible factorization: `c|ddcd|abab|bc|ddab|cdcd`.

## Example 3
### Input
```
1 3
lstnnt
tntlsdtnttntlsdlsdtnt
```
### Output
```
13
```
An optimal feasible factorization: `tntlsd|t|nttntl|sd|lsdtnt`.
