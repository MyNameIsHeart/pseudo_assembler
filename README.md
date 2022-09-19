# Pseudo Assembler [![License][license-badge]][license]

[license-badge]: https://img.shields.io/github/license/ajeetdsouza/zoxide?color=lightgray&style=flat-square
[license]: ./LICENSE

[[Source Code]][src]

[src]:          ./source

A multi-pass pseudo assembler for a predefined, made-up, Assembly-type language.
Developed in C.

<p align="center">
<img src="./images/assembler.png" alt="assembler.png" width="499" height="133">
</p>

## Table of Contents
- [Compilation](#Compilation)
- [Usage](#Usage)

## Compilation

This project includes a makefile for easy compilation on linux distributions:

* make:

```sh
    $ make
```

<p align="center">
  <img src="./images/make.png" alt="make.png" width="950">
</p>

<details><summary>Additional rules</summary>

</br>

* compile:

```sh
    $ make compile
```

<p align="center">
  <img src="./images/compile.png" alt="compile.png" width="650">
</p>

* clean:

```sh
    $ make clean
```

<p align="center">
  <img src="./images/clean.png" alt="clean.png" width="1000">
</p>

* rm:

```sh
    $ make rm
```

<p align="center">
  <img src="./images/rm.png" alt="rm.png" width="750">
</p>

* full_clean:

```sh
    $ make full_clean
```

<p align="center">
  <img src="./images/full_clean.png" alt="full_clean.png" width="900">
</p>

</details>

## Usage

The input should consist of one or more assembly files ('.asm' files).

```sh
    $ ./assembler.out [name_of_file].asm
```

The output should consist of one to three files:</br>
- The machine code, in the form of an object file ('.o')</br>
- A file with information about external symbols ('.ext'); won't be created if the .extern command wasn't in use</br>
- A file with information about entry symbols ('.ent'); won't be created if the .entry command wasn't in use</br>

<p align="center">
  <img src="./images/example_run.png" alt="example_run.png" width="500">
</p>

