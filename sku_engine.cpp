#include <iostream>
#include <string>
#include <sstream> //stringstream library
#include <unordered_map> //mapping sku dictionary
#include <algorithm> //std::transform
#include <cctype> //::toLower
#include <fstream> //file stream to read data from file on computer
#include <vector> //to manage dynamic lists with varying sizes (variants of products)
#include <iomanip> // for zero-padding (ex. 0028)

// Structural mapping for variant combinations
struct ProductVariant {
    std::string size;
    std::string colorName;
    std::string colorCode; // ex. BLK
    std::string sku; // ex. ABY-0028-BLK-52
    std::string imageSrc;
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

// Category profiles
std::unordered_map<std::string, std::string> categories = {
    {"jacket", "JKT"}, {"t-shirt", "TSH"}, {"dress", "DRS"}, {"pants", "PNT"}
};

// Color profiles
std::unordered_map<std::string, std::string> colorCodes = {
    {"charcoal black", "BLK"}, {"stone grey", "GRY"}, {"sahara gold", "GLD"},
    {"velvet cocoa", "CCA"}, {"muted matcha", "MCH"}, {"sky blue", "BLU"},
    {"green", "GRN"}
};

// Sizing profiles
std::vector<std::string> standardSizes = {"52", "54", "56", "58", "60"};

std::string getNextDesignID() {
    std::ifstream file("shopify_import.csv");
    int highestID = 0;

    if (!file.is_open()) { 
    return "0001"; // Base ID
    }

    std::string line;

    while (std::getline(file, line)) {

        size_t firstDash = line.find('-');
        if (firstDash != std::string::npos && firstDash + 5 <= line.length()) {
            std::string idStr = line.substr(firstDash + 1, 4);

            if (std::all_of(idStr.begin(), idStr.end(), ::isdigit)) {
                int currentID = std::stoi(idStr);
                if (currentID > highestID) {
                    highestID = currentID;
                }
        }
    }
}
    file.close();

    // Increment counter and zero-pad to four chars
    int nextID = highestID + 1;
    std::stringstream ss;
    ss << std::setw(4) << std::setfill('0') << nextID;
    return ss.str();

}

int main() {

    ShopifyProduct product;

    // Collect SKU fields
    std::cout << "=== SHOPIFY & AMAZON LISTING GENERATOR ===" << std::endl;
    std::cout << "Enter Product Title: ";
    std::getline(std::cin, product.title);

    std::cout << "Enter Category Abbreviation (e.g., MK, ABY): ";
    std::getline(std::cin, product.category);
    std::transform(product.category.begin(), product.category.end(), product.category.begin(), ::toupper);

    std::cout << "Enter Price: ";
    std::getline(std::cin, product.price);

    std::cout << "Enter Fabric Type: ";
    std::getline(std::cin, product.fabric);

    std::string colorsInput;
    std::cout << "Enter Colors (comma-separated, e.g., Charcoal Black, Stone Grey): ";
    std::getline(std::cin, colorsInput);

    // URL Handle Generation
    product.handle = product.title;
    std::transform(product.handle.begin(), product.handle.end(), product.handle.begin(), ::tolower);
    std::replace(product.handle.begin(), product.handle.end(), ' ', '-');

    // Design ID
    product.designId = getNextDesignID();

    // Parsing user input colors
    std::vector<std::string> individualColors;
    std::stringstream colorStream(colorsInput);
    std::string rawColor;

    while (std::getline(colorStream, rawColor, ',')) {
        // Trim whitespaces
        if (!rawColor.empty() && rawColor[0] == ' ') {
            rawColor = rawColor.substr(1);
        }
        if (!rawColor.empty()) {
            individualColors.push_back(rawColor);
        }
    }

    //
    for (const std::string& size : standardSizes) {
        for (const std::string& color : individualColors) {
            ProductVariant variant;
            variant.size = size;
            variant.colorName = color;
            
            // Convert color to 3-letter abbreviation
            std::string lowerColor = color;
            std::transform(lowerColor.begin(), lowerColor.end(), lowerColor.begin(), ::tolower);
            
            if (colorCodes.count(lowerColor)) {
                variant.colorCode = colorCodes[lowerColor];
            } else {
                variant.colorCode = "XXX";
            }
            
            // Final Variant SKU
            variant.sku = product.category + "-" + product.designId + "-" + variant.colorCode + "-" + variant.size;
            
            // Shopify Image Link
            variant.imageSrc = "https://cdn.shopify.com/files/" + product.handle + "_" + variant.colorCode + ".jpg";
            
            product.variants.push_back(variant);
        }
    }

    // Export Shopify CSV
    bool fileExists = std::ifstream("shopify_import.csv").good();
    std::ofstream outFile("shopify_import.csv", std::ios::app);

    if (!outFile.is_open()) {
        std::cout << "Error: Master database file locked or inaccessible." << std::endl;
        return 1;
    }

    // Shopify headings for creating new file
    if (!fileExists) {
        outFile << "Handle,Title,Body (HTML),Vendor,Option1 Name,Option1 Value,Option2 Name,Option2 Value,Variant SKU,Variant Inventory Qty,Variant Price,Status\n";
    }

    return 0;
}