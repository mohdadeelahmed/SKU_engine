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
    
    return 0;
}