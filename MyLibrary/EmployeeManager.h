#ifndef LEARNING_GOOGLEMOCK_EMPLOYEEMANAGER_H
#define LEARNING_GOOGLEMOCK_EMPLOYEEMANAGER_H

#include "Employee.h"
#include "IDatabaseConnection.h"

#include <map>
#include <vector>
#include <string>

class EmployeeManager
{
    public:
        EmployeeManager(IDatabaseConnection *dbCOnnection);
        ~EmployeeManager();

        void setSalary(int employeeId, float newSalary);
        float getSalary(int employeeId) const;

        std::map<std::string,float> getSalariesGreater(float low) const;
        std::map<std::string,float> getSalariesBetween(float low, float high) const;

    private:
        std::map<std::string, float> vectorToMap(std::vector<Employee> const& employees) const;

    private:
        IDatabaseConnection  *mDbConnection;
};


#endif //LEARNING_GOOGLEMOCK_EMPLOYEEMANAGER_H
