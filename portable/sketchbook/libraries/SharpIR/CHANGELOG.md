# Change Log  
All notable changes to this project will be documented in this file.  

The format is based on [Keep a Changelog](http://keepachangelog.com/)  
and this project adheres to [Semantic Versioning](http://semver.org/).  

[![Keep a Changelog specification badge](https://img.shields.io/badge/Keep%20a%20Changelog%20Specification-1.0.0-orange.svg)](http://keepachangelog.com)
[![Semantic Versioning specification badge](https://img.shields.io/badge/Semantic%20Versioning%20Specification-2.0.0-orange.svg)](http://semver.org)

## [2.0.1] - 2018-07-20 ##
## Fixed ##

- Keyword highlighting for Arduino IDE

## [2.0.0] - 2018-04-07 ##
### Changed ###
- Now sensor models have to be specified in the format SharpIR::model , this allow to generate a compile error if an invalid model is specified  
- Updated library properties  

### Fixed ##
- A typo for the GP2Y0A41SK0F sensor model  
- Fixed compile error for ESP8266 and potentially other platforms  

## [1.1.0] - 2017-03-14 ##
### Added  ###
- Support for burst reads  

### Changed ###
- This changelog format is now based on [Keep a Changelog](http://keepachangelog.com/)  
- Rewrote the changelog to accomplish what about the previous point  
- Renamed the file of the changelog for the same purpose  
- Updated README.md  

## [1.0.3] - 2017-01-02 ##
### Changed ###
- Fixed a warning about the **getDistance()** method  

## [1.0.2] - 2016-09-11 ##
### Changed ###
- Reduced memory use  

### Fixed ###
- (*Issue #1*) A bug that prevented the creation of the library instances  
- The keyword highlight in the Arduino IDE  

## [1.0.1] - 2016-08-25 - [YANKED] ##
### Changed ###
- Reduced memory use
- Minor grammatical corrections in **"examples/getDistance.ino"**  
- Changed the repository URL in **"library.properties"**  
- Completely rewrote **README.md**  

## [1.0.0] - 2016-08-25 ##
### Added ###
- Unreleased library files  
