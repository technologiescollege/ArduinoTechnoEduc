Grove Ranging sensor VL53L0X  [![Build Status](https://travis-ci.com/Seeed-Studio/Grove-Ranging-sensor-VL53L0X.svg?branch=master)](https://travis-ci.com/Seeed-Studio/Grove-Ranging-sensor-VL53L0X)
=======================================
  --World’s smallest Time-of-Flight ranging and gesture detection sensor  
---------------------------------------------





Introduction:  
------------------------  
>The VL53L0X is a new generation Time-of-Flight(ToF) laser-ranging module housed in the smallest package on the market today,providing accurate distance measurement whatever the target reflectances unlike conventional technologies.  

***

Usage: 
========== 
>Download all the source files and open one of example,compile,upload,and running.Open the corresponding Serial Port to watch the result.  

Notice:
----------
>Not recommended for use under bright sun light. It will affects the accuracy of infrared.
>The range of measurement:10~2000mm(It is recommended to be within 1 meter in normal mode),further more,It has different performance when measure different color of object.Refer to doc/vl53l0x-datasheet.pdf to get more detail.
>This module supports:
>+ single ranging mode:Perform measurement once when calling the measurement function.
>+ continuous ranging mode:Perform measurement continuous with out interval time when calling the measurement function.
>+ high speed ranging mode : Reduce measurement time.
>+ high accuracy mode :Higher accuracy.but it will costs much more time.
>+ long distance mode :Longer measurement distance.

***
reference:
============
>Software implementation flow:refer to **doc/software-flow.pdf**  
>Datasheet:refer to **doc/vl53l0x-datasheet.pdf**  


***
This software is written by downey  for seeed studio<br>
Email:dao.huang@seeed.cc
and is licensed under [The MIT License](http://opensource.org/licenses/mit-license.php). Check License.txt for more information.<br>

Contributing to this software is warmly welcomed. You can do this basically by<br>
[forking](https://help.github.com/articles/fork-a-repo), committing modifications and then [pulling requests](https://help.github.com/articles/using-pull-requests) (follow the links above<br>
for operating guide). Adding change log and your contact into file header is encouraged.<br>
Thanks for your contribution.

Seeed Studio is an open hardware facilitation company based in Shenzhen, China. <br>
Benefiting from local manufacture power and convenient global logistic system, <br>
we integrate resources to serve new era of innovation. Seeed also works with <br>
global distributors and partners to push open hardware movement.<br>

