# B Tree

## Introduction
In computer science, a B-tree is a self-balancing tree data structure that maintains sorted data and allows searches, sequential access, insertions, and deletions in logarithmic time. 

## Definition
B-tree of order m is a tree which satisfies the following properties:

**1.** Every node has at most m children.
**2.** Every non-leaf node (except root) has at least ?m/2? child nodes.
**3.** The root has at least two children if it is not a leaf node.
**4.** A non-leaf node with k children contains k ? 1 keys.
**5.** All leaves appear in the same level and carry no information.

## Source Code
Source code include implementation of B -tree. Code provide functionality to perform operations such as insertion, deletion, search. 


