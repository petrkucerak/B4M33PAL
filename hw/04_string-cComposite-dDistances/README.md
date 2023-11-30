# String Composite Distances
University stringology team is studying less traditional approaches to string processing and text search.
For two strings S and T, the team introduced a concept named elementary similarity, denoted byES(S, T). It is defined by the relation
ES(S, T) = |S|, when S and T are identical strings, and ES(S, T) = 0, otherwise.


For two nonempty strings S and T, the team introduced a concept named composite distance, denoted by CD(S, T), which is the sum of elementary similarities of all pairs of substrings, where the first element of the pair is a substring of S and the second element of the pair is a substring of T.
More formally, CD(S, T) = sum( ES(s, t); s is a substring of S, t is a substring of T ).

For example,
```
S = "010", T = "01".
ES(S, T) = 0.
CD(S, T) = ES("0", "0") + ES("1", "1") + ES("0", "0") + ES("01", "01") = 1 + 1 + 1 + 2 = 5.
```
Here the sum includes only non-zero terms, for better readability. For the same reason we write each substring in double quotes.
Note that the pair of substrings ("0", "0") appears twice in the sum, the elements in the first pair in the sum are the first character in S and the first character in T, the elements in the third pair in the sum are the third character in S and the first character in T.

Another example:
```
     S = "0101", T = "1010".
     Composite distance of S and T is a sum
     CD(S, T) =
        ES("0", "0") + ES("0", "0") + ES("0", "0") + ES("0", "0")
     + ES("1", "1") + ES("1", "1") + ES("1", "1") + ES("1", "1")
     + ES("01", "01") + ES("01", "01")
     + ES("10"," 10") + ES("10", "10")
     + ES("010", "010") + ES("101", "101")
     = 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 2 + 2 + + 2 + 2 + 3 + 3 = 22.
```
Again, note the repeating terms, due to the different positions of the substrings in their respective strings.

After some considerations, the team decided to restrict the concept of the composite distance. When it is computed, possibly all substrings of all lengths in S and T have to be considered. As their number may be relatively large, the team added two more parametrs, L1 and L2, to composite distance, creating the concept of restricted composite distance of S and T.
Given two integers L1 ≤ L2, restricted composite distance of S and T, denoted by RCD(S, T, L1, L2) is the sum of elementary similarities of all pairs of substrings, where the first element of the pair is a substring of S and the second element of the pair is a substring of S and the length of both elements in the is in the range [L1 .. L2].
For example:
```
     RCD("011001", "1100", 2, 3) =
     ES("11","11") + ES("10","10") + ES("00","00") + ES("110","110") + ES("100","100")
     = 2 + 2 + 2 + 3 + 3 = 12.
```

To compute RCD(S, T, L1, L2), the teams apllies a dictionary tree, often also called trie or digital tree or prefix tree, built over all substrings of S, which length is in range [L1 .. L2].
Managing the tree takes time, therefore the team is inderested in more characterisctics of the tree, namely the number of leaves in the tree and the total length of all paths from the root of the tree to a leaf.

## The task
Given S, T, L1, L2, compute RCD(S, T, L1, L2). Also compute, for the dictionary tree (trie, digital tree, prefix tree) built over the dictionary of all substrings of S which length is in the range [L1 .. L2], the number of the leaves and the total length of all paths from the root of the dictionary tree (trie) to its leaf.

## Input
The first input line contains four integers LS, LT, L1, L2, where LS and LT is the length of string S, T, respectively, and L1 and L2 is the third and fourth parameter of RCD(S, T, L1, L2). The second and the third input line contains string S and T, repectively.
Both S and T are composed only of characters '0' and '1'.
It holds, 2 ≤ |S|, |T| ≤ 104; 1 ≤ L1 ≤ L2 ≤ min( |S|, |T| ).

## Output
The output contains one text line with three integers D, L, P, separated by spaces. The value of D is equal to RCD(S, T, L1, L2). The value of L is equal to the number of leaves in a dictionary tree (trie, digital tree, prefix tree) built over S. The value of P refers to the same dictionary tree (trie, digital tree, prefix tree) and it is equal to the total length of all paths from the root of the tree to a leaf.

## Example 1
### Input
```
8 8 3 4
10101010
01010101
```
### Output
```
102 2 8
```
## Example 2
### Input
```
8 8 1 8
10101010
01010101
```

### Output
```
260 2 15
```
## Example 3
### Input
```
8 13 5 8
10100101
0101001001010
```
### Output
```
68 4 26
```
