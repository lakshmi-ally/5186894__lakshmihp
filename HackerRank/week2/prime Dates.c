#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int is_leap_year(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int get_days_in_month(int month, int year) {
    if (month == 2) {
        if (is_leap_year(year)) {
            return 29;
        } else {
            return 28;
        }
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        return 30;
    } else {
        return 31;
    }
}

int is_lucky_date(int day, int month, int year) {
    long long date = day;
    date = date * 100 + month;   // append month
    date = date * 10000 + year;  // append year
    return (date % 4 == 0 || date % 7 == 0);
}

int find_lucky_dates(char* d1, char* d2) {
    int day1, month1, year1;
    sscanf(d1, "%d-%d-%d", &day1, &month1, &year1);

    int day2, month2, year2;
    sscanf(d2, "%d-%d-%d", &day2, &month2, &year2);

    int count = 0;
    while (year1 < year2 ||
          (year1 == year2 && month1 < month2) ||
          (year1 == year2 && month1 == month2 && day1 <= day2)) {
        count += is_lucky_date(day1, month1, year1);
        day1++;
        if (day1 > get_days_in_month(month1, year1)) {
            day1 = 1;
            month1++;
            if (month1 > 12) {
                month1 = 1;
                year1++;
            }
        }
    }
    return count;
}

int main() {
    char d1[11], d2[11];
    scanf("%s %s", d1, d2);
    int result = find_lucky_dates(d1, d2);
    printf("%d\n", result);
    return 0;
}
