#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;



class Product {
private:
    string name;
    int price;

public:
    Product() : name("none"), price(0) {}

    Product(string na, int p) : name(na), price(p) {}

    void set_name(string na) { name = na; }
    void set_price(int p) { price = p; }

    string get_name() { return name; }
    int get_price() { return price; }

    // Virtual display function to be overridden by child classes if any
    virtual void display() {
        cout << "Name: " << name  << endl;
    }
};


class User {
private:
    string name;
    string address;
    string phone;

public:
    User() : name("Unknown"), address("null"), phone("0000") {}

    User(string n, string addr, string ph) : name(n), address(addr), phone(ph) {}

    void set_name(string newName) { name = newName; }
    void set_address(string newAddress) { address = newAddress; }
    void set_phone(string newPhone) { phone = newPhone; }

    string get_name() { return name; }
    string get_address() { return address; }
    string get_phone() { return phone; }

    void display() {
        cout << "Customer: " << name << ", Address: " << address << ", Phone: " << phone << endl;
    }
};


class Order {
private:
    // Customer Information
    string customerName;
    string customerAddress;
    string customerPhone;

    // Order item data
    vector<string> rawOrderData; // Formerly 'groub'
    vector<string> itemNames;
    vector<int> itemPrices;
    vector<int> itemQuantities;

public:
    Order() {} // Default constructor

    // Method to set customer data for the order
    void setCustomerInfo(string name, string address, string phone) {
        customerName = name;
        customerAddress = address;
        customerPhone = phone;
    }

    // Method to receive the raw list of selected items
    void set_order_items(const vector<string>& items) {
        rawOrderData = items;
    }


    void transfer_data() {
        // Clear previous data to avoid duplicates on multiple calls
        itemNames.clear();
        itemPrices.clear();
        itemQuantities.clear();

        for (int i = 0; i < rawOrderData.size(); i++) {
            string current_item_data = rawOrderData[i];
            string temp_name, price_str, quantity_str;
            size_t pos1 = current_item_data.find(" & ");
            size_t pos2 = current_item_data.rfind(" & ");

            if (pos1 != string::npos && pos2 != string::npos && pos1 != pos2) {
                temp_name = current_item_data.substr(0, pos1);
                price_str = current_item_data.substr(pos1 + 3, pos2 - (pos1 + 3));
                quantity_str = current_item_data.substr(pos2 + 3);

                itemNames.push_back(temp_name);
                itemPrices.push_back(stoi(price_str));
                itemQuantities.push_back(stoi(quantity_str));
            }
        }
    }


    int get_total() {
        int total = 0;
        for (int i = 0; i < itemPrices.size(); i++) {
            if (itemQuantities[i] >= 20) {
                // 5% discount
                total += (itemPrices[i] - (itemPrices[i] * 0.05)) * itemQuantities[i];
            } else if (itemQuantities[i] >= 15) {
                // 4% discount
                total += (itemPrices[i] - (itemPrices[i] * 0.04)) * itemQuantities[i];
            } else if (itemQuantities[i] >= 10) {
                // 2% discount
                total += (itemPrices[i] - (itemPrices[i] * 0.02)) * itemQuantities[i];
            } else {
                total += itemPrices[i] * itemQuantities[i];
            }
        }
        return total;
    }


    void print_invoice() {
        // First, parse the raw data into structured vectors
        transfer_data();

        cout << endl << endl << "          Final Invoice" << endl;
        cout << "------------------------------------------------------------------------------------" << endl;
        // Print customer info
        cout << "Customer Name: " << customerName << endl;
        cout << "Delivery Address: " << customerAddress << endl;
        cout << "Phone Number: " << customerPhone << endl;
        cout << "------------------------------------------------------------------------------------" << endl;

        for (int i = 0; i < itemNames.size(); i++) {
            cout << "Item Name >  " << itemNames[i]
                 << ", Price = " << itemPrices[i]
                 << ", Quantity = " << itemQuantities[i] << endl;
        }

        cout << endl << "                          Total Invoice : " << get_total() << " EGP" << endl;
        cout << "------------------------------------------------------------------------------------" << endl;
    }
};


class Memory {
public:
    // Prints the current menu from menu.txt
    static void print_menu() {
        ifstream menu("menu.txt");
        // Check if the file was opened successfully
        if (!menu.is_open()) {
            cerr << "Error: Could not open menu.txt. The file might be missing." << endl;
            return;
        }

        string model, name, price;
        int line_num = 1;

        while (getline(menu, model)) {
            size_t delimiter_pos = model.find(" & ");
            if (delimiter_pos != string::npos) {
                name = model.substr(0, delimiter_pos);
                price = model.substr(delimiter_pos + 3);
                cout << line_num++ << ". " << name << " ---> " << price << endl;
            }
        }
        menu.close();
    }

    // Removes a product from the menu based on its line number
    static void remove_product(int line_to_remove) {
        ifstream old_file("menu.txt");
        if (!old_file.is_open()) {
            cerr << "Error: menu.txt not found." << endl;
            return;
        }
        ofstream new_file("tempmenu.txt");

        string model;
        for (int i = 1; getline(old_file, model); i++) {
            if (i != line_to_remove)
                new_file << model << endl;
        }

        old_file.close();
        new_file.close();
        remove("menu.txt");
        rename("tempmenu.txt", "menu.txt");
    }

    // Modifies a product in the menu
    static void set_adjustment(int line_to_adjust) {
        ifstream old_file("menu.txt");
        if (!old_file.is_open()) {
             cerr << "Error: menu.txt not found." << endl;
            return;
        }
        ofstream new_file("tempmenu.txt");

        string model;
        for (int i = 1; getline(old_file, model); i++) {
            if (i != line_to_adjust) {
                new_file << model << endl;
            } else {
                string new_name;
                int new_price;
                cout << "Enter new item name: "; cin >> new_name;
                cout << "Enter new item price: "; cin >> new_price;
                new_file << new_name << " & " << new_price << endl;
            }
        }

        old_file.close();
        new_file.close();
        remove("menu.txt");
        rename("tempmenu.txt", "menu.txt");
    }

    // Adds a new product to the end of the menu file
    static void add_product(Product a) {
        ofstream myfile("menu.txt", ios::app); // Open in append mode
        if (!myfile.is_open()) {
            cerr << "Error: Could not open menu.txt to add product." << endl;
            return;
        }
        myfile << a.get_name() << " & " << a.get_price() << endl;
        myfile.close();
    }

    // Gets a specific line from the menu file
    static string extract_line_from_menu(int line_number) {
        ifstream a("menu.txt");
        if (!a.is_open()) return ""; // Return empty if file not found

        string model;
        for (int i = 1; getline(a, model); i++) {
            if (line_number == i) {
                a.close();
                return model;
            }
        }
        a.close();
        return ""; // Return empty if line not found
    }

    // Counts the total number of lines (items) in the menu file
    static int number_of_file_lines() {
        ifstream file("menu.txt");
        if (!file.is_open()) return 0;

        int lineCount = 0;
        string line;
        while (getline(file, line)) {
            lineCount++;
        }
        file.close();
        return lineCount;
    }
};


class Print {
public:
    static void print_main_options() {
        cout << " -----------------------------------------------------------------------------------------------------------" << endl;
        cout << "|   Option 1: Admin Panel   |   Option 2: View Menu & Place Order   |   Option 3: View Discount System      |" << endl;
        cout << " -----------------------------------------------------------------------------------------------------------" << endl;
    }

    static void print_continue_or_invoice() {
        cout << " -------------------------------------------------------------------------------------------------" << endl;
        cout << "|   To finalize your purchase and print the invoice, press 0    |   To continue shopping, press 1   |" << endl;
        cout << " -------------------------------------------------------------------------------------------------" << endl;
    }

    static void print_exit_or_main_menu() {
        cout << " ------------------------------------------------------------------------------------" << endl;
        cout << "|   To end the program, press 0     |   To return to the main menu, press 1     |" << endl;
        cout << " ------------------------------------------------------------------------------------" << endl;
    }

    static void print_admin_options() {
        cout << " -----------------------------------------------------------------------------------------------------------" << endl;
        cout << "|   Option 1: Add a product     |   Option 2: Modify a product price/name   |   Option 3: Delete a product      |" << endl;
        cout << " -----------------------------------------------------------------------------------------------------------" << endl;
    }
};


int validation(int min_val, int max_val) {
    int option;
    cout << "Please enter your choice: ";
    cin >> option;
    while (cin.fail() || option < min_val || option > max_val) {
        cin.clear(); // clear input buffer to restore cin to a usable state
        cin.ignore(10000, '\n'); // ignore last input
        cout << "Error: Please enter a number between " << min_val << " and " << max_val << ": ";
        cin >> option;
    }
    return option;
}

int main() {
    int continue_program_choice = 0;
    do {
        Print::print_main_options();
        int main_option = validation(1, 3);

        // Admin Panel
        if (main_option == 1) {
            int password = 758595, entered_password;
            cout << "Enter the administrator password: ";
            cin >> entered_password;
            if (entered_password != password) {
                for (int i = 0; i < 2; i++) {
                    cout << "Incorrect password. " << 2 - i << " attempts remaining: ";
                    cin >> entered_password;
                    if (entered_password == password) break;
                }
            }

            if (entered_password == password) {
                Print::print_admin_options();
                int admin_option = validation(1, 3);

                // Admin: Add a product
                if (admin_option == 1) {
                    int item_count;
                    cout << "How many items do you want to add? : ";
                    cin >> item_count;
                    for (int i = 0; i < item_count; i++) {
                        string name;
                        int price;
                        cout << "Enter item name: "; cin >> name;
                        cout << "Enter item price: "; cin >> price;
                        Product new_product(name, price);
                        Memory::add_product(new_product);
                    }
                }
                // Admin: Modify a product
                else if (admin_option == 2) {
                    Memory::print_menu();
                    cout << "Enter the item number to modify: ";
                    int item_num = validation(1, Memory::number_of_file_lines());
                    Memory::set_adjustment(item_num);
                }
                // Admin: Delete a product
                else if (admin_option == 3) {
                    Memory::print_menu();
                    cout << "Enter the item number to remove: ";
                    int item_num = validation(1, Memory::number_of_file_lines());
                    Memory::remove_product(item_num);
                }
            }
            Print::print_exit_or_main_menu();
            continue_program_choice = validation(0, 1);
        }

        // View Menu & Place Order
        else if (main_option == 2) {
            Memory::print_menu();
            vector<string> chosen_items;
            Order current_order;

            for (;;) {
                cout << "Choose an item number from the menu: ";
                int item_num = validation(1, Memory::number_of_file_lines());

                cout << "Enter the quantity for this item: ";
                int quantity = validation(1, 1000);

                string quantity_str = to_string(quantity);
                chosen_items.push_back(Memory::extract_line_from_menu(item_num) + " & " + quantity_str);

                cout << endl;
                Print::print_continue_or_invoice();
                int continue_choice = validation(0, 1);
                if (continue_choice == 0) {
                    break;
                }
            }

            // Set the collected items for the order
            current_order.set_order_items(chosen_items);

            // Get customer info before printing the invoice
            string cust_name, cust_address, cust_phone;
            cout << "\nPlease enter your details for the invoice." << endl;
            cout << "Enter your name: ";
            cin.ignore(); // To consume the newline character left by cin
            getline(cin, cust_name);
            cout << "Enter your delivery address: ";
            getline(cin, cust_address);
            cout << "Enter your phone number: ";
            getline(cin, cust_phone);

            // Set customer info in the order object
            current_order.setCustomerInfo(cust_name, cust_address, cust_phone);

            // Print the final invoice
            current_order.print_invoice();

            Print::print_exit_or_main_menu();
            continue_program_choice = validation(0, 1);
            if (continue_program_choice == 0) {
                cout << endl << "Thank you and we hope to see you again soon." << endl;
            }
        }

        // View Discount System
        else if (main_option == 3) {
            cout << "|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||" << endl;
            cout << "If you purchase more than 10 items of the same category, a 2% discount will be applied." << endl << endl;
            cout << "If you purchase more than 15 items of the same category, a 4% discount will be applied." << endl << endl;
            cout << "If you purchase more than 20 items of the same type, a 5% discount will be applied." << endl << endl;
            cout << "|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||" << endl;

            Print::print_exit_or_main_menu();
            continue_program_choice = validation(0, 1);
        }

    } while (continue_program_choice != 0);

    return 0;
}
