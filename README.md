# Latency-Constrained-Scheduling
In Project 2, we have implemented two versions of constrained list scheduling algorithms.
In this project, you are required to further minimize the hardware resource of the
latency-constrained scheduling algorithm taking operation probabilities and type distributions
into account (e.g., force-directed latency-constrained scheduling, FD-LCS). Your program
would be evaluated on Linux environment according to the following requirements.


1. For simplicity, there are only 3 types of Boolean operations: AND, OR, and NOT.
2. Assume that every operation takes 1-cycle latency. (The PI node is not an operation.)
3. Read a BLIF file and the latency constraint.
4. Write a report with your own pseudo code and detailed explanation of your algorithm.
5. Compare and discuss the LCS results of your Project02 and Project03 in your report.
6. Upload your source code tarball (*.tgz) to moodle (including your Makefile)
(NOTE: The uploaded file name should be the same with your student ID.)
7. Generate and upload one "difficult" test case with ReadMe file.



BLIF Example: sample02.blif \
.model sample02 \
.inputs a b c d e f \
.outputs o p q \
.names a d g \
1- 1 \
-1 1 \
.names a c h
11 1
.names c i
0 1
.names d e f j
1-- 1
-1- 1
--1 1
.names g h i k
1-- 1
-1- 1
--1 1
.names h i j l
111 1
.names i j m
11 1
.names l m n
11 1
.names b h k o
111 1
.names g p
0 1
.names n q
0 1
.end
SYNOPSIS for LCS
%> lcs BLIF_FILE LATENCY_CONSTRAINT
Run-time Example:
%> lcs sample02.blif 5
Latency-constrained Scheduling
1: {h} {j} {i}
2: {m} {} {}
3: {l} {g} {}
4: {n} {k} {p}
5: {o} {} {q}
#AND: 1
#OR: 1
#NOT: 1
END
Run-time Example:
%> lcs sample02.blif 4
Latency-constrained Scheduling
1: {h} {j} {i}
2: {l m} {g} {}
3: {n} {k} {p}
4: {o} {} {q}
#AND: 2
#OR: 1
#NOT: 1
END
Run-time Example:
%> lcs sample02.blif 3
Latency-constrained Scheduling
No feasible solution.
END
