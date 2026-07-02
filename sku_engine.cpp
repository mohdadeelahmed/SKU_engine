#include <iostream>
#include <string>
#include <sstream> //stringstream library
#include <unordered_map> //mapping sku dictionary


std::unordered_map<std::string, std::string> categories = {
    {"jacket", "JKT"}, {"t-shirt", "TSH"}, {"dress", "DRS"}, {"pants", "PNT"}
};

std::unordered_map<std::string, std::string> colors = {
    {"blue", "BLU"}, {"black", "BLK"}, {"red", "RED"}, {"white", "WHT"}
};


int main() {

    std::cout << "Enter clothing description: "; //output

    std::string description; //initialize description
    std::getline(std::cin, description); //gets input (std::cin)

    std::stringstream textStream(description); //object textStream reads description byte by byte
    std::string word; //initialize word string

    std::string skuCategory = "GEN"; //GEN = general
    std::string skuColor = "XXX";    //XXX = color (unknown)

    while (textStream >> word) { //loops through textStream word by word
        
        //look for category and color words in textStream
        if (categories.count(word)) { //.count() function to check if category exists once, if not skip to color
            skuCategory = categories[word];
        }
        else if (colors.count(word)) {
            skuColor = colors[word];
        }
    }

    std::string finalSKU = skuCategory + "-" + skuColor;
    std::cout << "Generated SKU: " << finalSKU << std::endl;

    return 0;
}