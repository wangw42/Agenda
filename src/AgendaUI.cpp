#include "AgendaUI.hpp"
#include <iostream>
#include <iomanip>

using namespace std;


AgendaUI:: AgendaUI(){
    startAgenda();
};

void AgendaUI:: OperationLoop(void){
    string op;
    do{
        cout << "-------------------- Agenda -------------------" << endl;
        cout << "Action: " << endl;
        cout << "l  - log in" << endl;
        cout << "r  - register" << endl;
        cout << "q  - quit" << endl;
        cout << "-----------------------------------------------" << endl;
        cout << "Agenda :~$";

        cin >> op;
        switch (op[0]){
        case 'l': userLogIn(); break;
        case 'r': userRegister(); break;
        case 'q': quitAgenda(); break;
        
        default: cout << "Illegal input." << endl; break;
            break;
        }
    }while(op[0] !='q');
};

void AgendaUI:: startAgenda(void){
    m_agendaService.startAgenda();
};

std::string AgendaUI:: getOperation(){
    cout << "-------------------- Agenda -------------------" << endl;
    cout << "Action: " << endl;
    cout << "lo    - log out" << endl;
    cout << "du    - delete Agenda user" << endl;
    cout << "lu    - list all Agenda users" << endl;
    cout << "cm    - create a meeting" << endl;
    cout << "amp   - add meeting participator" << endl;
    cout << "rmp   - remove meeting participator" << endl;
    cout << "qm    - quit meeting" << endl;
    cout << "lm    - list all meetings" << endl;
    cout << "lsm   - list all sponsor meetings" << endl;
    cout << "lpm   - list all participator meetings" << endl;
    cout << "qmt   - query meetings by title" << endl;
    cout << "qmti  - query meetings bt time interval" << endl;
    cout << "dm    - delete meeting by title" << endl;
    cout << "dam   - delete all meetings" << endl;
    cout << "-----------------------------------------------" << endl;
    cout << "Agenda@" << m_userName << " :~# ";
    string op;
    cin >> op;
    return op;
};

bool AgendaUI:: executeOperation(std::string t_operation){
    if(t_operation == "lo"){
        userLogOut();
        cout << "User logged out." << endl;
        return false;
    }else if(t_operation == "du"){
        deleteUser();
        return false;
    }else if(t_operation == "lu"){
        listAllUsers();
        return true;
    }else if(t_operation == "cm"){
        createMeeting();
        return true;
    }else if(t_operation == "amp"){
        addMeetingParticipator();
        return true;        
    }else if(t_operation == "rmp"){
        removeMeetingParticipator();
        return true;
    }else if(t_operation == "qm"){
        quitMeeing();
        return true;
    }else if(t_operation == "lm"){
        listAllMeetings();
        return true;
    }else if(t_operation == "lsm"){
        listAllSponsorMeetings();
        return true;
    }else if(t_operation == "lpm"){
        listAllParticipateMeetings();
        return true;
    }else if(t_operation == "qmt"){
        queryMeetingByTitle();
        return true;
    }else if(t_operation == "qmti"){
        queryMeetingByTimeInterval();
        return true;
    }else if(t_operation == "dm"){
        deleteMeetingByTitle();
        return true;
    }else if(t_operation == "dam"){
        deleteAllMeetings();
        return true;
    }else {
        cout << "Illegal input." << endl;
        return true;
    }
};

void AgendaUI:: userLogIn(void){
    string username, pwd;
    cout << "[log in] [username] [password]" << endl;
    cout << "[log in] ";
    cin >> username >> pwd;
    if(m_agendaService.userLogIn(username,pwd)){
        cout << "[log in] Succeed. Welcome to Agenda!" << endl;
        m_userName = username;
        m_userPassword = pwd;
        while(executeOperation(getOperation()));
    }else 
        cout << "[log in] Password incorrect or user doesn't exist." << endl;
    
};

void AgendaUI:: userRegister(void){
    cout << "[register] [username] [password] [email] [phone]" << endl;
    cout << "[register] ";
    string name, pwd, email, phone;
    cin >> name >> pwd >> email >> phone;
    if(m_agendaService.userRegister(name,pwd,email,phone))
        cout << "[register] Succeed!" << endl;
    else 
        cout << "[register] Illegal input." << endl;
};

void AgendaUI:: userLogOut(void){
    m_userName = "";
    m_userPassword = "";
};

void AgendaUI:: quitAgenda(void){
    m_agendaService.quitAgenda();
    cout << "Quit succeed. Thanks for using Agenda!" << endl;
};

void AgendaUI:: deleteUser(void){
    if(m_agendaService.deleteUser(m_userName,m_userPassword))
        cout << "[delete user] Succeed!" << endl;
    userLogOut();
};

void AgendaUI:: listAllUsers(void){
    list<User> users = m_agendaService.listAllUsers();
    cout << endl << "List of all users:" << endl;
    cout << "-------------------------------------" << endl;
    cout << left << setw(15) << "name" << left << setw(25) << "email" << "phone" << endl;
    for(auto i = users.begin(); i != users.end(); i++)
        cout << left << setw(15) << i->getName() << left << setw(25) << i->getEmail() << i->getPhone() << endl;
};

void AgendaUI:: createMeeting(void){
    cout << endl << "[create meeting] [the number of participators]" << endl;
    cout << "[create meeting] ";
    int num = 0;
    cin >> num;
    vector<string> par;
    for(int i = 0; i < num; i++){
        cout << "[create meeting] [enter the " << i+1 << "th participator]" << endl;
        string name;
        cin >> name;
        par.push_back(name);
    }

    cout << "[create meeting] [title] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl;
    cout << "[create meeting] ";
    string title, start, end;
    std::cin >> title >>  start >> end;

    if(m_agendaService.createMeeting(m_userName,title, start, end, par))
        cout << "[create meeting] Succeed!" << endl;
    else
        cout << "[create meeting] Failed!" << endl;
    
};

void AgendaUI:: addMeetingParticipator(void){
    cout << "[add participator] [meeting title] [participator username]" << endl;
    cout << "[add participator] ";
    string title, participator;
    cin >> title >> participator;
    if(m_agendaService.addMeetingParticipator(m_userName,title,participator))
        cout << "[add participator] Succeed!" << endl;
    else cout << "[add participator] Fail!" << endl;

};
void AgendaUI:: removeMeetingParticipator(void){
    cout << "[remove participator] [meeting title] [participator username]" << endl;
    cout << "[remove participator] ";
    string title, participator;
    cin >> title >> participator;
    if (m_agendaService.removeMeetingParticipator(m_userName, title, participator)) 
        cout << "[remove participator] Succeed!" << endl;
    else cout << "[remove participator] Fail!"<< endl;

};
void AgendaUI:: quitMeeing(void){
    cout << endl << "[quit meeting] [meeting title]" << endl;
    cout << "[quit meeting] ";
    string title;
    cin >> title;
    if (m_agendaService.quitMeeting(m_userName, title))
        cout << "[quitMeeting] Succeed!" << endl;
    else cout << "[quitMeeting] Fail!"<< endl;
};

void AgendaUI:: listAllMeetings(void){
    cout << endl << "List of all meetings:" <<endl;
    cout << "-------------------------------------" << endl;
    list<Meeting> meetings = m_agendaService.listAllMeetings(m_userName);
    printMeetings(meetings);
};

void AgendaUI:: listAllSponsorMeetings(void){
    cout << endl << "List of all sponsor meetings:" <<endl;
    cout << "-------------------------------------" << endl;
    list<Meeting> meetings = m_agendaService.listAllSponsorMeetings(m_userName);
    printMeetings(meetings);
};

void AgendaUI:: listAllParticipateMeetings(void){
    cout << endl << "List of all participitor meetings:" <<endl;
    cout << "-------------------------------------" << endl;
    list<Meeting> meetings = m_agendaService.listAllParticipateMeetings(m_userName);
    printMeetings(meetings);
};

void AgendaUI:: queryMeetingByTitle(void){
    cout << endl << "[query meeting] [title]" <<endl;
    cout << "[query meeting] ";
    string title;
    cin >> title;
    list<Meeting> meetings = m_agendaService.meetingQuery(m_userName, title);
    printMeetings(meetings);
};

void AgendaUI:: queryMeetingByTimeInterval(void){
    cout << endl << "[query meeting] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" <<endl;
    cout << "[query meeting] ";
    string start, end;
    cin >> start >> end;
    list<Meeting> meetings = m_agendaService.meetingQuery(m_userName, start, end);
    printMeetings(meetings);
};

void AgendaUI:: deleteMeetingByTitle(void){
    cout << endl << "[delete meeting] [title]" << endl;
    cout << "[delete meeting] ";
    string title;
    cin >> title;
    if(m_agendaService.deleteMeeting(m_userName, title))
        cout << "[delete meeting] Succeed!" << endl;
    else 
        cout << "[delete meeting] Failed!" << endl;
};

void AgendaUI:: deleteAllMeetings(void){
    if(m_agendaService.deleteAllMeetings(m_userName))
        cout << "[delete all meeting] Succeed! " << endl;
    else 
        cout << "[delete all meeting] Failed!" << endl;
};

void AgendaUI:: printMeetings(std::list<Meeting> t_meetings){
    cout << left << setw(15) << endl << "Title" << left << setw(12) << "Sponsor" << left << setw(20) << "Start Time" 
         << left << setw(20) << "End Time" << left << setw(15) << "Participators" << endl;
    for(auto i = t_meetings.begin(); i != t_meetings.end(); i++){
        vector<string> p = i->getParticipator();
        string par;
        if(p.size() == 0) par = "";
        else {
            for(int j = 0; j < p.size()-1; j++){
                par += p[j];
                par += ", ";
            }
            par += p[p.size()-1];
        }
        cout << left << setw(15) << i->getTitle() << left << setw(12) << i->getSponsor() << left << setw(20) << Date::dateToString(i->getStartDate()) 
         << left << setw(20) << Date::dateToString(i->getEndDate()) << left << setw(15) << par << endl;
    }
};