# Mersenne Twister


## Abstract

Mersenne Twister (MT) is one of the most widely used general-purpose pseudorandom number generator (PRNG). Its name derives from the fact that its period length is chosen to be a [Mersenne prime](https://en.wikipedia.org/wiki/Mersenne_prime).

The most commonly used version of the MT algorithm is based on the Mersenne prime $2^{19937} - 1$ (aka `MT19937`). The standard implementation of MT19937 uses a $32$-bit word length. There is also another implementation (variants) that uses a $64$-bit word length (aka `MT19937-64`).

In this project, we will reimplement MT19937 algorithm using C/C++ (comparing correctness with [C++ built-in functions](https://cplusplus.com/reference/random/): `std::mt19937`, `std::mt19937_64`) Then we will suggest some methods to attack Mersenne Twister (included C/C++ or Python scripts).

**IMPORTANT: Mersenne Twister is not cryptographically secure.**

## Algorithm Overview

In this section, we will recall three main parts in MT algorithm.

### 1. Initialization

The state needed for a Mersenne Twister implementation is an array of $n$ values of $w$ bits each. To initialize the array, a $w$-bit `seed` value is used to supply $x_{0}$ through $x_{n-1}$ by following algorithm

$$
\begin{cases}
    x_{0} &= seed \\
    x_{i} &= f \times \left(x_{i-1} (x_{i-1} \gg (w - 2)) \right) + i
\end{cases} \qquad \forall i=\overline{1,n}.
$$

### 2. Twisting

The MT algorithm generates a sequence of word vectors, which are        considered to be uniform pseudorandom integers between $0$ and $2^{w} - 1$. The algorithm is based on the following linear recurrence

$$
x_{k+n} = x_{k+m} \oplus \left(x_{k}^{u} \vert x_{k+1}^{l}\right)A \qquad \forall k=0,1,2,\cdots
$$

We have several constants:

- An integer $n$: the degree of the recurrence.
- $x_{k}^{u}$: the upper $w - r$ bits of $x_{k}$.
- $x_{k+1}^{l}$: the lower $r$ bits of $x_{k+1}$.
- $x_{k}^{u} \vert x_{k+1}^{l}$: is just concatenation.
- $A$: is a chosen matrix so that multiplication by $A$ is very fast:

$$
A = 
\begin{bmatrix}
    & &1 & & & \\
    & & &1 & & \\
    &\text{} &\text{} &\text{} &\ddots &\text{} \\
    &\text{} &\text{} &\text{} &\text{} &1 \\
    &a_{w-1} &a_{w-2} &a_{w-3} &\cdots &a_{0} 
\end{bmatrix}
$$

then the calculation of $\mathbf{x}A$ can be done using only bit operations:

$$
\mathbf{x}A = 
\begin{cases}
    \mathbf{x} \gg 1 \quad &\text{ if } x_{0} = 0 \\
    \left(\mathbf{x} \gg 1\right) \oplus \mathbf{a} \quad &\text{ if } x_{0} = 1 
\end{cases},
$$

where $\mathbf{a} = \left(a_{w-1}, a_{w-2}, \cdots, a_{0}\right)$ and $\mathbf{x} = \left(x_{w-1}, x_{w-2}, \cdots, x_{0}\right)$.

### 3. Tempering

The tempering operation to tamper a state register to the produced $32$-bit output. For each state value $\mathbf{x}$, we calculate the $32$-bit output $\mathbf{y}$ by following steps:

$$
\begin{align*}
    \mathbf{y} &= \mathbf{x} \oplus (\mathbf{x} \gg u) \\
    \mathbf{y} &= \mathbf{x} \oplus ((\mathbf{x} \ll s) \text{ AND } \mathbf{b}) \\
    \mathbf{y} &= \mathbf{x} \oplus ((\mathbf{x} \ll t) \text{ AND } \mathbf{c}) \\
    \mathbf{y} &= \mathbf{x} \oplus (\mathbf{x} \gg l) \\
\end{align*}
$$

where $l, s, t$ and $u$ are integers, $\mathbf{b}$ and $\mathbf{c}$ are suitable bitmasks of word size.

**IMPORTANT:  Tempering step is reversible**.

## Implement

- [mt19937](/src/mt19937.cpp)

## Some attacks

Update latter...

# License

- [LICENSE](/LICENSE)