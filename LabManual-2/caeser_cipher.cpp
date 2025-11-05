#include<iostream>
#include<string>
using namespace std;

int main (){
    string cipher = "odroboewscdrolocdcwkbdmyxdbkmdzvkdpybwyeddrobo";
    cout << "All Possible Caser Cipher Shifts:\n\n";

    for(int key=1; key<26; key++){
        string decrypted="";

        for(char c: cipher){
            if (isalpha(c)){
               char base ='a';
               char decryptedChar= ((c-base-key+26)%26)+base;
               decrypted += decryptedChar; 
            }
            else{
                decrypted += c;
            }
        }
        cout << "Shift" <<key << ":"<< decrypted <<endl;
    }
    return 0;
}