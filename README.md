********** GROUP INFORMATION **********

S. No.		Member				ID
1.		Arjun Tyagi			2021B3A72272H
2.		Kush Gadia			2021B4A72386H
3.		Divya Ramanan			2022A7PS2009H
4. 		Taanvi Dande			2022A7PS2007H
5.		Mahima Pragnya Bhattaram	2022A7PS2018H

----------X----------X----------

********** EXECUTION INSTRUCTIONS **********

On the Ubuntu terminal, write the following command to increase stack size: 
ulimit -s unlimited
This would ensure that the codes do not abruptly terminate due to too many recursive calls.

Install g++ compiler (if not already installed) using the following commands in the terminal: 
sudo apt update
sudo apt build-essential

In this file, 
First algorithm refers to the algorithm explained under 'The Exact Method' and is Algorithm 1 of the paper titled 'Efficient Algorithms for Densest Subgraph Discovery'
Second algorithm refers to the algorithm explained under 'The Code-Based Exact Method' and is Algorithm 4 of the paper titled 'Efficient Algorithms for Densest Subgraph Discovery'

In general, we recommend having all of the following in the same directory: 
01. txt file of the dataset
02. preprocess.cpp file
03. algo1.cpp file
04. algo3.cpp file
05. algo4.cpp file
Exact dependecies and details about preprocessing are mentioned below. 


Executing the code for the first algorithm -

Before executing the code for the first algorithm, ensure that the txt file of the dataset that you want to run it on is present in the same directory as the code (algo1.cpp). In the code, change the name of the file to the name of the dataset (for example: 'input.txt'). 

For executing the code, run the following commands in the terminal:
g++ algo1.cpp -o algo1
./algo1


Executing the code for the second algorithm -

Before executing the code for the second algorithm, ensure that the txt file of the dataset that you want to run it on and algo3.cpp are present in the same directory as the code (algo4.cpp). In the codes, change the name of the file to the name of the dataset (for example: 'input.txt'). 

For executing the code, run the following commands in the terminal:
g++ -std=c++17 -o program algo3.cpp
./program
g++ algo4.cpp -o algo4
./algo4

----------X----------X----------

********** DATASET PREPARATION **********

For executing the codes of the two algorithms, some preprocessing is required for the datasets. The datasets should be in txt format and the graphs must be 0-indexed. A separate file called preprocess.cpp has been added to the zip file for specific preprocessing of certain datasets. Some datasets have been synthetically generated too. 

----------X----------X----------

********** INDIVIDUAL CONTRIBUTIONS **********

We shall first explain how the work was structured, and then explain how each member contributed individually. 

This is how our work was structured  - 
A. We started off by inidividually reading and understanding both the algorithms we had to implement. 
B. Next, we had a group discussion where we talked about and helped each other improve our understanding of each of the three algorithms. 
C. We then divided work amongst ourselves. 
D. We set small deadlines for portions of the work, and had online meets or calls frequently to discuss and give updates about our progress. In these meets, we also took feedback and planned the work ahead. 
E. After completion, we are now sitting down together once again to compile everything and shall submit the assignment soon.

Following are individual contributions in the divided work - 

1. Arjun Tyagi
- worked on the implementation, debugging, and testing of the first algorithm
- created the front end (webpage) to host the assignment files
- drafted, edited, and reviewed the project report
- will proofread the readme file

2. Kush Gadia
- worked on the implementation, debugging, and testing of the first algorithm
- drafted, edited, and reviewed the project report
- plotted the histograms for the observations
- writing the readme file

3. Divya Ramanan 
- worked on finding the datasets for the testing of the algorithms
- worked on the implementation, debugging, and testing of the second algorithm
- compiled, analysed, and documented all obserservations after running both the codes on all the datasets
- drafted, edited, and reviewed the project report

4. Taanvi Dande
- worked on the implementation of the code for preprocessing the datasets 
- worked on the implementation, debugging, and testing of the second algorithm
- compiled, analysed, and documented all obserservations after running both the codes on all the datasets
- drafted, edited, and reviewed the project report

5. Mahima Pragnya Bhattaram
- worked on finding the datasets for the testing of the algorithms
- worked on the implementation, debugging, and testing of the second algorithm
- compiled, analysed, and documented all obserservations after running both the codes on all the datasets
- drafted, edited, and reviewed the project report

----------X----------X----------

********** LINKS TO DATASETS **********

Below are the links to the datasets that have been used in the testing of the implementation of the algorithms.

1. https://websites.umich.edu/~mejn/netdata/


2. https://drive.google.com/file/d/1X34wfoY-IY1YGYq7FeDZQIgmQRZ2n2c1/view?usp=drive_link


3. https://drive.google.com/file/d/1tSyst3u72u2QParcGrXkm_8SdX-WPsIs/view?usp=sharing

----------X----------X----------

