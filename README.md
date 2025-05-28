# ATM Management System (C++ Console Application)

This is a console-based ATM (Automated Teller Machine) Management System developed in C++. It provides a comprehensive set of features for managing bank client accounts and system users, designed for administrative use.

## Features

### Client Management
* **List Clients:** View a tabular list of all registered bank clients, including their account number, PIN, name, phone, and account balance.
* **Add New Client:** Add new client accounts to the system, with validation to ensure unique account numbers.
* **Delete Client:** Remove client accounts from the system. Users are prompted for confirmation before deletion.
* **Update Client Info:** Modify the details (PIN, name, phone, balance) of existing client accounts.
* **Find Client:** Search for and display the details of a specific client by their account number.

### Transaction Management
* **Deposit:** Deposit funds into a client's account.
* **Withdraw:** Withdraw funds from a client's account.
* **Total Balances:** Display a list of all clients along with their current account balances and a sum of all balances (implicitly from the code, although not explicitly summed in the current `ShowTotalBalancesScreen`).

### User Management
* **List Users:** Display all registered system users, their passwords, and assigned permissions.
* **Add New User:** Create new system user accounts with customizable access permissions. Includes a check for existing usernames.
* **Delete User:** Remove system user accounts. The 'Admin' user cannot be deleted.
* **Update User Info:** Modify a user's password and permissions. The 'Admin' user's permissions cannot be updated.
* **Find User:** Search for and display the details of a specific system user by username.

### Security and Access Control
* **Login System:** A secure login screen requires a valid username and password.
* **Permission-Based Access:** Each system user can be assigned specific permissions (e.g., `pCLientList`, `pAddClient`, `pDeleteClient`, `pUpdateClient`, `pFindClient`, `pTransactions`, `pManageUsers`). A `pAll` permission grants full access.
* **Access Denied Message:** Users attempting to access unauthorized features will receive a "Access Denied" message.

## How It Works

The system utilizes a simple file-based data storage approach:
* **`Clients.txt`**: Stores all bank client records, with details separated by "#//#".
* **`Users.txt`**: Stores all system user credentials and their assigned permissions, also separated by "#//#".

The application loads data from these files at startup and saves changes back to the files, ensuring data persistence.
