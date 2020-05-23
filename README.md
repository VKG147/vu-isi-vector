# vu-isi-vector (```Vector```)
The purpose of this project is to create a class that resembles ```std::vector``` as closely as possible. 

## ```Vector``` function examples

## ```Vector``` vs ```std::vector```

### ```push_back()``` test
![push_back() test results](/images/pushback_test_excel.png)<br/>
Unexpected behaviour ```Vector``` performs almost 3x faster than ```std::vector```?

### [vu-isi-student-results](https://github.com/VKG147/vu-isi-student-results-new) test
#### Memory reallocation comparison
In this context a memory reallocation occurs whenever the condition ```capacity() == size()``` is true.

#### General program test
![vu-isi-student-results tests](/images/100k_excel.png)<br/>
It appears that ```Vector``` performs slightly better than ```std::vector```. Biggest improvement to data seperation.
