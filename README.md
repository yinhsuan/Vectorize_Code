# Vectorize_Code

### Part 1: Vectorizing Code Using Fake SIMD Intrinsics

#### Please enter the `part1` folder:
```shell
$ cd part1
```
#### Execute:
```shell
$ ./myexp
```
#### Execute & Specify N (workload size):
```shell
$ ./myexp -s 3
$ ./myexp -s 1000
$ ./myexp -s 10000
```
#### Execute & Print Log:
```shell
$ ./myexp -l
```

---

### Part 2: Vectorizing Code with Automatic Vectorization Optimizations

#### Please enter the `part2` folder:
```shell
$ cd part2
```

---

### Part 3: Q & A

### Q1-1: Does the vector utilization increase, decrease or stay the same as `VECTOR_WIDTH` changes? Why?

### <font color="green">Discuss the vector utilization in two aspect:</font>
#### In `logger.cpp`, we can get the equations:

#### $$\text{{vector utilization}} = \frac{{\text{{utilized lane}}}}{{\text{{total lane}}}} \quad (1)$$

#### $$\text{{utilized lane}} = \sharp \text{{ of instruction}} \times \sharp \text{{ of 1 in mask}} \quad (2)$$

#### $$\text{{total lane}} = \sharp \text{{ of instruction}} \times \text{{vector width}} \quad (3)$$

#### 1. From the Equation (3):

#### When `VECTOR_WIDTH` increases, the total lane would increase accordingly. Therefore, the vector utilization would decrease.

#### 2. From the Implementation Aspect:

#### When # of 1 in the mask increases, the utilized lane would increase. In clampedExpVector(), I use values of `exponents` to decide values of masks. If the value of the exponent is not yet zero, the mask lane of that exponent would be set to 1. Otherwise, the mask lane would be set to zero. In the end, the calculation would stop until all the values of the mask lane become zero.

#### If one value of the exponents is much larger than other exponents, it is likely that other values have already finished the operation of multiplication, and only that particular exponent is still calculating. This would cause vector utilization to be less efficient. With smaller `VECTOR_WIDTH`, this situation can be eased, and therefore the vector utilizaiton would be higher.

- `VECTOR_WIDTH` = 2, Vector Utilization = 90.7%
```shell
$ make && ./myexp -s 10000

CLAMPED EXPONENT (required)
Results matched with answer!
****************** Printing Vector Unit Statistics *******************
Vector Width:              2
Total Vector Instructions: 187516
Vector Utilization:        90.7%
Utilized Vector Lanes:     340001
Total Vector Lanes:        375032
************************ Result Verification *************************
ClampedExp Passed!!!
```
- `VECTOR_WIDTH` = 4, Vector Utilization = 88.7%
```shell
$ make && ./myexp -s 10000

CLAMPED EXPONENT (required)
Results matched with answer!
****************** Printing Vector Unit Statistics *******************
Vector Width:              4
Total Vector Instructions: 107072
Vector Utilization:        88.7%
Utilized Vector Lanes:     379945
Total Vector Lanes:        428288
************************ Result Verification *************************
ClampedExp Passed!!!
```
- `VECTOR_WIDTH` = 8, Vector Utilization = 87.7%
```shell
$ make && ./myexp -s 10000

CLAMPED EXPONENT (required)
Results matched with answer!
****************** Printing Vector Unit Statistics *******************
Vector Width:              8
Total Vector Instructions: 57878
Vector Utilization:        87.7%
Utilized Vector Lanes:     406001
Total Vector Lanes:        463024
************************ Result Verification *************************
ClampedExp Passed!!!
```
- `VECTOR_WIDTH` = 16, Vector Utilization = 87.2%
```shell
$ make && ./myexp -s 10000

CLAMPED EXPONENT (required)
Results matched with answer!
****************** Printing Vector Unit Statistics *******************
Vector Width:              16
Total Vector Instructions: 30093
Vector Utilization:        87.2%
Utilized Vector Lanes:     419857
Total Vector Lanes:        481488
************************ Result Verification *************************
ClampedExp Passed!!!
```
```diff
A1-1: The vector utilization decreases as `VECTOR_WIDTH` increases.
```

---
### Q1 Bonus: Implement a vectorized version of arraySumSerial in arraySumVector

### <font color="green">Time Complexity:</font> 
#### $$O({N \over vector \text{ } width} + log{_2}{(vector \text{ } width)})$$

- `VECTOR_WIDTH` = 2, Vector Utilization = 100%
```shell
$ make && ./myexp -s 10000

ARRAY SUM (bonus)
****************** Printing Vector Unit Statistics *******************
Vector Width:              2
Total Vector Instructions: 10000
Vector Utilization:        100.0%
Utilized Vector Lanes:     20000
Total Vector Lanes:        20000
************************ Result Verification *************************
ArraySum Passed!!!
```
- `VECTOR_WIDTH` = 4, Vector Utilization = 100%
```shell
$ make && ./myexp -s 10000

ARRAY SUM (bonus)
****************** Printing Vector Unit Statistics *******************
Vector Width:              4
Total Vector Instructions: 5000
Vector Utilization:        100.0%
Utilized Vector Lanes:     20000
Total Vector Lanes:        20000
************************ Result Verification *************************
ArraySum Passed!!!
```
- `VECTOR_WIDTH` = 8, Vector Utilization = 100%
```shell
$ make && ./myexp -s 10000

ARRAY SUM (bonus)
****************** Printing Vector Unit Statistics *******************
Vector Width:              8
Total Vector Instructions: 2500
Vector Utilization:        100.0%
Utilized Vector Lanes:     20000
Total Vector Lanes:        20000
************************ Result Verification *************************
ArraySum Passed!!!
```
- `VECTOR_WIDTH` = 16, Vector Utilization = 100%
```shell
$ make && ./myexp -s 10000

ARRAY SUM (bonus)
****************** Printing Vector Unit Statistics *******************
Vector Width:              16
Total Vector Instructions: 1250
Vector Utilization:        100.0%
Utilized Vector Lanes:     20000
Total Vector Lanes:        20000
************************ Result Verification *************************
ArraySum Passed!!!
```
- [Reference](https://people.cs.vt.edu/yongcao/teaching/cs5234/spring2013/slides/Lecture10.pdf)

---
### Q2-2: What speedup does the vectorized code achieve over the unvectorized code? You may wish to run this experiment several times and take median elapsed times; you can report answers to the nearest 100% (e.g., 2×, 3×, etc).

### <font color="green">Elapsed execution time in test1():</font>
> #### In order to run for multiple experiments to reduce measurement error on gettime(), `I` is defined as 20000000 in `test.h`. 

- #### Case1: Unvectorized Code:
```shell
$ make clean && make && ./test_auto_vectorize -t 1

Running test1()...
Elapsed execution time of the loop in test1():
11.602sec (N: 1024, I: 20000000)
```
- #### Case2: Vectorized Code:
```shell
$ make clean && make VECTORIZE=1 && ./test_auto_vectorize -t 1

Running test1()...
Elapsed execution time of the loop in test1():
2.89274sec (N: 1024, I: 20000000)
```
:::info
#### A2-2: The speedup the vectorized code achieves is 4.01 times faster than the unvectorized code.
:::

---
### What can you infer about the bit width of the default vector registers on the PP machines? 

### <font color="green">Derive the the bit width of the default vector registers in two ways:</font>
1. #### The Speed Up Time
#### The speedup of the vectorized code is about 4 times faster than the unvectorized code. Therefore, we can infer that the handling data of the vectorized code is 4 times more than the unvectorized code. Since, the data type is `float`, the bit width of the default vector registers is: $$4 (times) * 4 (bytes) * 8 (bits) = 128 (bits)$$

2. #### The Assembly Code
#### The instructions of `movaps`, `movups` and `maxps` access the data in the base of 16 bytes. In addition, according to [MOVAPS instruction reference](https://www.felixcloutier.com/x86/movaps.html), we can infer that the bit width of XMM registers is 128 (bits).

#### The following is the assembly code of the inner loop in the vectorized code:
```shell
.LBB0_3:                                #   Parent Loop BB0_2 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	movaps	(%r15,%rcx,4), %xmm0
	movaps	16(%r15,%rcx,4), %xmm1
	maxps	(%rbx,%rcx,4), %xmm0
	maxps	16(%rbx,%rcx,4), %xmm1
	movups	%xmm0, (%r14,%rcx,4)
	movups	%xmm1, 16(%r14,%rcx,4)
	movaps	32(%r15,%rcx,4), %xmm0
	movaps	48(%r15,%rcx,4), %xmm1
	maxps	32(%rbx,%rcx,4), %xmm0
	maxps	48(%rbx,%rcx,4), %xmm1
	movups	%xmm0, 32(%r14,%rcx,4)
	movups	%xmm1, 48(%r14,%rcx,4)
	addq	$16, %rcx
	cmpq	$1024, %rcx                     # imm = 0x400
	jne	.LBB0_3
```
:::info
#### A: The bit width of the default vector registers on the PP machines is 128 (bits).
:::

---
### Q2-3: Provide a theory for why the compiler is generating dramatically different assembly.

### <font color="green">The Assembly Code of test2():</font>
- #### Case1: Before Modifying Code
#### Use `movl`, `movss` and `movd` to move the value of a, b, c, and then use `ucomiss` to compare the value of a and b to decide whether to put b into c. And finally, use `jbe` to connect all operations.

#### The instructions are trival and are not concise at all!
```cpp
c[j] = a[j];
if (b[j] > a[j])
    c[j] = b[j];
```

```shell
.LBB0_7:                                #   in Loop: Header=BB0_3 Depth=2
	addq	$2, %rcx
	cmpq	$1024, %rcx                     # imm = 0x400
	je	.LBB0_8
.LBB0_5:                                #   in Loop: Header=BB0_3 Depth=2
	movl	4(%r15,%rcx,4), %edx
	movl	%edx, 4(%rbx,%rcx,4)
	movss	4(%r14,%rcx,4), %xmm0           # xmm0 = mem[0],zero,zero,zero
	movd	%edx, %xmm1
	ucomiss	%xmm1, %xmm0
	jbe	.LBB0_7
# %bb.6:                                #   in Loop: Header=BB0_3 Depth=2
	movss	%xmm0, 4(%rbx,%rcx,4)
	jmp	.LBB0_7
```

- #### Case2: After Modifying Code
#### Use `movaps`, `maxps` to do both the operation and comparation. `maxps` can be used to find the max value and store the value into the register.

#### This method save the # of operation and the used registers, which can process more data at the same time.
```cpp
if (b[j] > a[j]) c[j] = b[j];
else c[j] = a[j];
```

```shell
.LBB0_3:                                #   Parent Loop BB0_2 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	movaps	(%r15,%rcx,4), %xmm0
	movaps	16(%r15,%rcx,4), %xmm1
	maxps	(%rbx,%rcx,4), %xmm0
	maxps	16(%rbx,%rcx,4), %xmm1
	movups	%xmm0, (%r14,%rcx,4)
	movups	%xmm1, 16(%r14,%rcx,4)
	movaps	32(%r15,%rcx,4), %xmm0
	movaps	48(%r15,%rcx,4), %xmm1
	maxps	32(%rbx,%rcx,4), %xmm0
	maxps	48(%rbx,%rcx,4), %xmm1
	movups	%xmm0, 32(%r14,%rcx,4)
	movups	%xmm1, 48(%r14,%rcx,4)
	addq	$16, %rcx
	cmpq	$1024, %rcx                     # imm = 0x400
	jne	.LBB0_3
```
:::info
#### A2-3: We can get better performance if the vectorized assembly code use `movaps` and `maxps`, instead of `ucomiss` and `jbe`.
:::
