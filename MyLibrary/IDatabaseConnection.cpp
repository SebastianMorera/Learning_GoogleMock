#include "IDatabaseConnection.h"
#include <iostream>

IDatabaseConnection::IDatabaseConnection(std::string serverAddress) : mServerAddress{serverAddress}
{
}


void IDatabaseConnection::connect()
{
    std::cout << "Connecting to database server " << mServerAddress << "\n";
    //initialize real DB connection
    /*if(myConnect != nullptr)
    {
        std::cout << "Callback was set \n";
        myConnect();
    }*/
    onConnect();
}

void IDatabaseConnection::disconnect()
{
    std::cout << "Disconnecting from database\n";
    //close the real connection
}

void IDatabaseConnection::setOnConnect(Callback onConnect)
{
    myConnect = onConnect;
}