# About
Time wheel implementation with circular linked list. For personal purpose.  
You should call timewheel_work() frequently as your precision interval needed.

# How to build
> mkdir build && cd build<br> 
> cmake ..<br> 
> make

# Execute
> ./twtest

# Expected Output
```
    [3452:807695] 300ms Interval log  
    [3453:107695] 300ms Interval log  
    [3453:407695] 300ms Interval log  
    [3453:507692] 1s Interval log  
    [3453:707695] 300ms Interval log
    [3454:  7695] 300ms Interval log
    [3454:307695] 300ms Interval log
    [3454:507692] 1s Interval log
    [3454:607695] 300ms Interval log
    [3454:907695] 300ms Interval log
    [3455:207695] 300ms Interval log
    [3455:507692] 1s Interval log
    [3455:507714] 300ms Interval log
    [3455:807695] 300ms Interval log
    [3456:107695] 300ms Interval log
    [3456:407695] 300ms Interval log
```