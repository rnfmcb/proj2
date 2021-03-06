// Software Security Project  
// Rachel Festervand and Grayson Hart 
#include <iomanip>
#include<cmath>
#include <openssl/sha.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <stdlib.h> 
#include <fstream>
#define SIZE 100
using namespace std;
struct user_t {
	char username[100];
	char plaintextPass[100];
	unsigned char hashedPass[32];
}user;

void gen_username(char *s, const int len){
	static const char alphanum[] = 
		"0123456789"
		"abcdefghijklmnopqrstuvwxyz";
	for(int i = 0; i < len; ++i){
		s[i] = alphanum[rand() % (sizeof(alphanum)-1)];
	}
	s[len] = 0;
}

void gen_random(char *s, const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    s[len] = 0;
}

void writeToPass1(user_t user); 
void writeToPass2(user_t user);
void writeToPass3(user_t user);
bool validate(user_t user); 
int main() {
	//THIS IS A TEST CAN I PUSH WAHT;
        //Ask user to login or create account 
	srand(time(NULL));
	char salt[20];
	unsigned char ibuf[SIZE];
        //Initialize username array 
        for(int i = 0; i < SIZE; i++)
           user.username[i] = 0; 
	
	int login;
	cout << "Type 1 to login and 0 to create a new account" << endl; 
	cin >> login;  
	if(login == 0){ //login 
		cout << "Please enter an eight charactor aphaneumeric user name\n";
		cin.ignore(100,'\n');
		cin.getline(user.username,SIZE);
		if (user.username[8] != 0){ 
		   cout << "Username needs to be no more then eight charactors long" << endl; 
                   return 1; 
                } 
                char c; int i = 0; int validate = 0; 
		while(validate = 0) { 
		   c = user.username[i]; 
		   if(isupper(c)){  
                      cout << "Username needs to be lower case" << endl; 
                      return 1; 
                   }  
                  i++; 
                  validate = 1; 
                 }
    
		cout << "Please enter an eight charactor alphaneumeric password with no capital letters\n";	//You may be wondering how this works
		cin.getline(user.plaintextPass,SIZE);
		cout << endl;		
		 if (user.plaintextPass[8] != 0){
                   cout << "Password needs to be no more then eight charactors long" << endl;
                   return 1;
                }
                validate = 0; i = 0; 
                while(validate = 0) {
                   c = user.plaintextPass[i];
                   if(isupper(c)){
                      cout << "Password needs to be lower case" << endl;
                      return 1;
                   }
                  i++;
                  validate = 1;
                 }

                 
		SHA256(ibuf,strlen(user.plaintextPass),user.hashedPass); //This hashes the password with NO salt
		
		writeToPass1(user);  
		writeToPass2(user);			
			
		gen_random(salt, 20); 	//This calls the above function dont ask how it works I got it off stack overflow
					//salt is now 20 random characters that I print out later
		int passSize = strlen(user.plaintextPass); //this is unnecessary
		int combinedSize = passSize + strlen(salt); //this combines the size of the salt and user password length for the next function
		for(int i = strlen(user.plaintextPass); i < combinedSize; ++i){ //this adds the salt to the end of the users password length. Note the ++i
			user.plaintextPass[i] = salt[i - passSize];
		}
		for(int i = 0; i < strlen(user.plaintextPass); i++){ //this prints out the plaint text password
			cout << user.plaintextPass[i];
		}
		cout <<endl;
		for(int i = 0; i < strlen(salt); i++){ //this prints out the salt
			cout << salt[i];
		}
		for(int i = 0; i < strlen(user.plaintextPass); i++){ //this assigns ibuf to the new password which is plaintext + salt
			ibuf[i] = user.plaintextPass[i];
		}
		SHA256(ibuf,strlen(user.plaintextPass),user.hashedPass); //calling hash function again on salted string
		cout << endl;	
		writeToPass3(user); 	//writes the third file
					//so I also realised that salt is actually stored with your account, and when you log in it adds it to the end of your pass then hashes it to verify
					//I dont think we need to store the salt since we have all three files to work with
					//also, theres a bunch of openssl salting stuff but I wasnt too familiar and frankly didnt care becaause this popped in my head first. . 
		
	} else if (login == 1) { 
             //Ask for user name 
		cout << "Enter a user name\n";
		cin.ignore(100,'\n');  
		cin.getline(user.username,SIZE);  	 
       	     //Get a password 
		cout << "Enter a password\n"; 
		cin.getline(user.plaintextPass,SIZE);   
             //Checks if password is valid 
               bool isPassword = validate(user);  
	       writeToPass1(user); 
             //Prints an error if password or username is invalid 
               if(isPassword == false)
 	           cout << "Username or Password is invalid" << endl;
               else 
 	           cout << "Successful Login" << endl;   
	       //writeToPass1(user); 
        }
	else {
	    cout << "Invalid option" << endl; 
	    return 1; 
	}  	
	return 0;
}

void writeToPass1(user_t user){ 
	ofstream myfile;
	myfile.open("pass1.txt");
	myfile << user.username;  
	myfile << " "; 
	myfile << user.plaintextPass << endl;
	myfile.close();
}
	
void writeToPass2(user_t user){
	char hexBuffer[128];
	hexBuffer[127] = 0;
	ofstream myfile;
	myfile.open("pass2.txt");
	myfile << user.username;
	myfile << " ";
	for(int i = 0; i < 32; i++){
		sprintf(&hexBuffer[2*i], "%02x ", user.hashedPass[i]);
	}
	myfile << hexBuffer;
	myfile << "" << endl;
	myfile.close();
}

void writeToPass3(user_t user){
	char hexBuffer[128];
        hexBuffer[127] = 0;
        ofstream myfile;
        myfile.open("pass3.txt");
        myfile << user.username;
        myfile << " ";
        for(int i = 0; i < 32; i++){
                sprintf(&hexBuffer[2*i], "%02x ", user.hashedPass[i]);
        }
        myfile << hexBuffer;
        myfile << "" << endl;
        myfile.close();
}

//Validates passwords and usernames. Returns true if valid 
bool validate(user_t user){
        //Hash username or password 
       // char compare[SIZE]; 
	//char hexBuffer[128];
        //hexBuffer[128] = 0;
        //for(int i = 0; i < 32; i++) { 
	  //      sprintf(&hexBuffer[2 * i], "%02x ",user.hashedPass[i]); 
       // }
                 
	size_t pos;
	size_t pos1;  
	ifstream inFile;
	string line;
	inFile.open("pass1.txt");
        if(!inFile){
          cout << "You do not have an account" << endl;
          exit(1);
         }
        
        // string str(user.username);
 	//cout << user.username << endl;
	//cout << user.plaintextPass << endl;
	//cout << user.hashedPass << endl;     
         //search for word 
         if(inFile.is_open()) { 
	   while(getline(inFile,line)){ 
	     pos = line.find(user.username);  
                if(pos!=string::npos)//username not found     
 		    return false;
		//Look at same line for password
                else 
                   return true; 
            }
           }
} 








