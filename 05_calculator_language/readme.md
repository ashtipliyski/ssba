# Simple Calculator Language

## Objective

Implement a program to evaluate simple mathematical expressions of the sort `5 * (-2 - (3+4))`.

## Scanning 

In this step the input command is parsed and individual tokens are constructed for the different elements of the expression.

Minimal set of tokens to consider 

- individual digits
- basic arithmetic operations (`+ -`, i.e. `1+2`)
- unary negation (`-`, i.e. `-2`) 

Extended set of tokens

- arbitrary whitespace between tokens
- multi-digit integers
- decimal values
- multiplication, division
- exponentiation

## Parsing

The list of tokens is further parsed to construct an Abstract Syntax Tree (AST) of the command.

## Evaluation

Having an AST for the command, one can write a simple evaluation function that traverses the AST and evaluates the expressions.
