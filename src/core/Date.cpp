
/**
 * @file Date.cpp
 * @author TV1 (Vy)
 * @brief Implements date validation and comparison utilities
 */

#include "core/Date.h"

#include <ctime>
#include <stdexcept>

// Constructors
Date::Date() {
    std::time_t now = std::time(nullptr);
    std::tm* current = std::localtime(&now);

    if (current == nullptr) {
        throw std::runtime_error("Cannot get current date");
    }

    m_day = current->tm_mday;
    m_month = current->tm_mon + 1;
    m_year = current->tm_year + 1900;
}

Date::Date(int day, int month, int year)
    : m_day(day), m_month(month), m_year(year) {
    if (!isValid()) {
        throw std::invalid_argument("Invalid date");
    }
}

// Public Methods
bool Date::isLeapYear() const {
    return m_year % 400 == 0 || (m_year % 4 == 0 && m_year % 100 != 0);
}

bool Date::isValid() const {
    if (m_year < 1 || m_month < 1 || m_month > 12) {
        return false;
    }

    const int DAYS_IN_MONTH[] = {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };

    int daysInMonth = DAYS_IN_MONTH[m_month - 1];

    if (m_month == 2 && isLeapYear()) {
        ++daysInMonth;
    }

    return m_day >= 1 && m_day <= daysInMonth;
}

bool Date::operator==(const Date& other) const {
    return m_day == other.m_day && m_month == other.m_month && m_year == other.m_year;
}

bool Date::operator!=(const Date& other) const {
    return !(*this == other);
}

bool Date::operator<(const Date& other) const {
    if (m_year != other.m_year) {
        return m_year < other.m_year;
    }

    if (m_month != other.m_month) {
        return m_month < other.m_month;
    }

    return m_day < other.m_day;
}

bool Date::operator<=(const Date& other) const {
    return !(*this > other);
}

bool Date::operator>(const Date& other) const {
    return other < *this;
}

bool Date::operator>=(const Date& other) const {
    return !(*this < other);
}

// Getters
int Date::getDay() const {
    return m_day;
}

int Date::getMonth() const {
    return m_month;
}

int Date::getYear() const {
    return m_year;
}
