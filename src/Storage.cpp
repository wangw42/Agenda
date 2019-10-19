#include "Storage.hpp"
#include "Path.hpp"
#include <fstream>
#include <vector>

std::shared_ptr<Storage> Storage:: m_instance = nullptr;

Storage:: Storage(){
    m_userList.clear();
    m_meetingList.clear();
    m_dirty = false;
    readFromFile();
};

bool Storage:: readFromFile(void){
    std::ifstream fUsers(Path::userPath);
    std::ifstream fMeetings(Path::meetingPath);
    if(!(fUsers.is_open() && fMeetings.is_open()) ) return false;

    std::string strUser, strMeeting;
    while(getline(fUsers, strUser)){
        std::string name, psw, email, phone;
        if(strUser.size() == 0) continue;

        int lenUser = strUser.size();
        int cnt = 1;
        while(strUser[cnt] != '"')
            name.push_back(strUser[cnt++]);
       
        cnt += 3;
        while(strUser[cnt] != '"')
            psw.push_back(strUser[cnt++]);
       
        cnt += 3;
        while(strUser[cnt] != '"')
            email.push_back(strUser[cnt++]);
       
        cnt += 3;
        while(strUser[cnt] != '"')
            phone.push_back(strUser[cnt++]);
       

        User _user(name, psw, email, phone);
        m_userList.push_back(_user);
    }
   
    while(getline(fMeetings, strMeeting)){
        std:: string sponsor, partcp, startDate, endDate, title;
        if(strMeeting.size() == 0) continue;

        int lenM = strMeeting.size();
        int cnt2 = 1;
        while(strMeeting[cnt2] != '"')
            sponsor.push_back(strMeeting[cnt2++]);

        cnt2 += 3;
        while(strMeeting[cnt2] != '"')
            partcp.push_back(strMeeting[cnt2++]);

        cnt2 += 3;
        while(strMeeting[cnt2] != '"')
            startDate.push_back(strMeeting[cnt2++]);

        cnt2 += 3;
        while(strMeeting[cnt2] != '"')
            endDate.push_back(strMeeting[cnt2++]);

        cnt2 += 3;
        while(strMeeting[cnt2] != '"')
            title.push_back(strMeeting[cnt2++]);

        std::vector<std::string> partcptors;
        std:: string tmpP;
        for(int i = 0; i < partcp.size(); i++){
            if(partcp[i] != '&'){
                tmpP.push_back(partcp[i]);
            }else {
                partcptors.push_back(tmpP);
                tmpP.clear();
            }
        }
        partcptors.push_back(tmpP);
        Date startD(startDate);
        Date endD(endDate);
        Meeting meeting(sponsor, partcptors, startD, endD, title);

        m_meetingList.push_back(meeting);
    }

    fUsers.close();
    fMeetings.close(); 

    return true;
};

bool Storage:: writeToFile(void){
    std::ofstream fUsers(Path::userPath);
    std::ofstream fMeetings(Path::meetingPath);
    if(!(fUsers.is_open() && fMeetings.is_open()) ) return false;

    for(auto it = m_userList.begin(); it != m_userList.end(); it++){
        std:: string tmp = "\",\"";
        fUsers << '"' << it->getName() << tmp << it->getPassword() << tmp 
                << it->getEmail() << tmp << it->getPhone() << '"' << std::endl;  
    }

    for(auto it = m_meetingList.begin(); it != m_meetingList.end(); it++){
        std:: string tmp = "\",\"";
        fMeetings << '"' << it->getSponsor() << tmp;

        std::vector<std::string> partcptors = it->getParticipator();
        for(int i = 0; i < partcptors.size()-1; i++){
            fMeetings << partcptors[i] << '&' ;
        }
        fMeetings << partcptors[partcptors.size()-1] << tmp << it->getStartDate().dateToString(it->getStartDate()) 
            << tmp << it->getEndDate().dateToString(it->getEndDate()) << tmp << it->getTitle() << '"' << std::endl;
    }

    fUsers.close();
    fMeetings.close();

    return true;
};

std::shared_ptr<Storage> Storage:: getInstance(void){
    if(m_instance == nullptr)
        m_instance = std::shared_ptr<Storage>(new Storage());
    return m_instance;
};

Storage:: ~Storage(){
    sync();
    m_instance = nullptr;
};

void Storage:: createUser(const User &t_user){
    m_userList.push_back(t_user);
    m_dirty = true;
};

std::list<User> Storage:: queryUser(std::function<bool(const User &)> filter) const{
    std::list<User> ret;
    for(auto it = m_userList.begin(); it != m_userList.end(); it++){
        if(filter(*it))
            ret.push_back(*it);
    }

    return ret;
};

int Storage:: updateUser(std::function<bool(const User &)> filter,
                 std::function<void(User &)> switcher){
    int ret = 0;
    for(auto it = m_userList.begin(); it != m_userList.end(); it++){
        if(filter(*it)){
            switcher(*it);
            ret++;
        }
    }

    if(ret > 0) m_dirty = true;

    return ret;
};

int Storage:: deleteUser(std::function<bool(const User &)> filter){
    int ret = 0;
    for(auto it = m_userList.begin(); it != m_userList.end(); ){
        if(filter(*it)){
            it = m_userList.erase(it);
            ret++;
        }else 
            it++;
    }
    if(ret > 0) m_dirty = true;

    return ret;
};

void Storage:: createMeeting(const Meeting &t_meeting){
    m_meetingList.push_back(t_meeting);
    m_dirty = true;
};

std::list<Meeting> Storage:: queryMeeting(
      std::function<bool(const Meeting &)> filter) const{
    std::list<Meeting> ret;
    for(auto it = m_meetingList.begin(); it != m_meetingList.end(); it++){
        if(filter(*it))
            ret.push_back(*it);
    }    

    return ret;
};

int Storage:: updateMeeting(std::function<bool(const Meeting &)> filter,
                    std::function<void(Meeting &)> switcher){
    int ret = 0;
    for(auto it = m_meetingList.begin(); it != m_meetingList.end(); it++){
        if(filter(*it)){
            switcher(*it);
            ret++;
        }
    }

    if(ret > 0) m_dirty = true;
    return ret;
};
 
int Storage:: deleteMeeting(std::function<bool(const Meeting &)> filter){
    int ret = 0;
    for(auto it = m_meetingList.begin(); it != m_meetingList.end(); ){
        if(filter(*it)){
            it = m_meetingList.erase(it);
            ret++;
        }else
            it++;
    }

    if(ret > 0) m_dirty = true;
    return ret;
};

bool Storage:: sync(void){
    if(m_dirty == false) return false;
    else return writeToFile();
};