#include <iostream>
#include <cassert>
#include <ctime>

enum WorkType
{
    CONTROL,
    NONE,
    A,
    B,
    C,
};

class Employer
{
public:
    std::string  name = "noname";
    WorkType status = NONE;
    int ordinalNumber = 0;
    Employer(int inOrdinalNumber): ordinalNumber(inOrdinalNumber)
    {
        assert(inOrdinalNumber >= 0);
        name = "worker" + inOrdinalNumber;
    }
};

class Manager: public Employer
{
public:
    Manager(int inManagerNumber) : Employer(inManagerNumber)
    {
        name = "manager" + inManagerNumber;
        status = CONTROL;
    }

    int taskDistribution(int task, int teamMates)
    {
        task += ordinalNumber;
        std::srand(task);
        return (rand()%teamMates + 1);
    }

    WorkType getWork()
    {
        srand(time(NULL));
        int type = rand() % 3;
        if (type == 0) return A;
        else if (type == 1) return B;
        else if (type == 2) return C;
        return NONE;
    }
};

class Team
{
    Manager* manager = nullptr;
    int index = 0;
    Employer** employers = nullptr;
    int teamNumber = 0;
    int freeMembers = 0;

public:
    Team(int inTeamNumber): teamNumber(inTeamNumber)
    {
        assert(inTeamNumber >= 0);
        manager = new Manager(inTeamNumber);
        std::cout << "Team " << teamNumber + 1 << ". Input number of members: ";
        std::cin >> index;
        freeMembers = index;
        employers = new Employer*[index];
        for (int i = 0; i < index; ++i) {
            employers[i] = new Employer(i);
        }
    }

    bool getTeamStatus()
    {
        if (freeMembers > 0) return true;
        return false;
    }

    void taskDistribution(int task)
    {
        int busyMembers = manager->taskDistribution(task, freeMembers);
        freeMembers -= busyMembers;
        for (int i = 0; i < teamNumber; ++i) {
            if (busyMembers > 0) {
                if (employers[i]->status == NONE) {
                    employers[i]->status = manager->getWork();
                }
                --busyMembers;
            }
        }
    }

    void deleteTeam()
    {
        delete manager;
        manager = nullptr;
        for (int i = 0; i < index; ++i) {
            delete employers[i];
            employers[i] = nullptr;
        }
    }
};

class Company
{
    std::string companyName = "Company";
    int teamTotal = 0;
    Team** teams = nullptr;
public:
    Company(int inTeamTotal): teamTotal(inTeamTotal)
    {
        assert(inTeamTotal > 0);
        teams = new Team*[teamTotal];
        for (int i = 0; i < teamTotal; ++i) {
            teams[i] = new Team(i);
        }
    }

    bool getCompanyStatus()
    {
        for (int i = 0; i < teamTotal; ++i) {
            if (!teams[i]->getTeamStatus()) return false;
        }
        return true;
    }

    void taskTarget(int task)
    {
        for (int i = 0; i < teamTotal; ++i) {
            teams[i]->taskDistribution(task);
        }
    }

    void deleteCompany()
    {
        for (int i = 0; i < teamTotal; ++i) {
            teams[i]->deleteTeam();
            delete teams[i];
            teams[i] = nullptr;
        }
    }
};

int main() {
    std::cout << "Input number of teams in your company: ";
    int number;
    int task = 0;
    std::cin >> number;
    Company* company = new Company(number);
    while (company->getCompanyStatus())
    {
        std::cout << "Input your task: ";
        std::cin >> task;
        company->taskTarget(task);
    }

    company->deleteCompany();
    delete company;
    company = nullptr;
    return 0;
}
