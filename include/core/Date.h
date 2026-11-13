
/**
 * @file Date.h
 * @author TV1 (Vy)
 * @brief Declares date validation and comparison utilities.
 */

#pragma once

class Date {
public:
    // Constructor
    Date();
    Date(int day, int month, int year);

    // Public Methods
    bool isLeapYear() const;
    bool isValid() const;

    bool operator==(const Date& other) const;
    bool operator!=(const Date& other) const;
    bool operator<(const Date& other) const;
    bool operator<=(const Date& other) const;
    bool operator>(const Date& other) const;
    bool operator>=(const Date& other) const;

    // Getters
    int getDay() const;
    int getMonth() const;
    int getYear() const;

private:
    // Private Members
    int m_day;
    int m_month;
    int m_year;
};
