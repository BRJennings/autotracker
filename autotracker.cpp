/*********************************************************/
/* Author: Brendan Jennings				 */
/* Student #: 538755661					 */
/* Title: CSCI 370 Final Project          		 */
/* Date: 2015-02-09  					 */
/*********************************************************/

//*******************************************************//
// Preprocessor Directives
#include <iostream>
#include <stdlib.h>
#include <occi.h>
#include <termios.h>
#include <unistd.h>
#include <string>
#include <locale>
#include <sstream>
#include <iomanip>

using namespace std;
using namespace oracle::occi;
//*******************************************************//

//*******************************************************//
// Secondary Function Declarations
void openMsg();
void closeMsg();
void displayMenu();
void control_loop();
int readCommand();
string readPwd();
string readUsrName();
void makeRes(Connection *conn);
void findRes(Connection *conn);
void showInv(Connection *conn);
void showVType(Connection *conn);
void newCust(Connection *conn);
void execSearch(Connection *conn, string qStr, string attList[], void (*function)(Connection *), int arrSize); 
void toUpper(string &s);
//*******************************************************//

//*******************************************************//
// Declared Constants and variables
const string conStr = "sunfire.csci.viu.ca";
string usrName = "";
string pwd = "";
//*******************************************************//
// Main Function
int main(){
   
   // displays opening message
   openMsg();

   // Creates the occi environment
   Environment *env = Environment::createEnvironment();

   // Prompts user for login information
   usrName = readUsrName();
   pwd = readPwd();   

   // opens a connection to the database with the user
   // supplied usrname and password
   Connection *conn = env->createConnection(usrName, pwd, conStr);

   control_loop();

   closeMsg();

   // closes the connection and the environment
   env->terminateConnection(conn);
   Environment::terminateEnvironment(env);

return 0;
}
//*******************************************************//

//*******************************************************//
//Secondary Function Definitions

// Displays menu instructions
void displayMenu(){
   system("clear");
   cout<<"Auto Tracker 9000!"<<endl;
   cout<<endl<<"\t"<<"Main Menu"<<endl<<endl;;
   cout<<"To make a new Reservation, press 1"<<endl;
   cout<<"To look up reservation information, press 2"<<endl;
   cout<<"To see which vehicles are available at a specific office, press 3"<<endl;
   cout<<"To display information on a specific model of vehicle , press 4"<<endl;
   cout<<"To create a new customer profile, press 5"<<endl;
   cout<<"To quit, press 6"<<endl;
}
void control_loop(){

   bool quit = false;
   int cmd;

   // 
   while(!quit){
      displayMenu();
      cmd = readCommand();

      switch (cmd)
      {
	 // create a new reservation
         case (1):
   	    makeRes(conn);         
            break;
	 // search for information about a reservation
	 case (2):
	    findRes(conn);
	    break;
	 // displays all the vehicles at a specified office
	 case (3):
            showInv(conn);            
	    break;
         // display information regarding a specified vehicle type
	 case (4):
	    showVType(conn);
	    break;
         // save a new customers information to the database
	 case (5):
	    newCust(conn);
	    break;
         // quit the program
         case (6):
            quit = true;
            break;
         // default; invalid command
	 default:
            cout<<"Not a valid command. Try Again"<<endl;
	    break;
      }
   } 

}
// prompts user for database password
string readPwd()
{
   // turns off echo for std in
   termios settings;
   tcgetattr(STDIN_FILENO, &settings);
   settings.c_lflag = (settings.c_lflag & ~(ECHO));
   tcsetattr(STDIN_FILENO, TCSANOW, &settings);

   // prompts user for password
   cout<<"Enter Password:";
   string password = "";
   getline(cin, password);

   // turns echo for std in back on
   settings.c_lflag = (settings.c_lflag | ECHO);
   tcsetattr(STDIN_FILENO, TCSANOW, &settings);

   return password;
}

// prompts user for database username 
string readUsrName()
{
   cout<<"User Name:";
   string username = "";
   getline(cin, username);
   return username;
}

// Checks for and rejects garbage input,
// then returns the command to the calling function.
int readCommand()
{
   string userIn;
   int newCmd;
   cout<<"Please Enter your Choice:";
   getline(cin, userIn);
   
   // Returns 0 if user Input is garbage or '0';
   newCmd = atoi(userIn.c_str());
   return newCmd;
}

// create a new reservation in the database
void makeRes(Connection *conn)
{
   string CustID, VehID, ModNum, StartDate, EndDate, OffID, again;

   // Prompt user to enter reservation data
   cout<<"Creating new reservation"<<endl;
   cout<<"Please enter the customer ID number"<<endl;
   getline(cin, CustID);
   cout<<"Please enter the Vehicle ID number:"<<endl;
   getline(cin, VehID);   
   cout<<"Please enter the model number  number:"<<endl;
   getline(cin, ModNum);   
   cout<<"Please enter the Start date in the form DD-MM-YYYY:"<<endl;
   getline(cin, StartDate);   
   cout<<"Please enter the End date in the form DD-MM-YYYY:"<<endl;
   getline(cin, EndDate);   
   cout<<"Please enter the office ID:" <<endl;
   getline(cin, OffID);

   // create new result set and statement pointers
   ResultSet *rs;
   Statement *stmt;

   // create a query string to search the database for existing customers with that customer ID number
   string qStr = "select * from customers where customer_id like '%" + CustID + "%'";

   // Try to execture the db query and catch any exceptions thrown. this searches for the customer in the
   // database.
   try{
      stmt = conn->createStatement();
      rs = stmt->executeQuery(qStr);
   }catch (SQLException ex){
      cout<<"Exception encountered for Find Customer"<< endl;
      cout<<"Error number: "<< ex.getErrorCode()<<endl;
      cout<<ex.getMessage() <<"test test test"<< endl;
   }

   // checks to see if the customer that the user is trying to make a reservation for
   // actually exists in the database.
   if(!rs->next()){
      cout<<"Unable to locate customer in the database. Please enter this customer's information prior to creating a new reservation."<<endl;
      newCust(conn);
   }

   // this builds the query string to enter the new reservation into the database
   qStr = "insert into Reservations(Customer_ID, Vehicle_ID, Model_No, Start_Date, End_Date, Office_ID) values(";
   qStr += CustID + ", " + VehID + ", '" + ModNum + "', to_date('" + StartDate + "', 'dd-mm-yyyy'), to_date('" + EndDate + "', 'dd-mm-yyyy'), " + OffID + ")";

   // Try to execture the db query and catch any exceptions thrown
   try{
      stmt = conn->createStatement();
      rs = stmt->executeQuery(qStr);
   }catch (SQLException ex){
      cout<<"Exception encountered for Find Reservation"<< endl;
      cout<<"Error number: "<< ex.getErrorCode()<<endl;
      cout<<ex.getMessage() << endl;
   }

   conn->commit();

   // Prompt user to search again. If yes, calls findRes()
   cout<<"Enter another Reservation: Y/N?"<<endl;
   getline(cin, again);
   toUpper(again);

   // asks user if they want to search again, and prompts for input
   if(again.compare("Y") == 0){
      makeRes(conn);
   }
   // terminate statement
   conn->terminateStatement(stmt);
}


// constructs the sql string to search the database for reservation 
// information, then passes the string to the function execQ().
void findRes(Connection *conn)
{
   // string variables used in this function
   string cName = "", qStr = "", again = "";
   string attList[6] = {"Customer ID", "Vehicle ID", "Model Numer", "Start Date", "End Date", "Office ID"};
   int arraySize = 6;

   // Prompt user to enter customer name to search under
   cout<<"Searching for reservation information."<<endl;
   cout<<"Please enter the customer's first and last name:"<<endl;
   getline(cin, cName);

   // Construct sql query string with user input
   qStr = "select * from Reservations where customer_id in(select customer_id from customers where name like '%";
   qStr += cName;
   qStr += "%')";

   void (*findResPtr) (Connection *conn);
   findResPtr = findRes;

   execSearch(conn, qStr, attList, findResPtr, arraySize);

}

// display the current inventory at the specified office
void showInv(Connection *conn)
{
   string again = "", qStr = "", officeID = "";
   string attList[6] = {"Vehicle ID#", "Office #", "Model", "Mileage", "License Plate", "Year"};
   int arraySize = 6;

   cout<<"Please enter the Identification number of the office to search:"<<endl;
   getline(cin, officeID);

   // Construct sql query string with user input
   qStr = "select * from inventory where office_id like '%";
   qStr += officeID + "%'";

   void (*invPtr) (Connection *conn);
   invPtr = showInv;

   // call the function execSearch to search the database using the query string
   execSearch(conn, qStr, attList, invPtr, arraySize);
}

// display information regarding the specified vehicle type
void showVType(Connection *conn)
{
   string vModel, qStr, again;
   string attList[6] = {"Model_Number", "Max_Occupants", "Vehicle_Type", "Description", "Cargo_Capacity", "Engine_Type"};
   int arraySize = 6;

   cout<<"Enter the model number of the vehicle you want more information for:"<<endl;
   getline(cin, vModel);

   qStr += "select * from Models where model_no like '%" + vModel + "%'";

   void (*invPtr) (Connection *conn);
   invPtr = showVType;

   // call the function execSearch to search the database using the query string
   execSearch(conn, qStr, attList, invPtr, arraySize);
}

// add a new customer's information to the database
void newCust(Connection *conn)
{
   string custID = "", name = "", phoneNum = "", address = "", again;

   cout<<"Enter customer ID number: "<<endl;
   getline(cin, custID);
   cout<<"Enter customer name: "<<endl;
   getline(cin, name);
   cout<<"Enter customer phone number: "<<endl;
   getline(cin, phoneNum);
   cout<<"Enter customer address: "<<endl;
   getline(cin, address);

   string qStr = "insert into customers values(";
   qStr += custID + ", '" + name + "', " + phoneNum + ", '" + address + "')";


cout<<"test "<<qStr<<endl;

   // create new result set and statement pointers
   ResultSet *rs;
   Statement *stmt;

   // Try to execture the db query and catch any exceptions thrown
   try{
      stmt = conn->createStatement();
      rs = stmt->executeQuery(qStr);
   }catch (SQLException ex){
      cout<<"Exception encountered for Find Reservation"<< endl;
      cout<<"Error number: "<< ex.getErrorCode()<<endl;
      cout<<ex.getMessage() << endl;
   }

   conn->commit();

   // Prompt user to search again. If yes, calls findRes()
   cout<<"Enter another customer: Y/N?"<<endl;
   getline(cin, again);
   toUpper(again);

   // asks user if they want to search again, and prompts for input
   if(again.compare("Y") == 0){
      newCust(conn);
   }

   // terminate statement
   conn->terminateStatement(stmt);
}

// executes the input query string and displays it
void execSearch(Connection *conn, string qStr, string attList[], void (*function) (Connection *), int arrSize)
{
   // holds the users input for search again
   string again = "";

   // create new result set and statement pointers
   ResultSet *rs;
   Statement *stmt;

   // Try to execture the db query and catch any exceptions thrown
   try{
      stmt = conn->createStatement();
      rs = stmt->executeQuery(qStr);
   }catch (SQLException ex){
      cout<<"Exception encountered"<< endl;
      cout<<"Error number: "<< ex.getErrorCode()<<endl;
      cout<<ex.what()<< endl;
   }

      // print the result statement returned from database
      if(rs->next()){
         for(int i = 1; i<=arrSize; i++){
            string result = rs->getString(i);
            cout<<setw(25)<<left<<attList[i-1]<<result<<endl;
         }
         cout<<"end of result"<<endl;
      }else{
         cout<<"No records found for the specified information"<<endl;
      }


   // Prompt user to search again. If yes, calls findRes()
   cout<<"Search again: Y/N?"<<endl;
   getline(cin, again);
   toUpper(again);

   // asks user if they want to search again, and prompts for input
   if(again.compare("Y") == 0){
      function(conn);
   }

   // terminate statement
   conn->terminateStatement(stmt);
}

// takes input string as argument, capitalizes the first letter
void toUpper(string &s)
{
   locale loc;
   char temp = toupper(s[0], loc);
   s[0] = temp;
}

// Displays the opening Message
void openMsg()
{
   cout<<"Welcome to Autotracker 9000! "<<endl;
   cout<<"Please enter your log-in information"<<endl;
}

// Displays the closing Message
void closeMsg()
{
   cout<<"Shutting Down. Good Bye!"<<endl;
}
//*******************************************************//
