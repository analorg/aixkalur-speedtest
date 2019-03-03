Challenge result
---

Just for fun created solution more than one language to see witch is quickest and if usal c/c++ fast file read is a real deal. And as i also got best results with C.

As it is ment to work with Estonian words in dictionary, program expects system LC_ALL value to be et_EE.UTF-8. 

Compiling
--
gcc -Wall main.c -o bin/c_anagramm

Running
--
./bin/c_anagramm lemmad.txt kirsa

./bin/python_anagramm/main lemmad.txt kirsa

./bash_anagramm lemmad.txt kirsa
