# Digital Logs and Receipts for Room Rentals

A simple, text-based room rental management system built in C++ to replace the inefficiencies and risks of traditional paper ledgers. This project is a standalone console application designed for small-scale landlords, providing a secure and reliable way to manage tenant accounts and financial records in an offline environment.

---

## The Problem
Small-scale room rental businesses often rely on manual, paper-based logbooks for their financial records. This traditional method, while simple, is prone to several critical issues that our project solves:
*   **Human Error:** Manually writing down transactions can easily lead to incorrect figures, causing financial discrepancies.
*   **Data Loss:** Paper records are vulnerable to being lost, damaged, or destroyed, resulting in the permanent loss of crucial financial history.
*   **Inefficiency:** Searching for a specific tenant's history or calculating balances is a slow, time-consuming manual process.
*   **Lack of Trust:** It is difficult to provide tenants with immediate, verifiable proof of their payment history, which can lead to disputes.

Our system digitizes this entire process, providing a single, reliable source of truth for both the landlord and the tenants.

---

## Key Features

The system is built around a dual-role user architecture, providing different capabilities for the administrator and tenants.

### Administrator Features
- **Secure Admin Login:** Access to a private menu with full system capabilities.
- **View All Tenants:** Display a list of all registered tenants and their room numbers.
- **Tenant Management:** Register new tenant accounts with unique credentials and room numbers.
- **Transaction Logging:** Add new rental charges (e.g., "October Rent," "Utilities") for any tenant.
- **Payment Recording:** Record payments from tenants, which automatically updates their balance and the status of their bills. The date of payment is also recorded.
- **Full Audit Log:** View a complete, unfiltered history of all transactions for all tenants in the system.

### Tenant Features
- **Secure Tenant Login:** Access to a private, view-only menu.
- **Transaction History:** View a complete personal history of all charges and payments, including the date payments were made.
- **Balance Inquiry:** See a clearly calculated summary of total amounts billed, total amounts paid, and the current outstanding balance.

---

## How It Works

The application is written entirely in C++ and is designed to run as a standalone console application on Windows. It uses a simple and robust local file system for data persistence.

*   **`users.txt`:** A space-separated file that stores the credentials and room numbers for all registered tenants (`username password room_number`).
*   **`transactions.txt`:** A comma-separated (CSV) file that stores all financial records (`username,description,totalAmount,paidAmount,paymentStatus,paymentDate`).

This file-based approach ensures the system requires no external databases or internet connectivity to function.

---

## Getting Started

To compile and run this project, you will need a C++ compiler (like G++ from MinGW on Windows).

### 1. Setup

1.  Clone this repository or download the source code.
2.  Place the main C++ source file (e.g., `Final Project Copy 1.cpp`) in a new directory.
3.  In the **same directory**, create two empty text files:
    *   `users.txt`
    *   `transactions.txt`
4.  To test the login system immediately, open `users.txt` and add a sample user like this:
    ```
    testuser password123 1A
    ```
    *Note: The hard-coded administrator account is `username: admin`, `password: admin`.*

### 2. Compilation

Open a terminal or command prompt in your project directory and compile the program using the following command (replace the filename if yours is different):
```sh
g++ "Final Project Copy 1.cpp" -o RentalSystem.exe