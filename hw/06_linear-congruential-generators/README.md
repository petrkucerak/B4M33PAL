# Finding linear congruential generators

A linear congruential generator (LCG) is a triple of integers ($A$, $C$, $M$) used to generate pseudorandom numbers by the formula $x_{n+1} = (Ax_n + C)$ mod $M$ applied recurrently to an initial value (seed) $x_0$.

The Hull-Dobell Theorem says that an LCG ($A$, $C$, $M$) has full period if and only if the following holds:

1. $C$ and $M$ are coprimes,
2. $A − 1$ is divisible by each prime factor of $M$,
3. if $4$ divides $M$, then $4$ also divides $A − 1$.

We know that a certain LCG ($A$, $C$, $M$) with full period was used to generate $x_1$ in one step from a seed $x_0$. In addition, we have the following information:

- $M$ has exactly $F ≥ 1$ prime factors (i.e., $M = p_1^{a_1}... p_F^{a_F}$ for some distinct primes pi and positive integers $a_i$),
- $M ≤ M_{max}$, where $M_{max}$ is a given bound,
- $0 ≤ x_0$, $x_1 < M$,
- $0 < C < M$,
- $1 < A < M$,
- there is no integer $A'$, such that $1 < A' < A$ and $A'$ fulfills conditions $2.$ and $3.$ of the Hull-Dobell Theorem with respect to $M$.

## The task

Find all LCGs with the properties specified above that generate a given $x_1$ from a given $x_0$.

## Input

The input contains one line with integers $F$, $M_{max}$, $x_0$ and $x_1$ separated by spaces. The following limits hold:
$1 ≤ F ≤ 8$; $2 ≤ M_{max} ≤ 10^9$; $0 ≤ x_0$, $x_1 < M_{max}$.

## Output

The output consists of one line containing integer $G$ which is equal to the number of feasible LCGs.

## Example 1
### Input
```
2 20 7 6
```
### Output
```
1
```
There is only one feasible LCG ($A = 7$, $C = 11$, $M = 18$). We can verify that ($7 × 7 + 11$) mod $18 = 6$.
## Example 2
### Input
```
3 120 0 37
```
### Output
```
2
```
The two feasible LCGs are ($A = 31$, $C = 37$, $M = 90$) and ($A = 61$, $C = 37$, $M = 120$).