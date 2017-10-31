# AutoJudge

A Windows-based local judge system. Currently, it support C++ only.
### Before we start
If this documentation is hard to read or there is any issue, please contact me or use "Issues" page to report.

# How To Use


## Prepare


You need to prepare the following files to use this program:
* Some test cases pair, each of them contains one input file and one output file, these two filename should be the same.
* Code you are going to judge.
* "main" file if the code you want to judge is a header(.h) file.
* (Optional) config.ini


Then put them as the following:


```
A_FOLDER\  
 
├─AutoJudge.exe  
├─config.ini (auto regenerate if it's missing)  
├─FOLDER_OF_FIRST_PROBLEM\  
│ ├─input\  
│ │ └─(all of your input file)   
│ ├─output\
│ │ └─(all of your output file)   
│ ├─(config.ini)  
│ ├─("main" file if need)  
│ └─(all codes you need to judge)  
├─FOLDER_OF_SECOND_PROBLEM\  
│ └─......
└─......
```


## Setup
Here list the meaning of each option, each option can be overwritten by put a `config.ini` to your problem folder.


* [GENERAL]


	| key | value |
	|-|-|
	| `mode` | `0` = submit code cantains "main", `1` = submit code cantains class(es) only |
	| `cleanup` | `0` = don't delete submit file, `1` = delete submit file after judge | 


* [CODE]


	| key | value |
	|-|-|
	| `coderegex` | define your submit filename format (if needed) |


* [HEADER] (only work on mode=1)


	| key | value |
	|-|-|
	| `headerregex` | define your submit filename format (if needed) |
	| `main` | define "main" filename you provide |
	| `include` | the header file you `#include` in "main" you provide |


* [COMPILE]


	| key | value |
	|-|-|
	| `path` | the directory of g++ compiler |
	| `options` | other compile options you need (like optimization or warning level) |


* [LIMIT]


	| key | value |
	|-|-|
	| `time` | time limit of execution (ms) |
	| `memory` | RAM limit (MB) |
  
* [SCORE]


	| key | value |
	|-|-|
	| `part-score` | `1` = judge result will show how many test case passed instead of just AC/WA |
	| `allowPE` | `1` = consider "Presentation Error" as "Answer Correct" |
  
* [SEGMENT]  
	More detail on [Advanced Use](#segment-feature).
  
	| key | value |
	|-|-|
	| `segment-score` | `1` = every single file contains more then one testcases (part-score must be `1`) |
	| `token` | the token string between test cases |


## Start judge and get result


Run AutoJudge.exe and all result will show on a cmd window (and that means you can redirect it to file or somewhere if needed), the meaning of result shows as the following table:


* Accepted Output (green)


	| result | meaning |
	|-|-|
	| Answer Correct | the outputs same as the correctly answer |
	| Answer Correct(PE) | the outputs are correct but has some presentation problem and you don't care (when allowPE=1) |


* Error Output (red)


	| result | meaning |
	|-|-|
	| Answer Correct X/Y | only X out of Y outputs are correct (when partscore=1) |
	| Presentation Error | the outputs are correct but has some presentation problem |
	| Wrong Answer | all outputs are wrong (when partscore=1) or at least one line of outputs is wrong (when partscore=0) |
	| Compiltion Error | encounter some error during compiltion |
	| Time Limit Exceeded | process use too much time to finish |
	| Runtime Error | process dead during excuction |
	| Not Judged | there is no testcase to judge or something wrong (that means a BUG, please contact author) |
	| Something Wrong | technically speaking it's impossible appaer, please contact author |


# Advanced use


## Segment feature
For some reason, your test case maybe can't be spared into multiple file and you still needed part-score feature. Seeing the following example:

### Sample Output:

    123
    321
    12345
    ==========
    aaaaa
    bbbbb
    ccccc
    ==========
    hello world
   
In this case, the output has 3 parts: numbers, some character, and "hello world". If you need to seen them as 3 "test cases" ,set `segment-score = 1` and `token = ==========` (yup, you can use equal sign), and the following user outputs should be:

### Output1: Answer Corrected 2/3

    123
    321
    12345
    ==========
    aaaaa
    aaaaa
    ccccc
    ==========
    hello world
  
### Output2: Wrong Answer

    123
    321
    12345
    aaaaa
    aaaaa
    ccccc
    =========
    hello world
   **Note:** In this case, user only output 1 segment ( there only 9 `=` instead of 10)
