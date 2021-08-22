# Idash2021 - track2
## Team: LLHE
We are a team from Wuhan University of technology, Wuhan, China.This project is mainly to solve the idash2021 competition - Track II: Homomorphic Encryption-based Secure Viral Strain Classification

## Measured time (using 2000 validation datasets)

|model\process|  Preprocess [s]|  Inference [s]|
|----| ---- | ---- |
|version1|  14   |  0.172 |
|version2|  100  |  8.2   |

## Achieved Accuracy (using 2000 validation datasets)

|model|  Accuracy plaintext&ciphtext      |  
|----| ---- |
|version1|  98.6  &  98.55   |
|version2|  99.5  &  99.05   |


## How To Use It
put the hold-out dataset into the file folder.

docker build -t idash .
docker run -it idash /bin/bash

python3.8 preat.py #Get the test.txt file
g++ idash2021.cpp -p -main -ltfhe-spqlios-fma
./main

## Algorithm
## Preprocessing
version1:

A strain containing 29000 base pairs is transformed into a 7500 dimensional vector as the input of the neural network.

>B.1.427_5729AGATCTGTTCTCTAAACGAACTTTAAAATCTGTGTGGCT....
（AAAA->1，AAAC->2,…,TTTT->256,other->0, If the dimension is less than 7500, supplement -1）

so,around(29000)*2000 --> 7500*2000

version2:
If we are allowed to do excessive operations on preprocessing,we can also make statistics on the 7500 dimensional data obtained.

>B.1.427_5729AGATCTGTTCTCTAAACGAACTTTAAAATCTGTGTGGCT....
（AAAA->0，AAAC->1,…,TTTT->255，count the frequency of each gene string，each strain generated a 256 dimensional data）

around(29000)*2000 --> 256*2000

## Inference

DNN model

On the ciphertext side, we use packaging technology，which greatly reduces the consumption of time.


## TFHE
If you choose to run our program on a physical machine,please install the tfhe library according to the following link.
https://tfhe.github.io/tfhe/coding.html





