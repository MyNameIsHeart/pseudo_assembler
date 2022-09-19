# Pseudo Assembler

[[Source Code]][src]

[src]:          ./source

A multi-pass pseudo assembler for a predefined, made-up, Assembly-type language.
Developed in C.

<p align="center">
<img src="./images/assembler.png" alt="assembler.png" width="499" height="133">
</p>

## Compilation

This project includes a makefile for easy compilation on linux distributions:

* make:

```sh
    $ make
```

<p align="center">
  <img src="./images/make.png" alt="make.png" width="738">
</p>

<details><summary>Additional rules</summary>

</br>

* compile:

```sh
    $ make compile
```

<p align="center">
  <img src="./images/compile.png" alt="compile.png" width="738">
</p>

* clean:

```sh
    $ make clean
```

<p align="center">
  <img src="./images/clean.png" alt="clean.png" width="738">
</p>

* rm:

```sh
    $ make rm
```

<p align="center">
  <img src="./images/rm.png" alt="rm.png" width="738">
</p>

* full_clean:

```sh
    $ make full_clean
```

<p align="center">
  <img src="./images/full_clean.png" alt="full_clean.png" width="738">
</p>

## Usage

The input should consist of one assembly file with the following extension - '.asm'.
The output should consist of three files, with the following extensions - '.bin', '.ent' and '.ext'.

```sh
    $ ./assembler [name_of_file].asm
```
