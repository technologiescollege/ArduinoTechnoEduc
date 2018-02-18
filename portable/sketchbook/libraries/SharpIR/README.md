# SharpIR
An Arduino library that allows to acquire distance data from a Sharp analog distance sensor

## Currently supported MCU architectures

- ALL

## Currently supported sensor models

- GP2YA41SK0F  
- GP2Y0A21YK0F  
- GP2Y0A02YK0F  

## Usage

- Adding the library to the sketch

~~~c++
#include <SharpIR.h>
~~~

- Creating an istance  

~~~c++
SharpIR sensorName(sensorModel, sensorPin);
~~~
~~~
sensorName	: the name of the object   
sensorModel	: the model code of the sensor (e.g. GP2YA41SK0F)  
sensorPin	: the analog pin where the sensor Vout pin is attached  
~~~    

- Acquiring data

~~~c++
sensorName.getDistance();
sensorName.getDistance( avoidBurstRead );
~~~
~~~
avoidBurstRead	: can be true or false
~~~

The above method returns an uint8_t type value that is the distance in centimeters from the sensor and the object in front of it.  
By default burst reads are avoided but this causes a delay of 20ms on every call.
In order to speed up the sketch is possible to allow bust reads by setting the parameter **avoidBurstRead** to **false**, in this way:

~~~c++
sensorName.getDistance( false );
~~~

## Getting started

### Installation
#### via the Arduino Library Manager

1. Open the Arduino IDE
2. Go to Sketch > Include Library > Manage Libraries
3. Search **SharpIR**
4. Click on **"SharpIR** by **Giuseppe Masino (HackerInside)"**
5. Select the latest version and then click on **install**
6. When the download is complete restart the IDE

![How to open the library manager](https://github.com/HackerInside0/support_repo/blob/master/generic/ArduinoLibraryManager.png  "Go to Sketch > Include Library > Manage Libraries")

![How to download from the library manager](https://github.com/HackerInside0/support_repo/blob/master/Arduino_SharpIR/LibraryManagerTutorial.png  "Click on **SharpIR** by **Giuseppe Masino (HackerInside)**")

#### via GitHub ( manual installation )

1. Download the library from GitHub
2. Copy the zip file in the arduino library folder
3. If already open, restart the IDE

![How to download from github](https://github.com/HackerInside0/support_repo/blob/master/Arduino_SharpIR/downloadFromGitHub.png "Click on **SharpIR** by **Giuseppe Masino (HackerInside)**")

### Where to find help?

First be sure to read the FAQs in this repository ( if present ) and/or the [generic FAQs](https://www.facebook.com/notes/giuseppe-masino/faqs-about-my-works/197854657355058).

Use the GitHub issue tracker for techincal troubles about the implementation and to report bugs, for other requests there is [THIS PAGE](http://www.facebook.com/dev.hackerinside/).

## LICENSE

<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/">
<img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-sa/4.0/88x31.png" />
</a>
<br />
<span xmlns:dct="http://purl.org/dc/terms/" property="dct:title">The SharpIR Library</span> by <a xmlns:cc="http://creativecommons.org/ns#" href="https://github.com/HackerInside0/" property="cc:attributionName" rel="cc:attributionURL">Giuseppe Masino ( HackerInside0 )</a> is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/">Creative Commons Attribution-ShareAlike 4.0 International License</a> and also under a custom MIT license.

The updated terms of the license can be found [HERE]( https://www.facebook.com/notes/giuseppe-masino/faqs-about-my-works/197854657355058) and shall be available in the LICENSE.md 

If you need permissions that are beyond the scope of this license, you can ask to me through <a xmlns:cc="http://creativecommons.org/ns#" href="https://www.facebook.com/dev.hackerinside/" rel="cc:morePermissions">THIS PAGE</a>