# vu-isi-vector (```Vector```)
The purpose of this project is to create a class that resembles ```std::vector``` as closely as possible. 

## ```Vector``` function examples
### ```operator>```
![example_1](/images/example_1_more_code.png)<br/>
Prints out "v1 more than v2"
### ```operator==```
![example_2_1](/images/example_2_1.png)<br/>
"v1 equals v2"<br/><br/>
![example_2_2](/images/example_2_2.png)<br/>
"v1 doesn't equal v2"
### ```print()```
![example_3_1](/images/example_3_1.png)<br/>
![example_3_2](/images/example_3_2.png)
### ```insert()```
![example_4_3_1](/images/example_4_3_1.png)<br/>
![example_4_3_2](/images/example_4_3_2.png)
### ```swap()```
![example_5_1_1](/images/example_5_1_1.png)<br/>
![example_5_1_2](/images/example_5_1_2.png)


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
