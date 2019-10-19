#include "Meeting.hpp"

Meeting:: Meeting(const std::string &t_sponsor,
          const std::vector<std::string> &t_participator,
          const Date &t_startTime, const Date &t_endTime,
          const std::string &t_title)
          :m_sponsor(t_sponsor),
           m_participators(t_participator),
           m_startDate(t_startTime),
           m_endDate(t_endTime),
           m_title(t_title){};

Meeting:: Meeting(const Meeting &t_meeting){
    m_sponsor = t_meeting.m_sponsor;
    m_participators.assign(t_meeting.m_participators.begin(),t_meeting.m_participators.end());
    m_startDate = t_meeting.m_startDate;
    m_endDate = t_meeting.m_endDate;
    m_title = t_meeting.m_title;
};

std::string Meeting:: getSponsor(void) const{
    return this->m_sponsor;
};

void Meeting:: setSponsor(const std::string &t_sponsor){
    this->m_sponsor = t_sponsor;
};

std::vector<std::string> Meeting:: getParticipator(void) const{
    return this->m_participators;
};

void Meeting:: setParticipator(const std::vector<std::string> &t_participators){
    this->m_participators.assign(t_participators.begin(),t_participators.end());
};

void Meeting:: addParticipator(const std::string &t_participator){
    this->m_participators.push_back(t_participator);
};
  
void Meeting:: removeParticipator(const std::string &t_participator){
    for(int i = 0; i < this->m_participators.size(); i++){
        if(this->m_participators[i] == t_participator){
            std::vector<std::string>::iterator it = m_participators.begin() + i;
            m_participators.erase(it);
        }
    }
};

Date Meeting:: getStartDate(void) const{
    return this->m_startDate;
};

void Meeting:: setStartDate(const Date &t_startTime){
    this->m_startDate = t_startTime;
};

Date Meeting:: getEndDate(void) const{
    return this->m_endDate;
};

void Meeting:: setEndDate(const Date &t_endTime){
    this->m_endDate = t_endTime;
};

std::string Meeting:: getTitle(void) const{
    return m_title;
};

void Meeting:: setTitle(const std::string &t_title){
    this->m_title = t_title;
};

bool Meeting:: isParticipator(const std::string &t_username) const{
    for(int i = 0; i < this->m_participators.size(); i++){
        if(this->m_participators[i] == t_username)
            return true;
    }

    return false;
};