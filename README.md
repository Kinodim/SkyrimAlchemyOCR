# SkyrimAlchemyOCR

**WARNING:** This is an experimental project!

It started out of interest whether I could use OpenCV in combination with Tesseract to read and process Skyrim screenshots or phone pictures in order to generate possible recipes

### Dependencies
* [OpenCV](http://opencv.org/)
* [Tesseract](https://github.com/tesseract-ocr/tesseract)

### General Overview
Instead of calculating possible recipes based on the ingredient effects after the OCR step, this project precompiles possible recipes together with their required ingredients. This way the calculation only needs to be performed once and the module that later scans the ingredients only has to search for recipes that match the inventory.

### Language
As I ran the game in German while developing this project I use a dictionary to translate the ingredients into English, so you would need to change the code accordingly to just skip this step or use another dictionary for your language.

### Used third-party code
* [JSON for Modern C++](https://github.com/nlohmann/json)

### License
See [Licence](LICENSE)
