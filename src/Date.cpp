#include "Date.hpp"
#include <sstream>
#include <iomanip>
#include <iostream>

int daysOfMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};

bool isDateValid(int y, int m, int d, int h, int min){
    if(y < 1000 || y > 9999) return false;
    bool leap = ((y % 4 == 0) && y % 100 != 0) || y % 400 == 0;
    int days[] = {31,28+int(leap),31,30,31,30,31,31,30,31,30,31};
    if(m < 1 || m > 12) return false;
    if(days[m-1] < d || d <= 0) return false;
    if(h < 0 || h > 23) return false;
    if(m < 0 || m > 59) return false;

    return true;
}

int toNum(std::string str, int x, int y){
    std::string substr = str.substr(x,y);
    std::stringstream ss(substr); 
    int ret;
    ss >> ret;
    return ret;
};

bool isNum(std::string str, int x, int y){
    if(x >= 0 && y <= str.length() && x < y){
        for(int i = x; i < y; i++){
            if(!(str[i] >= '0' && str[i] <= '9')) return false;
        }
        return true;
    }else{
        return false;
    }
}

std::pair<bool,Date> checkStrValid(const std::string & dateString){
    if(dateString[4] == '-' && dateString[7] == '-' && dateString[10] == '/'
        && dateString[13] == ':' && isNum(dateString,0,4) && isNum(dateString,5,7)
        && isNum(dateString,8,10) && isNum(dateString, 11,13) && isNum(dateString,14,16)){
        
        int y = toNum(dateString,0,4);
        int m = toNum(dateString,5,2);
        int d = toNum(dateString,8,2);
        int h = toNum(dateString,11,2);
        int min = toNum(dateString,14,2);
        if(isDateValid(y,m,d,h,min)){
            Date date(y,m,d,h,min);
            return std::pair<bool,Date>(true,date);
        }
    }
    return std::pair<bool,Date>(false,Date());
}

Date::Date(){
    setYear(0);
    setMonth(0);
    setDay(0);
    setHour(0);
    setMinute(0);
};

Date:: Date(int t_year, int t_month, int t_day, int t_hour, int t_minute){
    setYear(t_year);
    setMonth(t_month);
    setDay(t_day);
    setHour(t_hour);
    setMinute(t_minute);
};

Date::Date(const std::string &dateString){
    std::pair<bool,Date> res = checkStrValid(dateString);
    if(res.first){
        *this = res.second;
    }else{
        m_year = 0;
        m_month = 0;
        m_day = 0;
        m_hour = 0;
        m_minute = 0;
    }
};

int Date:: getYear(void) const{
    return m_year;
};

void Date:: setYear(const int t_year){
    this->m_year = t_year;
};

int Date:: getMonth(void) const{
    return m_month;
};

void Date:: setMonth(const int t_month){
    this->m_month = t_month;
};

int Date:: getDay(void) const{
    return m_day;
};

void Date:: setDay(const int t_day){
    this->m_day = t_day;
};

int Date:: getHour(void) const{
    return m_hour;
};

void Date:: setHour(const int t_hour){
    this->m_hour = t_hour;
};

int Date:: getMinute(void) const{
    return m_minute;
};

void Date:: setMinute(const int t_minute){
    this->m_minute = t_minute;
};

bool isLeapYear(Date _date){
    return (_date.getYear() % 4 == 0 && _date.getYear() % 100 != 0) 
        || _date.getYear() % 400 == 0;
}

bool Date:: isValid(const Date &t_date){
    if(t_date.m_hour < 0 || t_date.m_hour >= 24) return false;
    if(t_date.m_minute < 0 || t_date.m_minute >= 60) return false;
    if(t_date.m_year < 1000 || t_date.m_year >= 10000) return false;
    if(t_date.m_month <= 0 || t_date.m_month > 12) return false;
    if(t_date.m_day <= 0) return false;

    if(isLeapYear(t_date) && t_date.m_month == 2 && t_date.m_day == 29) return true;
    if(t_date.m_day > daysOfMonth[t_date.m_month-1]) return false;


    /*
    if(t_date.getMonth() != 2){
        switch (t_date.getMonth()){
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            if(t_date.getDay() < 1 || t_date.getDay() > 31) return false;
            break;

        case 4:
        case 6:
        case 9:
        case 11:
            if(t_date.getDay() < 1 || t_date.getDay() > 30) return false;
            break;

        default:
            break;
        }
    }else if(isLeapYear(t_date)){
        if(t_date.getDay() < 1 || t_date.getDay() > 29) return false;
    }else {
        if(t_date.getDay() < 1 || t_date.getDay() > 28) return false;
    }
    */

    return true;
};



Date Date:: stringToDate(const std::string &t_dateString){
    std::pair<bool,Date> ret = checkStrValid(t_dateString);
    return ret.second;
};

std::string Date:: dateToString(const Date &t_date){
    if(isValid(t_date)){
        std::stringstream ss;
        std::string tmp;
        std::string ret = "";
        ss << t_date.m_year;
        ss >> tmp;
        ret = tmp + '-';
        ss.clear();
        ss << std::setw(2) << std::setfill('0') << t_date.m_month;
        ss >> tmp;
        ret = ret+tmp+'-';
        ss.clear();
        ss << std::setw(2) << std::setfill('0') << t_date.m_day;
        ss >> tmp;
        ret = ret+tmp+'/';
        ss.clear();
        ss << std::setw(2) << std::setfill('0') << t_date.m_hour;
        ss >> tmp;
        ret = ret+tmp+':';
        ss.clear();
        ss << std::setw(2) << std::setfill('0') << t_date.m_minute;
        ss >> tmp;
        ret = ret+tmp;
        return ret;
    }else 
        return "0000-00-00/00:00";
};

Date& Date:: operator=(const Date &t_date){
    this->setYear(t_date.getYear());
    this->setMonth(t_date.getMonth());
    this->setDay(t_date.getDay());
    this->setHour(t_date.getHour());
    this->setMinute(t_date.getMinute());
    
    return *this;
};

bool Date:: operator==(const Date &t_date) const{
    if( this->m_year == t_date.m_year &&
        this->m_month == t_date.m_month &&
        this->m_day == t_date.m_day &&
        this->m_hour == t_date.m_hour &&
        this->m_minute == t_date.m_minute)
        return true;
    return false;
};

bool Date:: operator>(const Date &t_date) const{
    return Date::dateToString(*this) > Date::dateToString(t_date);
};

bool Date:: operator<(const Date &t_date) const{
    if(*this > t_date || *this == t_date) return false;
    return true;
};

bool Date:: operator>=(const Date &t_date) const{
    if(*this > t_date || *this == t_date) return true;
    return false;
};

bool Date:: operator<=(const Date &t_date) const{
    if(*this < t_date || *this == t_date) return true;
    return false;
};
