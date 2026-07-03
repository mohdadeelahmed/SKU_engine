#include <iostream>
#include <string>
#include <sstream> //stringstream library
#include <unordered_map> //mapping sku dictionary
#include <algorithm> //std::transform
#include <cctype> //::toLower
#include <fstream> //file stream to read data from file on computer
#include <vector> //to manage dynamic lists with varying sizes (variants of products)

// Structural mapping for variant combinations
struct ProductVariant {
    std::string size;
    std::string colorName;
    std::string colorCode; // ex. BLK
    std::string sku; // ex. ABY-0028-BLK-52
    std::string imagesrc;
    std::string variantImage;
    int inventoryQty;
};

struct ShopifyProduct {
    std::string handle;
    std::string title;
    std::string category;
    std::string designId;
    std::string price;
    std::string fabric;
    std::vector <ProductVariant> variants;
};


std::unordered_map<std::string, std::string> categories = {
    {"jacket", "JKT"}, {"t-shirt", "TSH"}, {"dress", "DRS"}, {"pants", "PNT"}
};

std::unordered_map<std::string, std::string> colors = {
    {"blue", "BLU"}, {"black", "BLK"}, {"red", "RED"}, {"white", "WHT"}
};

// Getter for serial number (skuBase)
int getNextSerialNumber(std::string skuBase) {

    // Reading csv file (choose name)
    std::ifstream file("inventory.csv");

    // If file DNE, then return 1st number (1st product)
    if (!file.is_open()) { 
    return 1;
    }

    std::string line;
    int count = 0;

    // Read file line by line
    while (std::getline(file, line)) {

        // Check if the sku exists in the current line (npos = no position)
        if (line.find(skuBase) != std::string::npos) {
            count++;
        }
    }

    file.close();
    return count + 1;
}

int main() {

    std::cout << "Enter clothing description: ";

    std::string description;
    std::getline(std::cin, description);

    // Object textStream reads description byte by byte
    std::stringstream textStream(description);
    std::string word;

    // Set placeholder initializations
    std::string skuCategory = "GEN";
    std::string skuColor = "XXX";

    // Loops through textStream word by word
    while (textStream >> word) {
        
        // Transforming every char from upper to lower case
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);

        // Looks for category and color words in textStream
        if (categories.count(word)) { //.count() function to check if category exists once, if not skip to color
            skuCategory = categories[word];
        }
        else if (colors.count(word)) {
            skuColor = colors[word];
        }
    }

    // GENERATE SKU
    std::string skuBase = skuCategory + "-" + skuColor;
    int serialNumber = getNextSerialNumber(skuBase);
    std::string finalSKU = skuBase + "-" + std::to_string(serialNumber);
    
    std::cout << "\nGenerated SKU: " << finalSKU << std::endl;

    // APPEND MODE (ios::app): Append the SKU onto csv file
    std::ofstream outFile("inventory.csv", std::ios::app);

    if (outFile.is_open()) {
        // Write: SKU, Description, and end with a blank line
        outFile << finalSKU << "," << description << "\n";
        outFile.close();

        std::cout << "Successfully saved to inventory.csv!" << std::endl;
    } else {
        std::cout << "Error: Could not open inventory.csv for writing." << std::endl;
    }

    return 0;
}