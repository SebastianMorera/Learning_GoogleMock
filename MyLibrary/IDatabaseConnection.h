#ifndef LEARNING_GOOGLEMOCK_IDATABASECONNECTION_H
#define LEARNING_GOOGLEMOCK_IDATABASECONNECTION_H

#pragma once

#include <string>
#include <vector>
#include <functional>

#include "Employee.h"

// Interface because we might subclass it
// Example: IOracleDatabaseConnectionManager, ISQLDatabaseConnectionManager
class IDatabaseConnection
{
    public:
        using Callback = std::function<void()>; // Callback

        IDatabaseConnection(std::string serverAddress);

        virtual float getSalary(int id) const = 0; // SELECT * FROM employees WHERE id=...
        virtual void updateSalary(int id, float newSalary) = 0;

        virtual std::vector<Employee> getSalariesRange(float low) const = 0; //just greater than low
        virtual std::vector<Employee> getSalariesRange(float low, float high) const = 0;

        virtual void connect();
        virtual void disconnect();

        void setOnConnect(Callback onConnect); // Callback

    private:
        void onConnect(){};

    protected:
        std::string mServerAddress;

        Callback myConnect; // Callback
};

#endif //LEARNING_GOOGLEMOCK_IDATABASECONNECTION_H
