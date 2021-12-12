# Project Title

SnowDash

## Description

A dashboard of snow data for winter resorts displayed on the Seeed Wio Terminal. 
Current, historical and forecast data for the dashboard is collected from OnTheSnow's open API using a resorts unique ID. Multiple resorts can be toggled through using the built-in buttons on the Wio Terminal.


## Getting Started

### Dependencies

* Seeed Wio Terminal or any arduino-based microcontroller with a screen
* Continued stability of OnTheSnow API. A private API with no documentation and no guarantee of functionality. Tread respectfully. 


### Installing

* Install all libraries
* Include the Free_Fonts.h file in the same directory
* Update the WiFi login details to your network
* Change the resort IDs to one's relevant to you. (Reference the OnTheSnowResortIDs file.)


## Help

Recently, the HTTPClient library has been frequently suffering from 'read Timeouts'. When encountering this error the program reboots the Wio Terminal. This loop continues until data has been received. Rebooting prevents the selection of other resorts as it returns the Wio Terminal to the base state. 


The accuracy of the API data is unknown. Also, the timeliness of the data is questionable. Use and reference with a grain of salt. 


## Authors

Will Dolezal
[willdolezal.com](https://willdolezal.com)
[@willdolezal](https://twitter.com/willdolezal)  


## Version History


* 0.1
    * Initial Release

## Acknowledgments

Inspiration, code snippets, etc.
* [OnTheSnow API Wrapper](https://github.com/rymai/on_the_snow)
* [PNW Cams](https://github.com/brainbrian/pnwcams)
* [Snow Report](https://github.com/tyhilde/AlexaSkillSkiResort)
* [README-Template](https://gist.github.com/DomPizzie/7a5ff55ffa9081f2de27c315f5018afc)