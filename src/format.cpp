#include <string>

#include "format.h"

using std::string;

// Done: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function

string AddZero(int number){
    if (number > 9){
        return std::to_string(number);
    }
    else{
        return '0' + std::to_string(number);
    }
}

string Format::ElapsedTime(long seconds) { 
    int Hour = seconds / 3600;
    int Minute = (seconds - Hour * 3600) / 60;
    int Second = seconds - Hour * 3600 - Minute * 60;

    return AddZero(Hour) + ':' + AddZero(Minute) + ':' + AddZero(Second);
}