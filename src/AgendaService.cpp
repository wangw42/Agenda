#include "AgendaService.hpp"
#include <vector>
#include <iostream>


AgendaService:: AgendaService(){
    startAgenda();
};

AgendaService::~AgendaService(){
    quitAgenda(); 
};

bool AgendaService:: userLogIn(const std::string &userName, const std::string &password){
    std::list<User> users = listAllUsers();
    for(auto user = users.begin(); user != users.end(); user++){
        if((*user).getName() == userName && (*user).getPassword() == password)
            return true;
    }
    return false;
};

bool AgendaService:: userRegister(const std::string &userName, const std::string &password,
                    const std::string &email, const std::string &phone){
    std::list<User> users = listAllUsers();
    for(auto user = users.begin(); user != users.end(); user++){
        if((*user).getName() == userName)
            return false;
    }

    User newUser(userName,password,email,phone);
    m_storage->createUser(newUser);
    return true;
};

bool AgendaService:: deleteUser(const std::string &userName, const std::string &password){
    auto filter = [&](const User& u){
        return (u.getName() == userName && u.getPassword() == password);
    };
    auto filterM = [&](const Meeting& m){
        return (m.getSponsor() == userName);
    };
    if(m_storage->deleteUser(filter) == 0) return false;
    else m_storage->deleteMeeting(filterM);

    std::list<Meeting> meetings = listAllParticipateMeetings(userName);
    for(auto i = meetings.begin(); i != meetings.end(); i++)
        removeMeetingParticipator(i->getSponsor(), i->getTitle(), userName);
    
    return true;
};

std::list<User> AgendaService:: listAllUsers(void) const{
    std::list<User> ret;
    auto filter = [&](const User &user){return true;};
    ret = m_storage->queryUser(filter);
    return ret;
};


bool AgendaService:: createMeeting(const std::string &userName, const std::string &title,
                     const std::string &startDate, const std::string &endDate,
                     const std::vector<std::string> &participator){
    //check user 
    if(m_storage->queryUser([&](const User& u){
        return (u.getName() == userName);
    }).empty()) return false;
    if(participator.empty()) return false;
    for(auto p : participator){
        if(m_storage->queryUser([&](const User& u){
            return (u.getName() == p); 
        }).empty()) return false;
    }

    //check user repeat
    for(auto i = participator.begin(), j = participator.end(); i != j; i++){
        if(*i == userName) return false;
        for(auto t = i+1; t != j; t++){
            if(*t == *i) return false;
        }
    }

    //check time valid
    Date start(startDate);
    Date end(endDate);

    if(!(Date::isValid(start) && Date::isValid(end))) return false;
    if(endDate <= startDate) return false;

    //check time 
    //sponsor
    std::list<Meeting> toCheck;
    toCheck = listAllMeetings(userName);
    for(auto i : toCheck){
        if(i.getStartDate() <= start && i.getEndDate() > start) return false;
        if(i.getStartDate() < end && i.getEndDate() >= end) return false;
        if(i.getStartDate() >= start && i.getEndDate() <= end) return false;
    }

    //participator
    for(auto p : participator){
        toCheck = listAllMeetings(p);
        for(auto i : toCheck){
            if(i.getStartDate() <= start && i.getEndDate() > start) return false;
            if(i.getStartDate() < end && i.getEndDate() >= end) return false;
            if(i.getStartDate() >= start && i.getEndDate() <= end) return false;
        }
    }

    //check title
    if(!m_storage->queryMeeting([&](const Meeting& m){
        return (m.getTitle() == title);
    }).empty()) return false;

    Meeting newM(userName, participator, start, end, title);
    m_storage->createMeeting(newM);
    return true;
};


bool AgendaService:: addMeetingParticipator(const std::string &userName,
                              const std::string &title,
                              const std::string &participator){
    if(userName == participator) return false;
    if(m_storage->queryUser([&](const User& u){
        return (u.getName() == participator);
    }).empty()) return false;

    std::list<Meeting> meetings = m_storage->queryMeeting([&](const Meeting &m){
        return m.getSponsor() == userName && m.getTitle() == title;
    });
    if(meetings.size() != 1 ) return false;

   
    auto meeting = meetings.front();
    Date start = meeting.getStartDate();
    Date end = meeting.getEndDate();
    for(auto p : listAllMeetings(participator)){
        if(start > p.getStartDate() && start < p.getEndDate()) return false;
        if(end > p.getStartDate() && end < p.getEndDate()) return false;
        if(start <= p.getStartDate() && end >= p.getEndDate()) return false;
    }

    int flag = m_storage->updateMeeting([&](const Meeting& m){
        return meeting.getSponsor() == userName && m.getTitle() == title && (!m.isParticipator(participator));
    }, [&](Meeting& m2){
        m2.addParticipator(participator);
    });
    
    return flag; 
};

bool AgendaService:: removeMeetingParticipator(const std::string &userName,
                                 const std::string &title,
                                 const std::string &participator){
  
    int ret = m_storage->updateMeeting([&](const Meeting &m){
        return (m.getSponsor() == userName && m.getTitle() == title && m.isParticipator(participator));
    }, [&]( Meeting & m2){
        m2.removeParticipator(participator);
    });
    
    m_storage->deleteMeeting([&](const Meeting& m){
        return m.getParticipator().size() == 0;
    });

    if(ret == 0) return false;
    return true;
};

bool AgendaService:: quitMeeting(const std::string &userName, const std::string &title){
    int ret = m_storage->updateMeeting([&](const Meeting& m){
        if(m.getTitle() == title && m.getSponsor() == userName) return false;
        else if(m.getTitle() == title && m.isParticipator(userName))  return true;
        return false;
    }, [&](Meeting &m2){
        m2.removeParticipator(userName);
    });
    if( ret == 0) return false;
    m_storage->deleteMeeting([&](const Meeting & m){
        if(m.getParticipator().empty()) 
            return true;
        return false;
    });

    return m_storage->sync();
};

std::list<Meeting> AgendaService:: meetingQuery(const std::string &userName,
                                  const std::string &title) const{
    return m_storage->queryMeeting([&](const Meeting& m){
        if(m.getTitle() == title){
            return (m.getSponsor() == userName || m.isParticipator(userName));
        }
        return false;
    });
};

std::list<Meeting> AgendaService:: meetingQuery(const std::string &userName,
                                  const std::string &startDate,
                                  const std::string &endDate) const{
    Date start(startDate);
    Date end(endDate);
    if(!start.isValid(start) || !end.isValid(end))  return std::list<Meeting>();
    if(start > end)  return std::list<Meeting>();
    
    return m_storage->queryMeeting([&](const Meeting& m){
        if(m.getSponsor() == userName || m.isParticipator(userName)){
            return (!(end < m.getStartDate() || start > m.getEndDate()));
        }else 
            return false;
    });
};

std::list<Meeting> AgendaService:: listAllMeetings(const std::string &userName) const{

    auto filter = [userName](const Meeting &m) -> bool{
        if(m.getSponsor() == userName || m.isParticipator(userName)) return true;
        else return false;
    };
    return m_storage->queryMeeting(filter);
};

std::list<Meeting> AgendaService:: listAllSponsorMeetings(const std::string &userName) const{
    std::list<Meeting> ret;
 
   auto filter = [userName](const Meeting &m) -> bool{
       if(m.getSponsor() == userName) return true;
       else return false;
   };
   return m_storage->queryMeeting(filter);
};

std::list<Meeting> AgendaService:: listAllParticipateMeetings(
      const std::string &userName) const{

    auto filter = [userName](const Meeting& m) -> bool{
        if(m.isParticipator(userName)) return true;
        return false;
    };
    return m_storage->queryMeeting(filter);
};

bool AgendaService:: deleteMeeting(const std::string &userName, const std::string &title){
    int ret = m_storage->deleteMeeting([&](const Meeting &m){
        if(m.getSponsor() == userName && m.getTitle() == title) return true;
        return false;
    });
    if(ret == 0) return false;
    return true;
};

bool AgendaService:: deleteAllMeetings(const std::string &userName){
    m_storage->deleteMeeting([&](const Meeting &m){return m.getSponsor() == userName;});
    return true;
};

void AgendaService:: startAgenda(void){
    m_storage = Storage::getInstance();
};

void AgendaService:: quitAgenda(void){
    m_storage->sync();
};
