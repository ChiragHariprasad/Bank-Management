# Bank Account Management System

A console-based banking system implementation in C that provides basic banking operations and account management functionality. This system is designed to run on Windows systems and provides a user-friendly interface for managing bank accounts and transactions.

## Features

- Account Creation and Management
  - Create new bank accounts with detailed user information
  - Secure password-protected accounts
  - Support for multiple account types (savings/current)

- Banking Operations
  - Check account balance with transaction history
  - Transfer money between accounts
  - View detailed account information
  - Secure login/logout functionality

- User Interface
  - Clean console-based menu system
  - Password masking for security
  - Interactive loading animations
  - Clear screen transitions

## Prerequisites

- Windows operating system
- C compiler (recommended: GCC)
- Basic development tools:
  - Make (optional)
  - Text editor or IDE
  - Git (for version control)

## Installation

1. Clone or download the source code:
```bash
git clone <repository-url>
```

2. Navigate to the project directory:
```bash
cd bank-account-system
```

3. Compile the source code:
```bash
gcc -o bank_system main.c -lwinmm
```

## Usage

1. Run the compiled executable:
```bash
./bank_system
```

2. The main menu provides three options:
   - Create a bank account
   - Sign in to existing account
   - Exit

### Creating a New Account

1. Select option 1 from the main menu
2. Enter the following information:
   - First and last name
   - Father's and mother's name
   - Address
   - Account type (savings/current)
   - Date of birth
   - Aadhar number
   - Phone number
   - Username (max 50 characters)
   - Password (max 50 characters)

### Logging In

1. Select option 2 from the main menu
2. Enter your username and password
3. After successful login, you can:
   - Check balance
   - Transfer money
   - View account information
   - Logout
   - Exit

### Transferring Money

1. Login to your account
2. Select the "Transfer Money" option
3. Enter:
   - Your username (sender)
   - Recipient's username
   - Amount to transfer

### Checking Balance

1. Login to your account
2. Select "Check Balance"
3. View:
   - Transaction history
   - Total balance
   - Transaction details

## File Structure

The system uses two main files for data storage:
- `username.txt`: Stores user account information
- `mon.txt`: Stores transaction records

## Security Features

- Password masking during input
- Secure file-based data storage
- Input validation and buffer overflow prevention
- Session management
- Protected account access

## Technical Details

- Written in C
- Uses Windows API for console manipulation
- Binary file operations for data persistence
- Structured programming approach with separate functions for different operations

## Limitations

- Windows-only compatibility
- Console-based interface
- No encryption for stored data
- Limited concurrent user support
- No database integration
