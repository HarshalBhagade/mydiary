// read
// binary
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <bitset>
#include <stdlib.h>
#include <ctime>
#include <chrono>
#include <algorithm>
using namespace std;

class diary
{
private:
    string data;

public:
    static void fnChoice();
    void fnSignUp(string);
    void fnLogin(string);
    void fnEdit();
    void fnInsertPage();
    string fnGetPassword();
    string charToBinary(char c);
    char binaryToChar(const string &binaryString);
    string binaryToString(const string &binaryData);
    void read();
};

char diary ::binaryToChar(const string &binaryString)
{
    char result = 0;
    for (int i = 0; i < 8; ++i)
    {
        result = (result << 1) | (binaryString[i] - '0');
    }
    return result;
}

string diary ::binaryToString(const string &binaryData)
{
    string decodedText;
    for (size_t i = 0; i < binaryData.size(); i += 8)
    {
        string binaryChar = binaryData.substr(i, 8);
        decodedText.push_back(binaryToChar(binaryChar));
    }
    return decodedText;
}

void diary ::fnChoice()
{
    string dir="";
    ifstream markerFile("marker.txt");
    if (markerFile)
    {
        ifstream i("marker.txt");
        getline(i, dir);
    }
    else
    {
        cout << "Enter directory for storage \nExample : /home/impulse/cmpt130/" << endl;
        cin >> dir;
        dir = dir + "Diary";
        ofstream createMarkerFile("marker.txt");
        createMarkerFile << dir << endl;
        if (!filesystem::exists(dir))
        {
            // Creating the folder
            if (filesystem::create_directory(dir))
            {

                string r = dir + "/Password";
                if (!filesystem::exists(r))
                {
                    if (filesystem::create_directory(r))
                    {
                        cout << "Folder created successfully." << endl;
                    }
                }
            }
            else
            {
                cout << "Failed to create folder " << endl;
            }
        }
        else
        {
            cout << "Folder already exists." << endl;
        }
    }

    char cChoice;
    cout << "What do you want to do \n1.Sign Up\n2.Login" << endl;
    cin >> cChoice;

    if (cChoice == '1')
    {
        diary obj;
        obj.fnSignUp(dir);
    }
    else if (cChoice == '2')
    {
        diary obj;
        obj.fnLogin(dir);
    }
    else
    {
        cout << "Wrong Input" << endl;
    }
}

string diary ::charToBinary(char c)
{
    string binaryString;
    for (int i = 7; i >= 0; --i)
    {
        binaryString.push_back(((c & (1 << i)) != 0) ? '1' : '0');
    }
    return binaryString;
}

string diary ::fnGetPassword()
{

    string sPass = "";
    cin >> sPass;

    return sPass;
}

void diary ::fnSignUp(string dir)
{

    string sFolderName;
    cout << "Enter Name (No spaces allowed) : ";
    cin >> sFolderName;
    string m = sFolderName;
    sFolderName = dir + "/" + sFolderName;

    if (!filesystem::exists(sFolderName))
    {
        // Creating the folder
        if (filesystem::create_directory(sFolderName))
        {
            string sPassword;
            cout << "Folder '" << m << "' created successfully." << endl;
            cout << "Enter new password for folder " << m << " : ";
            sPassword = fnGetPassword();
            ofstream newPassword;
            sFolderName = dir + "/Password/" + m + ".bin";
            newPassword.open(sFolderName);
            string textToWrite = sPassword;

            // Convert the string to a binary sequence of '0's and '1's
            string binaryData;
            for (char c : textToWrite)
            {
                binaryData += charToBinary(c);
            }
            reverse(binaryData.begin(), binaryData.end());
            // Write the binary sequence to the binary file
            newPassword.write(binaryData.c_str(), binaryData.size());
        }
        else
        {
            cout << "Failed to create folder '" << sFolderName << "'." << endl;
        }
    }
    else
    {
        cout << "Folder '" << sFolderName << "' already exists." << endl;
    }
    fnLogin(dir);
}

void diary ::fnLogin(string dir)
{

    ifstream i("marker.txt");
    string temp;
    string t;
    getline(i, t);
    string UserName;
    cout << "Enter User name : " << endl;
    cin >> temp;
    UserName = temp;
    temp = t + "/" + temp;

    // Create an ifstream object with the specified file path
    ifstream checkFile(temp);

    // Check if the file exists by checking if the open operation was successful
    if (checkFile.is_open())
    {
        temp = t + "/Password/" + UserName + ".bin";
        ifstream in(temp);
        string sCheckPass;
        getline(in, sCheckPass);
        string sPass;
        cout << "Enter password for " << UserName << " : " << endl;
        sPass = fnGetPassword();

        ifstream binaryFile(temp, ios::in | ios::binary);
        binaryFile.seekg(0, ios::end);
        size_t fileSize = binaryFile.tellg();
        binaryFile.seekg(0, ios::beg);
        string binaryData(fileSize, '\0');
        binaryFile.read(&binaryData[0], fileSize);
        reverse(binaryData.begin(), binaryData.end());
        binaryFile.close();

        // Decode the binary data to a string
        sCheckPass = binaryToString(binaryData);

        if (sPass == sCheckPass)
        {

            cout << "Access Granted" << endl;
            char cChoice;
            cout << "What do you want to do \n1.Read\n2.Write\n3.Exit" << endl;
            cin >> cChoice;

            if (cChoice == '1')
            {

                ifstream i("marker.txt");
                string temp;
                getline(i, temp);
                string sDate;
                cout << "Enter data of file \nExample : 6|01|2007" << endl;
                cin >> sDate;
                cout << endl;
                temp = temp + "/" + UserName + "/" + sDate + ".bin";
                ifstream readFile(temp, ios::in | ios::binary);

                if (readFile.is_open())
                {
                    string f = "";
                    string binarydata = "";
                    getline(readFile, binarydata);
                    reverse(binarydata.begin(), binarydata.end());
                    readFile.close();

                    // Decode the binary data to a string
                    size_t pos = 0;
                    while (pos < binarydata.length())
                    {
                        string byte = binarydata.substr(pos, 8);                   // Extract a byte (8 bits)
                        char c = static_cast<char>(bitset<8>(byte).to_ulong()); // Convert the byte to a character
                        f += c;
                        pos += 8;
                    }

                    cout << f << endl;
                }
                else
                {
                    cout << "No such file" << endl;
                }
            }
            else if (cChoice == '2')
            {

                string newpage;

                time_t currentTime = time(nullptr);

                // Convert the current time to a tm struct
                tm *localTime = localtime(&currentTime);

                // Extract the year, month, and day from the tm struct
                int year = localTime->tm_year + 1900; // Years since 1900
                int month = localTime->tm_mon + 1;    // Months are 0-based
                int day = localTime->tm_mday;

                string d = to_string(day) + "|" + to_string(month) + "|" + to_string(year);
                string date = t + "/" + UserName + "/" + d + ".bin";

                // Get the current time point
                chrono::system_clock::time_point now = chrono::system_clock::now();

                // Convert the time point to a time_t structure
                time_t now_c = chrono::system_clock::to_time_t(now);

                // Convert the time_t to a tm structure (contains date and time components)
                struct tm *timeinfo = localtime(&now_c);

                // Extract hour, minute, and second
                int hour = timeinfo->tm_hour;
                int minute = timeinfo->tm_min;
                int second = timeinfo->tm_sec;

                // Print only the time part

                string t = to_string(hour) + ":" + to_string(minute) + ":" + to_string(second);

                ifstream in(date);
                string h;
                getline(in, h);
                if (h != "")
                {
                    string ht;
                    string f = "";

                    cout << "Today's Page : " << endl;

                    ifstream binaryFile(date, ios::in | ios::binary);
                    string binarydata = "";
                    getline(binaryFile, binarydata);
                    reverse(binarydata.begin(), binarydata.end());
                    binaryFile.close();

                    // Decode the binary data to a string
                    size_t pos = 0;
                    while (pos < binarydata.length())
                    {
                        string byte = binarydata.substr(pos, 8);                // Extract a byte (8 bits)
                        char c = static_cast<char>(bitset<8>(byte).to_ulong()); // Convert the byte to a character
                        f += c;
                        pos += 8;
                    }
                    cout << f << endl;
                    cout << "Enter today's content (Type \"exitexit\" to exit)" << endl;
                    while (true)
                    {
                        string temp;
                        getline(cin, temp);

                        if (temp == "exitexit")
                        {
                            break;
                        }

                        newpage += temp + '\n';
                    }
                    ofstream np(date, ios::out | ios::binary);
                    string textToWrite = f + t + newpage;
                    string binaryData = "";
                    for (char c : textToWrite)
                    {
                        bitset<8> binary(c);              // Convert the character to its binary representation (8 bits)
                        binaryData += binary.to_string(); // Append the binary representation to the binaryString
                    }
                    reverse(binaryData.begin(), binaryData.end());
                    np << binaryData << endl;
                    np.close();
                }
                else
                {
                    cout << "Enter today's content (Type \"exitexit\" to exit)" << endl;
                    while (true)
                    {
                        string temp;
                        getline(cin, temp);

                        if (temp == "exitexit")
                        {
                            break;
                        }

                        newpage += temp + '\n';
                    }
                    newpage = t + newpage;
                    ofstream np(date, ios::out | ios::binary);
                    string textToWrite = d + "\n\n" + newpage;
                    string binaryData = "";
                    for (char c : textToWrite)
                    {
                        bitset<8> binary(c);              // Convert the character to its binary representation (8 bits)
                        binaryData += binary.to_string(); // Append the binary representation to the binaryString
                    }
                    reverse(binaryData.begin(), binaryData.end());
                    np << binaryData << endl;
                    np.close();
                }

                // checkFile.close(); // Close the file if it was opened
            }
            else if (cChoice == '3')
            {
            }
            else
            {
                cout << "Wrong Input" << endl;
            }
        }
        else
        {
            cout << "Acsess Prohibited" << endl;
        }
    }
    else
    {
        cout << "File does not exist." << endl;
    }
}

int main()
{
    diary ::fnChoice();
    return 0;
}