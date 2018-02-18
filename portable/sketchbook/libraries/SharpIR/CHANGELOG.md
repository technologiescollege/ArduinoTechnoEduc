# Change Log  
All notable changes to this project will be documented in this file.  
  
The format is based on [Keep a Changelog](http://keepachangelog.com/)  
and this project adheres to [Semantic Versioning](http://semver.org/).  
  
## [UNRELEASED]  
- Nothing unreleased

## [1.1.0] - 2017-03-14  
### Added  
- Support for burst reads  

### Changed  
- This changelog format is now based on [Keep a Changelog](http://keepachangelog.com/)  
- Rewrote the changelog to accomplish what about the previous point  
- Renamed the file of the changelog for the same purpose
- Updated README.md  
  
## [1.0.3] - 2017-01-02  
### Changed  
- Fixed a warning about the **getDistance()** method    
- Converted the **#define** statemets in constants  
- Replaced the **#define guards** with a **#pragma once** statement on the top of the header file  
  
## [1.0.2] - 2016-09-11  
### Added  
- Added a warning message about the bug that brokes the release v1.0.1 in the changelog  

### Changed  
- Changed the #define statements in **"src/SharpIR.h"** to reduce memory use  
- Fixed a sintax error in **"examples/getDistance/getDistance.ino"** according to the change to the **#define** statements cited above  
- Fixed the identation in **"keywords.txt"**: it breaks the keyword highlight function of the IDE for some keywords   

### Fixed
- (*Issue #1*) Fixed a bug that prevented the creation of the library instances 
	because of the change of the data type of the **#define** statements in the v1.0.1 release  

## [1.0.1] - 2016-08-25 - [YANKED]
### Changed  
- Changed some variable's type in **"src/SharpIR.h"** to reduce memory use  
- Changed the #define statements in **"src/SharpIR.h"** to reduce memory use  
- Minor grammatical corrections in **"examples/getDistance.ino"**  
- Changed the repository URL in **"library.properties"**  
- Completely rewrote **README.md**    

## [1.0.0] - 2016-08-25  
### Added  
- Unreleased library files