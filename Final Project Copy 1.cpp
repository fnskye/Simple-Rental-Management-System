#include <iostream>
#include <string>    // Includes the "String" library
#include <fstream>   //Includes the "File Stream" library
#include <conio.h>   // For _getch() function to hide password input
#include <stdlib.h>  // For system("CLS") to clear console
#include <windows.h> // For SetConsoleOutputCP to support UTF-8
#include <vector>    // For using vectors
#include <sstream>   // For stringstream

using namespace std;

// --- GLOBAL VARIABLES ---
// These variables are accessible from ANY function.
// They hold the current state of our application.
string currentUser;                                     // Stores the username of the person who is logged in.
bool isLoggedIn = false;                                // Is anyone currently logged in? (true/false)
bool isAdmin = false;                                   // Is the logged-in user an administrator? (true/false)
string storeuser, storepass;                            // Store the username and password of the client
string username, password;                              // Store the username and password of the client
string room_num;                                        // Store the room number of the tenant
const string user_filename = "users.txt";               // File to store user credentials
const string transaction_filename = "transactions.txt"; // File to store rental transactions
const string pesoSign = "₱";                            // Peso currency sign

// Calling and Declaring Variables
void login();
void adminMenu();
void userMenu();
void viewTenants();
void registerUser();
void addTransaction();
void recordPayment();
void viewAllTransactions();
void viewTenantTransaction();
void saveAllTransactions();
void logout();
string getdecouplePassword();

struct RentalTransactions // Structure to hold rental transaction details
{
    string username;
    string description;
    double totalAmount;
    double paidAmount;
    bool paymentStatus;
    string paymentDate;
};

vector<RentalTransactions> getAllTransactions() // Function to get all transactions from the file
{
    vector<RentalTransactions> transactions; // Vector to hold all transactions
    ifstream file((transaction_filename));   // Open the file for reading
    string line;
    while (getline(file, line)) // Read each line from the file
    {
        stringstream ss(line); // Use stringstream to parse the line
        RentalTransactions tenants;
        getline(ss, tenants.username, ','); // Read until the comma
        getline(ss, tenants.description, ',');
        ss >> tenants.totalAmount;
        ss.ignore(); // Ignore the comma
        ss >> tenants.paidAmount;
        ss.ignore(); // Ignore the comma
        ss >> tenants.paymentStatus;
        ss.ignore(); // Ignore the comma
        ss >> tenants.paymentDate;
        transactions.push_back(tenants); // Add the transaction to the vector
    }
    file.close();
    return transactions; // Return the vector of transactions
}

int main()
{
    SetConsoleOutputCP(CP_UTF8); // Properly print utf8 characters in windows console

    int choice = 0; // Variable to store user's menu choice
    while (choice != 2)
    {
        if (!isLoggedIn) // If no user is logged in, show the login menu
        {
            Sleep(600);
            system("CLS");
            cout << "\n--- Digital Logs and Receipts for Room Rentals ---\n";
            cout << "1. Login\n";
            cout << "2. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice)
            {
            case 1:
                system("CLS");
                login();
                break;

            case 2:
                cout << "Exiting the system..." << endl;
                break;

            default:
                Sleep(600);
                cout << "Invalid input. Please enter a number." << endl;
                cin.clear();
                cin.ignore(256, '\n');
            }
        }
        else // If a user is logged in, show the appropriate menu
        {
            if (isAdmin) // If the logged-in user is an admin, show the admin menu
            {
                system("CLS");
                adminMenu();
            }
            else // If the logged-in user is a regular tenant, show the user menu
            {
                system("CLS");
                userMenu();
            }
        }
    }
    return 0;
}

void saveAllTransactions(const vector<RentalTransactions> &transactions) // Function to save all transactions back to the file
{
    ofstream file((transaction_filename), ios::trunc); // Open the file in truncate mode to overwrite existing content
    for (const auto &tenants : transactions)           // Iterate through all transactions
    {
        file << tenants.username << ","
             << tenants.description << ","
             << tenants.totalAmount << ","
             << tenants.paidAmount << ","
             << tenants.paymentStatus << ","
             << tenants.paymentDate << endl;
    }
    file.close();
}

string getdecouplePassword() // Function to hide password input
{
    string enteredPassword;                // Variable to store the entered password
    char character;                        // Variable to store each character input
    while ((character = _getch()) != '\r') // Loop until Enter key is pressed
    {
        if (character == '\b') // Handle backspace
        {
            if (!enteredPassword.empty()) // Check if there's something to delete
            {
                cout << "\b \b";
                enteredPassword.pop_back(); // Remove the last character from the password
            }
        }
        else
        {
            enteredPassword += character; // Append the character to the password
            cout << '*';                  // Print asterisk for each character entered
        }
    }
    cout << endl;
    return enteredPassword; // Return the entered password
}

void login() // Function to handle user login
{
    cout << "Enter your Username: ";
    cin >> username;
    cout << "Enter your Password: ";
    password = getdecouplePassword(); // Call function to hide password input

    if (username == "admin" && password == "admin") // Check for admin credentials
    {
        cout << "Admin login successful!\n";
        currentUser = "admin"; // Set the current user to admin
        isLoggedIn = true;
        isAdmin = true; // Set admin status to true for admin user
        Sleep(2000);
        return;
    }

    ifstream file(user_filename);
    string line;

    while (getline(file, line)) // reads the file line by line
    {
        stringstream ss(line); // Use stringstream to parse the line

        if (ss >> storeuser >> storepass) // we need to extract the first two words from the line, the if statement makes sure we successfully read that two words.
        {
            if (storeuser == username && storepass == password) // compare the stored credentials with the input credentials
            {
                cout << "Login Successful! Welcome " << username << endl;
                currentUser = username; // Set the current user to the logged-in username
                isLoggedIn = true;
                isAdmin = false; // Set admin status to false for regular users
                file.close();
                Sleep(2000);
                return;
            }
        }
    }
    file.close(); // It is important to close the file after opening it

    cout << "Invalid username or password." << endl;
    isLoggedIn = false; // Ensure login status remains false
    Sleep(600);
}

void logout() // Function to handle user logout
{
    cout << "Logging out...\n";
    currentUser = "";
    isLoggedIn = false;                               // Set login status to false
    isAdmin = false;                                  // Set admin status to false
    string storeuser, storepass = "";                 // Clear stored credentials
    string username, password, decouplepassword = ""; // Clear stored credentials
}

void adminMenu() // Function to display admin menu and handle admin actions
{
    int choice; // Variable to store admin's menu choice
    int x = 0;  // Counter for invalid attempts
    do          // Loop until admin chooses to logout
    {
        Sleep(600);
        system("CLS");
        cout << "\n--- Administrator Menu ---\n";
        cout << "1. View Tenants\n";
        cout << "2. Add New Tenant\n";
        cout << "3. Add New Rental Charge\n";
        cout << "4. Record a Payment\n";
        cout << "5. View All Transactions\n";
        cout << "6. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cout << endl;

        switch (choice)
        {
        case 1:
            system("CLS");
            viewTenants();
            break;
        case 2:
            system("CLS");
            registerUser();
            break;
        case 3:
            system("CLS");
            addTransaction();
            break;
        case 4:
            system("CLS");
            recordPayment();
            break;
        case 5:
            system("CLS");
            viewAllTransactions();
            break;
        case 6:
            logout();
            break;
        default:
            cout << "Invalid input. Please enter a number." << endl;
            x++; // Increment invalid attempt counter
            cin.clear();
            cin.ignore(256, '\n');
        }

        if (x >= 3) // Check for too many invalid attempts
        {
            Sleep(600);
            cout << endl;
            cout << "Too many invalid attempts. Returning to login screen..." << endl;
            Sleep(1500);
            system("CLS");
            logout(); // Logout the user
            return;
        }
    } while (choice != 6);
}

void userMenu() // Function to display user menu and handle user actions
{
    int choice; // Variable to store user's menu choice
    int x = 0;  // Counter for invalid attempts
    do          // Loop until user chooses to logout
    {
        cout << "\n--- User Menu (Tenant View) ---\n";
        cout << "1. View Transaction History & Balance\n";
        cout << "2. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cout << endl;

        switch (choice)
        {
        case 1:
            system("CLS");
            viewTenantTransaction();
            break;
        case 2:
            logout();
            break;
        default:
            cout << "Invalid input. Please enter a number." << endl;
            x++;
            cin.clear();           // Clear the error flag on cin
            cin.ignore(256, '\n'); // Discard invalid input
        }

        if (x >= 3) // Check for too many invalid attempts
        {
            Sleep(600);
            cout << endl;
            cout << "Too many invalid attempts. Returning to login screen..." << endl;
            Sleep(1500);
            system("CLS");
            logout(); // Logout the user
            return;
        }
    } while (choice != 2);
}

void viewTenants() // Function to view all registered tenants
{
    cout << "\n--- List of All Registered Tenants ---\n";
    ifstream file(user_filename);
    if (!file.is_open())
    {
        cout << "Error: Could not open the user file." << endl;
        Sleep(1500);
        return;
    }

    string username, password; // Temporary variables to hold username and password
    int tenantCount = 0;       // Counter for tenants

    while (file >> username >> password >> room_num) // Read the file word by word (username then password)
    {
        if (username != "admin") // Only check for non-admin users
        {
            tenantCount++;                                                                  // Increment tenant counter
            cout << tenantCount << ". " << username << " --> Room No." << room_num << endl; // Display tenant info
        }
    }
    file.close();

    if (tenantCount == 0) // No tenants found
    {
        cout << "No tenants have been registered yet." << endl;
    }

    cout << "\nPress any key to return to the admin menu..." << endl;
    _getch(); // Pauses the screen until the user presses a key
    cout << "\nExiting View Tenants..." << endl;
    Sleep(600);
    return;
}

void registerUser() // Function to register a new tenant
{
    cout << endl;
    cout << "Enter a username for the new tenant: ";
    cin >> username;

    ifstream file(user_filename); // Open the user file to check for existing usernames
    string line;                  // Temporary variable to hold each line
    bool usernameExists = false;  // Flag to track if username exists
    while (file >> line)          // Read the file word by word
    {
        if (line == username)
        {
            usernameExists = true; // Set flag if username is found
            break;
        }
    }
    file.close();

    if (usernameExists) // Check if username already exists
    {
        cout << "Username already exists. Please select another username." << endl;
        Sleep(600);
    }

    else if (username == "admin") // Prevent 'admin' as a tenant username
    {
        cout << "Tenant cannot be 'admin'. Please select another username." << endl;
        Sleep(1500);
        return;
    }

    else // If username is unique, proceed to register
    {
        cout << "Enter a temporary password: ";
        cin >> password;

        cout << "Enter Room No. (e.g 2F-305): ";
        cin >> room_num; // Get the room number of the tenant
        cout << endl;

        ofstream file((user_filename), ios::app);                       // adds the tenant's username and password in the file and appending it to make sure it is created in a new line
        file << username << " " << password << " " << room_num << endl; // this is the arrangement of how the output of the tenant's credentials arranged
        file.close();                                                   // It is important to close the file after opening/writing on it

        cout << "User " << username << " registered successfully!" << endl;
        Sleep(1500);
    }
}

void addTransaction() // Function to add a new rental charge for a tenant
{
    RentalTransactions tenants; // Create a new transaction object
    cout << "Enter username of the tenants: ";
    cin >> tenants.username;

    vector<RentalTransactions> transactions = getAllTransactions(); // Get all existing transactions
    bool userFound = false;                                         // Flag to track if user is found

    ifstream file(user_filename);
    string line;
    while (file >> line) // Read the file word by word
    {
        if (line == tenants.username) // Check if the username exists
        {
            userFound = true;
            break;
        }
    }
    file.close();

    if (tenants.username == "admin") // Prevent 'admin' as a tenant username
    {
        cout << "Tenant cannot be 'admin'. Please select another username." << endl;
        Sleep(1000);
        return;
    }

    if (!userFound) // If username not found, display error
    {
        cout << "User " << tenants.username << " " << "not found.\n";
        Sleep(1000);
        return;
    }

    if (userFound) // If username found, proceed to add transaction
    {
        cout << "Enter transaction description (e.g., October-Rent): ";
        cin.ignore();
        getline(cin, tenants.description); // Read the full description including spaces

        cout << "Enter Total amount for this charge: " << pesoSign << " ";
        cin >> tenants.totalAmount; // Read the total amount to be paid by tenants

        tenants.paidAmount = 0.0;      // New transactions start with zero paid amount
        tenants.paymentStatus = false; // New transactions are initially unpaid
        tenants.paymentDate = "N/A";   // New transactions have no payment date yet.

        ofstream file((transaction_filename), ios::app); // Open the transaction file in append mode
        file << tenants.username << ","
             << tenants.description << ","
             << tenants.totalAmount << ","
             << tenants.paidAmount << ","
             << tenants.paymentStatus << ","
             << tenants.paymentDate << endl; // Write the new transaction to the file
        file.close();

        cout << "Transaction added successfully!" << endl;
        Sleep(1500);
    }
}

void recordPayment() // Function to record a payment made by a tenant
{
    int prompt;                 // Variable to store user's choice
    RentalTransactions tenants; // Create a transaction object
    string username;
    cout << "Enter Tenant username: ";
    cin >> username;

    vector<RentalTransactions> transactions = getAllTransactions(); // Get all existing transactions
    double totalLiability = 0;                                      // Variable to hold total outstanding balance
    bool userFound = false;                                         // Flag to track if user is found

    for (auto &tenants : transactions) // Iterate through all transactions
    {
        if (tenants.username == username && !tenants.paymentStatus) // Check for matching username and unpaid status
        {
            userFound = true;
            totalLiability += tenants.totalAmount - tenants.paidAmount; // Calculate outstanding balance
        }
    }

    if (username == "admin") // Prevent 'admin' as a tenant username
    {
        cout << "Tenant cannot be 'admin'. Please select another username." << endl;
        Sleep(1000);
        return;
    }

    else if (totalLiability <= 0) // If no outstanding balance, inform the user
    {
        cout << username << " " << "has no outstanding balance.\n";
        Sleep(1000);
        return;
    }

    else if (!userFound) // If user not found, display error
    {
        cout << "User " << username << " " << "not found.\n";
        Sleep(1000);
        return;
    }

    cout << "Outstanding balance for " << username << " " << tenants.description << "is " << pesoSign << " " << totalLiability << endl; // Display outstanding balance

    Sleep(600);
    do // Loop to prompt user for payment action
    {
        cout << "\nDo you want to proceed to Payment?" << endl;
        cout << "Type 1 if (yes) Type 2 if (no)" << endl;
        cout << "1. Yes" << endl;
        cout << "2. No" << endl;
        cout << "Enter your choice: ";
        cin >> prompt;

        switch (prompt)
        {
        case 1:
            if (prompt == 1)
            {
                cout << "\nEnter amount being paid now: " << pesoSign << " ";
                double payment;               // Variable to hold payment amount
                cin >> payment;               // Read the payment amount
                if (payment > totalLiability) // Check if payment exceeds total liability
                {
                    cout << "\nPayment cannot be greater than total amount to paid";
                    Sleep(600);
                    return;
                }

                string dateOfPayment; // Variable to hold the date of payment
                cout << "Enter the date of payment (e.g., YYYY-MM-DD): ";
                cin.ignore(256, '\n');       // Clears the input buffer
                getline(cin, dateOfPayment); // Reads the full date string

                for (auto &tenants : transactions) // Iterate through all transactions to update payment info
                {
                    if (payment <= 0) // If payment is fully paid, exit loop
                    {
                        break;
                    }
                    if (tenants.username == username && !tenants.paymentStatus) // Check for matching username and unpaid status
                    {
                        double owedOnThis = tenants.totalAmount - tenants.paidAmount; // Calculate amount owed on this transaction
                        double amountToPay = min(payment, owedOnThis);                // Determine amount to pay towards this transaction

                        tenants.paidAmount += amountToPay;   // Update the paid amount
                        payment -= amountToPay;              // Reduce the remaining payment amount
                        tenants.paymentDate = dateOfPayment; // Update the payment date

                        if (tenants.paidAmount >= tenants.totalAmount) // If fully paid, update payment status
                        {
                            tenants.paymentStatus = true; // Mark as paid
                        }
                    }
                }
                saveAllTransactions(transactions); // Save the updated transactions back to the file
                cout << "Payment recorded successfully!\n";
                Sleep(600);
            }
            return;
        case 2:
            cout << "\nExiting record payment..." << endl;
            Sleep(600);
            return;
        }
    } while (prompt != 2);
}

void viewAllTransactions() // Function to view all transactions (admin view)
{
    vector<RentalTransactions> transactions = getAllTransactions(); // Get all transactions from the file
    cout << fixed;                                                  // Set output to fixed-point notation
    cout.precision(2);                                              // Set precision to 2 decimal places
    cout << "\n--- Full Transaction Log ---\n";

    if (transactions.empty()) // Check if there are no transactions
    {
        cout << "No transactions in the system.\n";
        Sleep(600);
        return;
    }

    for (const auto &tenants : transactions) // Iterate through all transactions and display details
    {
        cout << "User: " << tenants.username << " | Description: " << tenants.description
             << " | Total:" << pesoSign << tenants.totalAmount
             << " | Paid:" << pesoSign << tenants.paidAmount
             << " | Date: " << tenants.paymentDate
             << " | Status: " << (tenants.paymentStatus ? "Paid" : "Pending") << endl; // Display transaction details
    }
    cout << "----------------------------\n";

    cout << "\nPress any key to return to the admin menu..." << endl;
    _getch(); // Pauses the screen until the user presses a key
    cout << "\nExiting View All Transactions..." << endl;
    Sleep(600);
    return;
}

void viewTenantTransaction() // Function to view transactions for the logged-in tenantS
{
    vector<RentalTransactions> transactions = getAllTransactions(); // Get all transactions from the file
    int count = 0;                                                  // Counter for transactions
    double totalDue = 0.0, totalPaid = 0.0;                         // Variables to hold total due and total paid amounts

    cout << fixed;
    cout.precision(2);
    cout << "\n--- Your Transaction History for " << currentUser << " ---\n";

    for (const auto &tenants : transactions) // Iterate through all transactions
    {
        if (tenants.username == currentUser) // Check if the transaction belongs to the logged-in user
        {
            cout << ++count << ") " << tenants.description         // Display transaction number and description
                 << " | Total:" << pesoSign << tenants.totalAmount // Display total amount
                 << " | Paid:" << pesoSign << tenants.paidAmount   // Display paid amount
                 << " | Date: " << tenants.paymentDate
                 << " | Status: " << (tenants.paymentStatus ? "Paid" : "Pending") << endl; // Display transaction details
            totalDue += tenants.totalAmount;                                               // Accumulate total due amount
            totalPaid += tenants.paidAmount;                                               // Accumulate total paid amount
        }
    }

    if (count == 0) // If no transactions found for the user
    {
        cout << "No transactions found.\n";
    }
    else // Display summary of totals
    {
        double balance = totalDue - totalPaid; // Calculate current balance
        cout << "---------------------------------\n";
        cout << "Total Billed:" << pesoSign << totalDue << "\n";   // Display total billed
        cout << "Total Paid:" << pesoSign << totalPaid << "\n";    // Display total paid
        cout << "Current Balance:" << pesoSign << balance << "\n"; // Display current balance
    }

    cout << "\nPress any key to return to the user menu..." << endl;
    _getch(); // Pauses the screen until the user presses a key
    cout << "\nExiting View Transaction Log..." << endl;
    Sleep(600);
    system("CLS");
    return;
}
