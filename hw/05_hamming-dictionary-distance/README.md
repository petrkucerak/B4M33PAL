# Hamming dictionary distance

Let $D = \{d_1,..., d_m\}$ be a dictionary of $m$ words, whose length is at least $F$ (where $F ≥ 3$), over an alphabet $A$. For two words $u$, $v$ of the same length, let $HD(u, v)$ denote their Hamming distance. For a text $T$ over $A$, we say that $T = w_1 ... w_k$ is a feasible factorization of $T$ with respect to $D$ and $F$ if and only if

- for all $i = 1,..., k,$ either $1 ≤ |w_i| < F$, or $|w_i| = |d_j|$ for some $j ∈ {1,..., m}$,
- there is no $1 ≤ i < k$ such that $|w_i| < F$ and $|w_i+1| < F$.

For a feasible factorization $T = w_1...w_k$, we define its Hamming distance to $D$ as the sum $C(w_1) + ... + C(w_k)$, where
- $C(w_i) = 1$, if $|w_i| < F$,
- $C(w_i) = min\{HD(w_i, d) : d ∈ D ∧ |d| = |w_i|\}$, otherwise.

We define the Hamming dictionary distance $HDD(T, D, F)$ of $T$ with respect to $D$ and $F$ as the as the minimum Hamming distance of $T$ to $D$ over all feasible factorizations of $T$.