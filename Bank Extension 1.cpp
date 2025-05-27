#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

const string ClinetsFileName = "D:\Programing\ATM System Semulator\Clients.txt";
const string UserFileName = "Users.txt";

struct stClients
{
    string AccountNumber;
    string Pincode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkedForDelete = false;
};

struct stUsers
{
    string UserName;
    string PassWord;
    int Permisions;
    bool MarkForDeleted = false;
};

stUsers CurrentUser;

enum enMainMenuOptions {eClientsList = 1, eAddClient = 2, eDeleteClient = 3, eUpdateClient = 4,
    eFindClient = 5, eTransactions = 6, eManageUsers = 7, eLogout = 8};

enum enTransactions {eDeposit = 1, eWithDraw = 2, eTotalBalances = 3, eMainMenu = 4};

enum enManageUsers {eListUser = 1, eAddUser = 2, eDeleteUser = 3, eUpdateUser = 4,
    eFindUser = 5, EMainMenu = 6};

enum enUserPermissions {pAll = -1, pCLientList = 1, pAddClient = 2, pDeleteClient = 4, pUpdateClient = 8,
    pFindClient = 16, pTransactions = 32, pManageUsers = 64};

void showAccessDeniedMessage();
bool CheckUserPermissions(enUserPermissions Permission);
void ShowMainMenuScreen();
void ShowTransationsScreen();
void GoBackToMainMenu();
void GoBackToTransactionsMenu();
void login();
void ShowManageUsersScreen();
void GoBackToManageUsersScreen();

vector <string> SplitString(string S1, string Delemiter)
{
    string sWord;
    int Pos = 0;
    vector <string> vString;

    while ((Pos = S1.find(Delemiter)) != std::string::npos)
    {
        sWord = S1.substr(0, Pos);
        if (sWord != "")
        {
            vString.push_back(sWord);
        }

        S1.erase(0, Pos + Delemiter.length());
    }
    if (S1 != "")
    {
        vString.push_back(S1);
    }

    return vString;
}

stClients ConvertlineToRecord(string Line, string seperator = "#//#")
{
    stClients Client;
    vector <string> vClients;
    vClients = SplitString(Line, seperator);

    Client.AccountNumber = vClients[0];
    Client.Pincode = vClients[1];
    Client.Name = vClients[2];
    Client.Phone = vClients[3];
    Client.AccountBalance = stod(vClients[4]);

    return Client;
}

string ConvertRecordToLine(stClients Client, string Seperator = "#//#")
{
    string stclientRecord = "";
    stclientRecord += Client.AccountNumber + Seperator;
    stclientRecord += Client.Pincode + Seperator;
    stclientRecord += Client.Name + Seperator;
    stclientRecord += Client.Phone + Seperator;
    stclientRecord += to_string(Client.AccountBalance);

    return stclientRecord;
}

vector <stClients> LoadClientDataFromFile(string FileName)
{
    vector <stClients> vClients;
    fstream MyFile;

    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        stClients Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertlineToRecord(Line);
            vClients.push_back(Client);
        }

        MyFile.close();
    }

    return vClients;
}

void PrintClientRecordLine(stClients Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.Pincode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12)<< left << Client.AccountBalance;
}

void ShowClientsListScreen()
{
    if (!CheckUserPermissions(enUserPermissions::pCLientList))
    {
        showAccessDeniedMessage();
        //GoBackToMainMenu();
        return;
    }

    vector <stClients> vClients = LoadClientDataFromFile(ClinetsFileName);
    cout << "\t\t\tClient List " << vClients.size() << " Client(s).";
    cout << "\n______________________________________";
    cout << "________________________________________________________________\n";

    cout << "| " << left << setw(15) << "Account Number";
    cout << "| " << left << setw(10) << "PinCode";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Client Phone";
    cout << "| " << left << setw(12) << "Balance";
    
    cout << "\n_____________________________________________________";
    cout << "_________________________________________________\n" << endl;

    if (vClients.size() == 0)
    {
        cout << "No Clients In System.";
    }
    else
    {
        for (stClients Client : vClients)
        {
            PrintClientRecordLine(Client);
            cout << endl << endl;

        }
    }

    cout << "\n_______________________________________";
    cout << "___________________________________________________\n" << endl;

}

bool ClientExistByAccountNumber(string AccountNumber, string FileName)
{
    vector <stClients> vClients;
    fstream MyFile;

    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        stClients Client;
        string  Line;

        while (getline(MyFile, Line))
        {
            Client = ConvertlineToRecord(Line);

            if (Client.AccountNumber == AccountNumber)
            {
                MyFile.close();
                return true;
            }

            vClients.push_back(Client);
        }

        MyFile.close();
    }

    return false;
}
stClients ReadNewClient()
{
    stClients Client;

    cout << "Enter Account Number: ";
    getline(cin >> ws , Client.AccountNumber);

    while (ClientExistByAccountNumber(Client.AccountNumber, ClinetsFileName))
    {
        cout << "Client With AccountNumber [" << Client.AccountNumber << "] Already Exists, Enter Another Account Number. ";
        getline(cin >> ws, Client.AccountNumber);
    }

    cout << "Enter Pincode: ";
    getline(cin, Client.Pincode);

    cout << "Enter Name: ";
    getline(cin, Client.Name);

    cout << "Enter Phone: ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance: ";
    cin >> Client.AccountBalance;

    return Client;
}

void AddDataLineToFile(string FileName, string stDataLine)
{
    fstream MyFile;

    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {
        MyFile << stDataLine << endl;

        MyFile.close();
    }

}

void AddNewClient()
{
    stClients Client;
    Client = ReadNewClient();
    AddDataLineToFile(ClinetsFileName, ConvertRecordToLine(Client, "#//#"));
}

void AddNewClients()
{
    char AddMore = 'Y';
    do
    {
        cout << "\nAdding New Client: \n\n";

        AddNewClient();

        cout << "\nClient Added Succesfully Do You Want To Add More Y/N ? ";
        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');
}

void ShowAddNewClientsScreen()
{
    if (!CheckUserPermissions(enUserPermissions::pAddClient))
    {
        showAccessDeniedMessage();
        //GoBackToMainMenu();
        return;
    }
    cout << "\n===================================\n";
    cout << "\tAdd New Clients Screen";
    cout << "\n===================================\n";

    AddNewClients();
}

string ReadClientAccountNumber()
{
    string AccountNumber;
    cout << "Enter AccountNumber: ";
    cin >> AccountNumber;

    return AccountNumber;
}

bool FindClientByAccountNumber(string AccountNumber, vector <stClients> vClients, stClients &Client)
{
    for (stClients C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }
    }
    
    return false;
}

void PrintClientCard(stClients Client)
{
    cout << "\nThe Following Are The Client Details: ";
    cout << "\n========================================";
    cout << "\nAccount Number: " << Client.AccountNumber;
    cout << "\nPinCode: " << Client.Pincode;
    cout << "\nName: " << Client.Name;
    cout << "\nPhone: " << Client.Phone;
    cout << "\nBalance: " << Client.AccountBalance;
    cout << "\n========================================\n\n";
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <stClients> &vClients)
{
    for (stClients &C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            C.MarkedForDelete = true;
            return true;
        }
    }

    return false;
}

vector <stClients> SaveClientDataToFile(string FileName, vector<stClients> vClients)
{
    fstream MyFile;

    MyFile.open(FileName, ios::out);

    string DataLine;

    if (MyFile.is_open())
    {
        for (stClients C : vClients)
        {
            if (C.MarkedForDelete == false)
            {
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;
            }
        }

        MyFile.close();
    }

    return vClients;
}
bool DeleteClientByAccountNumber(string AccountNumber, vector <stClients> &vClients)
{
    stClients Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);

        cout << "Are You Sure Your Want To Delete This Client? y/n ";
        cin >> Answer;

        if (Answer == 'Y' || Answer == 'y')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveClientDataToFile(ClinetsFileName, vClients);

            vClients = LoadClientDataFromFile(ClinetsFileName);

            cout << "\n\nClient Delete Succesfully.";
            return true;
        }

    }
    else
    {
        cout << "\nClient With AccountNumber " << Client.AccountNumber << " Is Not Found.";
        return false;
    }

}

void ShowDeleteClientScreen()
{

    if (!CheckUserPermissions(enUserPermissions::pDeleteClient))
    {
        showAccessDeniedMessage();
        //GoBackToMainMenu();
        return;
    }

    cout << "\n===================================\n";
    cout << "\tDelete Clients Screen";
    cout << "\n===================================\n";

    vector <stClients> vClients = LoadClientDataFromFile(ClinetsFileName);
    string AccountNumber = ReadClientAccountNumber();
    DeleteClientByAccountNumber(AccountNumber, vClients);
    
}

stClients ChageClientRecord(string AccountNumber)
{
    stClients Client;

    Client.AccountNumber = AccountNumber;

    cout << "Enter PinCode: ";
    getline(cin >> ws, Client.Pincode);

    cout << "Enter Name: ";
    getline(cin, Client.Name);

    cout << "Enter Phone: ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance: ";
    cin >> Client.AccountBalance;

    return Client;
}

bool UpdateClientByAccountNumber(string AccountNumber, vector <stClients> &vClients)
{
    stClients Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);

        cout << "Are You Sure You Want To Update This Client? y/n ";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {
            for (stClients& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = ChageClientRecord(AccountNumber);
                    break;
                }
            }

            SaveClientDataToFile(ClinetsFileName, vClients);

            cout << "\nClient Updated Succesfully.\n";
            return true;
        }
      
    }
    else
    {
        cout << "\nClient With Account Number " << AccountNumber << " Is Not Found.";
        return false;
    }

}

void ShowUpdateClientScreen()
{
    if (!CheckUserPermissions(enUserPermissions::pUpdateClient))
    {
        showAccessDeniedMessage();
        //GoBackToMainMenu();
        return;
    }

    cout << "\n===================================\n";
    cout << "\tUpdate Clients Info Screen";
    cout << "\n===================================\n";

    vector <stClients> vClients = LoadClientDataFromFile(ClinetsFileName);
    string AccountNumber = ReadClientAccountNumber();
    UpdateClientByAccountNumber(AccountNumber, vClients);
}

void ShowFindClientScreen()
{
    if (!CheckUserPermissions(enUserPermissions::pFindClient))
    {
        showAccessDeniedMessage();
        //GoBackToMainMenu();
        return;
    }

    cout << "\n===================================\n";
    cout << "\tFind Clients Screen";
    cout << "\n===================================\n";

    stClients Client;
    vector <stClients> vClients = LoadClientDataFromFile(ClinetsFileName);
    string AccountNumber = ReadClientAccountNumber();
    
    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);
    }

}

void ShowEndProgramScreen()
{
    cout << "\n===================================\n";
    cout << "\tProgram End :)";
    cout << "\n===================================\n";
}

void GoBackToMainMenu()
{
    cout << "\n\npress any key to go back to main menu....";
    system("pause> 0");
    ShowMainMenuScreen();
}

short ReadMainMenuOption()
{
    short Option = 0;

    cout << "Choose What Do You Want From 1 To 8 ? ";
    cin >> Option;

    return Option;
}

bool DepositByAccountNumber(string AccountNumber, double Amount, vector <stClients> vClients)
{
    char Answer = 'n';

    cout << "Are You Sure Your Want To Perform This Transation? ";
    cin >> Answer;

    if (Answer == 'y' || Answer == 'Y')
    {
        for (stClients& C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += Amount;
                SaveClientDataToFile(ClinetsFileName, vClients);
                cout << "Done Succesfully, New Balance Is: " << C.AccountBalance << ". ";
                
                return true;
            }
        }

        return false;
    }
}

void ShowDepositScreen()
{
    cout << "\n===================================\n";
    cout << "\tDeposit Screen";
    cout << "\n===================================\n";

    string AccountNumber = ReadClientAccountNumber();
    vector <stClients> vClients = LoadClientDataFromFile(ClinetsFileName);
    stClients Client;

    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "Worng AccountNumber Try Again.\n";
        AccountNumber =  ReadClientAccountNumber();
    }

    PrintClientCard(Client);

    double Amount = 0;
    cout << "Please Enter Deposit Amount: ";
    cin >> Amount;

    DepositByAccountNumber(AccountNumber, Amount, vClients);
    
}

bool WithdrawByAccountNumber(string AccountNumber, double Amount, vector <stClients> vClients)
{
    char Answer = 'n';

    cout << "Are You Sure Your Want To Perform This Transation? ";
    cin >> Answer;

    if (Answer == 'y' || Answer == 'Y')
    {
        for (stClients& C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance -= Amount;
                SaveClientDataToFile(ClinetsFileName, vClients);
                cout << "Done Succesfully, New Balance Is: " << C.AccountBalance << ". ";

                return true;
            }
        }

        return false;
    }
}

void ShowWithdrawScreen()
{
    cout << "\n===================================\n";
    cout << "\tWithdraw Screen";
    cout << "\n===================================\n";

    string AccountNumber = ReadClientAccountNumber();
    vector <stClients> vClients = LoadClientDataFromFile(ClinetsFileName);
    stClients Client;

    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "Worng AccountNumber Try Again.\n";
        AccountNumber = ReadClientAccountNumber();
    }

    PrintClientCard(Client);

    double Amount = 0;
    cout << "Please Enter Withdraw Amount: ";
    cin >> Amount;

    WithdrawByAccountNumber(AccountNumber, Amount, vClients);

}

void ShowTotalBalancesScreen()
{
    vector <stClients> vClients = LoadClientDataFromFile(ClinetsFileName);
    
    cout << "\t\tBalances list (" << vClients.size() << ") Client(s).";
    cout << "\n======================================================================\n";
    cout << "| " << left << setw(15) << "Account Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n======================================================================\n";

    if (vClients.size() == 0)
    {
        cout << "No Clients In System.";
    }
    else
    {
        for (stClients C : vClients)
        {
            cout << "| " << setw(15) << left << C.AccountNumber;
            cout << "| " << setw(40) << left << C.Name;
            cout << "| " << setw(12) << left << C.AccountBalance;
            cout << endl << endl;
        }
        cout << "\n======================================================================\n";
    }
} 


void GoBackToTransactionsMenu()
{
    cout << "\n\npress any key to go back to Transactions menu....";
    system("pause> 0");
    ShowTransationsScreen();
}

short ReadTransActionsOption()
{
    short Option = 0;

    cout << "Choose What Do You Want From 1 To 4 ? ";
    cin >> Option;

    return Option;
}

void PerformTransationsScreen(enTransactions TransactionsOption)
{
    switch (TransactionsOption)
    {
    case enTransactions::eDeposit:
    {
        system("cls");
        ShowDepositScreen();
        GoBackToTransactionsMenu();
        break;
    }
    case enTransactions::eWithDraw:
    {
        system("cls");
        ShowWithdrawScreen();
        GoBackToTransactionsMenu();
        break;
    }
    case enTransactions::eTotalBalances:
    {
        system("cls");
        ShowTotalBalancesScreen();
        GoBackToTransactionsMenu();
        break;

    }
    case enTransactions::eMainMenu:
    {
        system("cls");
        ShowMainMenuScreen();
        break;
    }
    }
}

void ShowTransationsScreen()
{
    if (!CheckUserPermissions(enUserPermissions::pTransactions))
    {
        showAccessDeniedMessage();
        GoBackToMainMenu();
        return;
    }

    system("cls");
    cout << "\n====================================\n";
    cout << "\t\tTransation Menu Screen";
    cout << "\n====================================\n";

    cout << "\t[1] Deposit.\n";
    cout << "\t[2] WithDraw.\n";
    cout << "\t[3] Total Balances.\n";
    cout << "\t[4] Main Menu.\n";

    cout << "\n====================================\n";

    PerformTransationsScreen(enTransactions(ReadTransActionsOption()));
}

void PerformMainMenuOptions(enMainMenuOptions MainMenuOption)
{
    switch (MainMenuOption)
    {
    case enMainMenuOptions::eClientsList:
    {
        system("cls");
        ShowClientsListScreen();
        GoBackToMainMenu();
        break;
    }
    case enMainMenuOptions::eAddClient:
    {
        system("cls");
        ShowAddNewClientsScreen();
        GoBackToMainMenu();
        break;
    }
    case enMainMenuOptions::eDeleteClient:
    {
        system("cls");
        ShowDeleteClientScreen();
        GoBackToMainMenu();
        break;
    }
    case enMainMenuOptions::eUpdateClient:
    {
        system("cls");
        ShowUpdateClientScreen();
        GoBackToMainMenu();
        break;
    }
    case enMainMenuOptions::eFindClient:
    {
        system("cls");
        ShowFindClientScreen();
        GoBackToMainMenu();
        break;
    }
    case enMainMenuOptions ::eTransactions:
    {
        system("cls");
        ShowTransationsScreen();
        GoBackToMainMenu();
        break;
    }
    case enMainMenuOptions::eManageUsers:
    {
        system("cls");
        ShowManageUsersScreen();
        GoBackToMainMenu();
        break;
    }
    case enMainMenuOptions::eLogout:
    {
        system("cls");
        login();
        break;
    }
    }
}

void ShowMainMenuScreen()
{
    system("cls");
    cout << "========================================\n";
    cout << "\t\tMain Menu Screen\n";
    cout << "========================================\n";

    cout << "\t [1] Show Client List.\n";
    cout << "\t [2] Add New Client.\n";
    cout << "\t [3] Delete Client.\n";
    cout << "\t [4] Update Client Info.\n";
    cout << "\t [5] Find Client.\n";
    cout << "\t [6] Transations.\n";
    cout << "\t [7] ManageUsers.\n";
    cout << "\t [8] Logout.\n";

    cout << "========================================\n";

    PerformMainMenuOptions((enMainMenuOptions)ReadMainMenuOption());
}

stUsers ConverUserLineToRecord(string Line, string Sep = "#//#")
{
    stUsers User;
    vector <string> vUsers;

    vUsers = SplitString(Line, Sep);

    User.UserName = vUsers[0];
    User.PassWord = vUsers[1];
    User.Permisions = stoi(vUsers[2]);

    return User;
}

vector <stUsers> LoadUserDataFromFile(string FileName)
{
    vector <stUsers> vUsers;
    fstream MyFile;

    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        stUsers User;

        while (getline(MyFile, Line))
        {
            User = ConverUserLineToRecord(Line);
            vUsers.push_back(User);
        }

        MyFile.close();
    }

    return vUsers;
}

bool FindUserByUsernameAndPassword(string Usermane, string Password, stUsers& User)
{
    vector <stUsers> vUser = LoadUserDataFromFile(UserFileName);

    for (stUsers U : vUser)
    {
        if (U.UserName == Usermane && U.PassWord == Password)
        {
            User = U;
            return true;
        }

    }
    return false;
}

void PrintUserCard(stUsers User)
{
    cout << "| " << setw(40) << left << User.UserName;
    cout << "| " << setw(15) << left << User.PassWord;
    cout << "| " << setw(10) << left << User.Permisions;
}

void ShowUsersList()
{
    vector <stUsers> vUsers = LoadUserDataFromFile(UserFileName);

    if (vUsers.size() == 0)
    {
        cout << "No Users To Show.\n";
    }
    else
    {
        cout << "\t\t\tUsers List (" << vUsers.size() << ") User(s).";

        cout << "\n\n===========================================================================\n\n";
        cout << "| " << left << setw(40) << "User Name";
        cout << "| " << left << setw(15) << "Password";
        cout << "| " << left << setw(10) << "Permissions";
        cout << "\n\n===========================================================================\n\n";

        for (stUsers &U : vUsers)
        {
            PrintUserCard(U);
            cout << endl << endl;
        }

        cout << "\n===========================================================================\n";
    }
}

string ConvertUserRecordToLine(stUsers User, string Seperator = "#//#")
{
    string stUserRecord = "";
    stUserRecord += User.UserName + Seperator;
    stUserRecord += User.PassWord + Seperator;
    stUserRecord += to_string(User.Permisions);

    return stUserRecord;
}

bool CheckUserExistByUserName(string Username, string FileName)
{
    vector <stUsers> vUsers;
    fstream Myfile;

    Myfile.open(FileName, ios::in);

    if (Myfile.is_open())
    {
        string Line;
        stUsers User;

        while (getline(Myfile, Line))
        {
            User = ConverUserLineToRecord(Line);

            if (User.UserName == Username)
            {
                Myfile.close();
                return true;
            }

            vUsers.push_back(User);
        }

        Myfile.close();
    }

    return false;
}

int ReadUserPermissionsToset()
{
    int Permissions = 0;
    char Answer = 'n';

    cout << "\nDo You Want To Give Full Acces y/n? ";
    cin >> Answer;

    if (Answer == 'Y' || Answer == 'y')
    {
        return -1;
    }

    cout << "\nDo You Want To Give Acces To: \n";

    cout << "\nShow Client List y/n? ";
    cin >> Answer;

    if (Answer == 'y' || Answer == 'Y')
    {
        Permissions += enUserPermissions::pCLientList;
    }

    cout << "\nAdd New Client y/n? ";
    cin >> Answer;

    if (Answer == 'y' || Answer == 'Y')
    {
        Permissions += enUserPermissions::pAddClient;
    }

    cout << "\nDelete Client y/n? ";
    cin >> Answer;

    if (Answer == 'y' || Answer == 'Y')
    {
        Permissions += enUserPermissions::pDeleteClient;
    }

    cout << "\nUpdate Client y/n? ";
    cin >> Answer;

    if (Answer == 'y' || Answer == 'Y')
    {
        Permissions += enUserPermissions::pUpdateClient;
    }

    cout << "\nFind Client y/n? ";
    cin >> Answer;

    if (Answer == 'y' || Answer == 'Y')
    {
        Permissions += enUserPermissions::pFindClient;
    }

    cout << "\nTransactions y/n? ";
    cin >> Answer;

    if (Answer == 'y' || Answer == 'Y')
    {
        Permissions += enUserPermissions::pTransactions;
    }

    cout << "\nManageUsers y/n? ";
    cin >> Answer;

    if (Answer == 'y' || Answer == 'Y')
    {
        Permissions += enUserPermissions::pManageUsers;
    }

    return Permissions;

}

stUsers ReadUserInfo()
{
    stUsers User;

    cout << "Enter Username: ";
    getline(cin >> ws, User.UserName);

    if (CheckUserExistByUserName(User.UserName, UserFileName))
    {
        cout << "User [" << User.UserName << "] Already Exist, Try Again.";
        getline(cin >> ws, User.UserName);
    }

    cout << "Enter Password: ";
    getline(cin, User.PassWord);

    User.Permisions = ReadUserPermissionsToset();

    return User;
}

void AddNewUser()
{
    stUsers User;
    User = ReadUserInfo();
    AddDataLineToFile(UserFileName, ConvertUserRecordToLine(User));
}

void AddNewUsers()
{
    char Answer = 'Y';

    do
    {
        cout << "Adding New User: \n";
        AddNewUser();

        cout << "User Added Succesfully, Do You Wamt To Add More? Y/N. ";
        cin >> Answer;

    } while (toupper(Answer) == 'Y');
}

void ShowAddNewUserScreen()
{
    cout << "\n________________________________________\n";
    cout << "\tAdd New User Screen";
    cout << "\n________________________________________\n";

    AddNewUsers();
}

string ReadUserName()
{
    string UserName;
    
    cout << "Enter User Name: ";
    cin >> UserName;

    return UserName;
}

bool FindUserByUserName(string UserName, vector <stUsers> vUsers, stUsers& User)
{
    for (stUsers U : vUsers)
    {
        if (U.UserName == UserName)
        {
            User = U;
            return true;
        }
    }

    return false;
}

void PrintUserDetails(stUsers User)
{
    cout << "The Following Is The User Details: \n";
    cout << "\nUserName: " << User.UserName;
    cout << "\nPassWord: " << User.PassWord;
    cout << "\nPermisions: " << User.Permisions;

    cout << endl;
}

bool MarkUserForDeleteByUserName(string UserName, vector <stUsers> &vUsers)
{
    for (stUsers &U : vUsers)
    {
        if (U.UserName == UserName)
        {
            U.MarkForDeleted = true;
            return true;
        }
    }
    return false;
}

vector <stUsers> SaveUserDataToFile(string FileName, vector <stUsers> vUsers)
{
    fstream Myfile;

    Myfile.open(FileName, ios::out);

    string DataLine;

    if (Myfile.is_open())
    {

        for (stUsers U : vUsers)
        {
            if (U.MarkForDeleted == false)
            {
               DataLine =  ConvertUserRecordToLine(U);
               Myfile << DataLine << endl;
            }
        }

        Myfile.close();
    }

    return vUsers;
}

bool DeleteUserByUserName(string UserName, vector <stUsers> &vUsers)
{
    if (UserName == "Admin")
    {
        cout << "\n\nYou Can't Delete This User.";
        return false;
    }

    char Answer = 'n';
    stUsers User;

    if (FindUserByUserName(UserName, vUsers, User))
    {
        PrintUserDetails(User);

        cout << "\nAre You Sure You Want To Delete This User y/n? ";
        cin >> Answer;

        if (Answer == 'Y' || Answer == 'y')
        {
            MarkUserForDeleteByUserName(UserName, vUsers);
            SaveUserDataToFile(UserFileName, vUsers);

            vUsers = LoadUserDataFromFile(UserFileName);

            cout << "\nUser Deleted Succesfully.";
            return true;
        }

    }
    else
    {
        cout << "User With UserName [" << UserName << "] Dose Not Exist. ";
        return false;
    }

}

void ShowDeleteUserScreen()
{
    cout << "\n________________________________________\n";
    cout << "\tDelete User Screen";
    cout << "\n________________________________________\n";

    vector <stUsers> vUsers = LoadUserDataFromFile(UserFileName);
    string UserName = ReadUserName();
    DeleteUserByUserName(UserName, vUsers);
}

stUsers ChangeUserDetails(string UserName)
{
    stUsers User;

    User.UserName = UserName;

    cout << "Enter Password: ";
    getline(cin >> ws, User.PassWord);

    User.Permisions = ReadUserPermissionsToset();

    return User;
}

bool UpdateUserByUserName(string UserName, vector <stUsers> &vUsers)
{
    if (UserName == "Admin")
    {
        cout << "\n\nYou Can't Update This User.\n";
    }

    stUsers User;
    char Answer = 'n';

    if (FindUserByUserName(UserName, vUsers, User))
    {
        PrintUserDetails(User);

        cout << "Are You Sure You Want To Update This User Y/N ? ";
        cin >> Answer;

        if (Answer == 'Y' || Answer == 'y')
        {
            for (stUsers &U : vUsers)
            {
                if (U.UserName == UserName)
                {
                    U = ChangeUserDetails(UserName);
                    break;
                }
            }
            SaveUserDataToFile(UserFileName, vUsers);
            cout << "Updated Succesfully.\n";
            return  true;
        }
    }
    else
    {
        cout << "User [ " << User.UserName << " ] Dose Not Exist.\n";
        return false;
    }
}

void ShowUpdateUserScreen()
{
    cout << "\n________________________________________\n";
    cout << "\tUpdate User Screen";
    cout << "\n________________________________________\n";

    vector <stUsers> vUsers = LoadUserDataFromFile(UserFileName);
    string UserName = ReadUserName();
    UpdateUserByUserName(UserName, vUsers);
}

void ShowFindUserScreen()
{
    cout << "\n________________________________________\n";
    cout << "\tUpdate User Screen";
    cout << "\n________________________________________\n";

    stUsers User;
    vector <stUsers> vUsers = LoadUserDataFromFile(UserFileName);
    string UserName = ReadUserName();

    if (FindUserByUserName(UserName, vUsers, User))
    {
        PrintUserDetails(User);
    }
    else
    {
        cout << "User Dose Not Exist.\n";
    }
}

bool CheckUserPermissions(enUserPermissions Permission)
{
    if (CurrentUser.Permisions == enUserPermissions::pAll)
        return true;
    if ((Permission & CurrentUser.Permisions) == Permission)
        return true;
    else
        return false;
}

void showAccessDeniedMessage()
{
    cout << "\nyou Dont Have The Permission To Do This\n";
    cout << "Please Contact Your Admin.\n";
}

void GoBackToManageUsersScreen()
{
    cout << "\n\npress any key to go back to ManageUsers menu....";
    system("pause> 0");
    ShowManageUsersScreen();
}

short ReadManageUsersOption()
{
    short Option = 0;

    cout << "Choose What Do You Want From 1 To 6 ? ";
    cin >> Option;

    return Option;
}

void PerformManageUsersOptions(enManageUsers Option)
{
    switch (Option)
    {
    case enManageUsers::eListUser:
    {
        system("cls");
        ShowUsersList();
        GoBackToManageUsersScreen();
        break;
    }
    case enManageUsers::eAddUser:
    {
        system("cls");
        ShowAddNewUserScreen();
        GoBackToManageUsersScreen();
        break;
    }
    case enManageUsers::eDeleteUser:
    {
        system("cls");
        ShowDeleteUserScreen();
        GoBackToManageUsersScreen();
        break;
    }
    case enManageUsers::eUpdateUser:
    {
        system("cls");
        ShowUpdateUserScreen();
        GoBackToManageUsersScreen();
        break;
    }
    case enManageUsers::eFindUser:
    {
        system("Cls");
        ShowFindUserScreen();
        GoBackToManageUsersScreen();
        break;
    }
    case enManageUsers::EMainMenu:
    {
        system("cls");
        ShowMainMenuScreen();
        break;
    }
    }
}

void ShowManageUsersScreen()
{
    if (!CheckUserPermissions(enUserPermissions::pManageUsers))
    {
        showAccessDeniedMessage();
        GoBackToMainMenu();
        return;
    }

    system("cls");
    cout << "========================================\n";
    cout << "\t\tManage Users Screen\n";
    cout << "========================================\n";

    cout << "\t [1] List Users.\n";
    cout << "\t [2] Add New User.\n";
    cout << "\t [3] Delete User.\n";
    cout << "\t [4] Update User Info.\n";
    cout << "\t [5] Find User.\n";
    cout << "\t [6] MainMenu.\n";

    cout << "========================================\n";

    PerformManageUsersOptions((enManageUsers)ReadManageUsersOption());
}

bool LoadUserInfo(string UserName, string Password)
{
    if (FindUserByUsernameAndPassword(UserName, Password, CurrentUser))
        return true;
    else
        return false;
}

void login()
{
    bool LoginFailed = false;
    string username, password;

    do
    {
        system("cls");

        cout << "========================================\n";
        cout << "\t\tLogin Screen\n";
        cout << "========================================\n";

        if (LoginFailed)
        {
            cout << "Invalid UserName/Password.\n";
        }

        cout << "Enter UserName: ";
        cin >> username;

        cout << "Enter Password: ";
        cin >> password;

        LoginFailed = !LoadUserInfo(username, password);

    } while (LoginFailed);

    ShowMainMenuScreen();

}

int main()
{
    login();

    return 0;
}

