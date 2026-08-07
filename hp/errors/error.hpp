#pragma once

#include <iostream>
#include "hp/colors/color.hpp"

namespace hp {
    enum Error {
        NUMBER, AGE, STRING, INT, E_DOUBLE, FLOAT, INVALID_CHOICE,
        DIV_ZERO, FILE_READ, FILE_WRITE, FILE_EXIST, ACCES_DENIED, PASS_MATCH,
        TIMEOUT, SPACE, EMPTY, SHORT, LONG, FILE_ERR
        };

    // ----- Get errorso
    inline std::string getError(Error error, Color color = RED) {
        switch (error) {
            case NUMBER:         return getColorCode(color) + "Input must be a number" + getColorCode(RESET);
            case AGE:            return getColorCode(color) + "Invalid age (must be 10-100)" + getColorCode(RESET);
            case STRING:         return getColorCode(color) + "Invalid string input" + getColorCode(RESET);
            case INT:            return getColorCode(color) + "Input must be an integer" + getColorCode(RESET);
            case E_DOUBLE:       return getColorCode(color) + "Input must be a decimal number" + getColorCode(RESET);
            case FLOAT:          return getColorCode(color) + "Input must be a decimal number" + getColorCode(RESET);
            case INVALID_CHOICE: return getColorCode(color) + "Invalid choice, try again" + getColorCode(RESET);
            case DIV_ZERO:       return getColorCode(color) + "Cannot divide by zero" + getColorCode(RESET);
            case FILE_READ:      return getColorCode(color) + "Could not read file" + getColorCode(RESET);
            case FILE_WRITE:     return getColorCode(color) + "Could not write to file" + getColorCode(RESET);
            case FILE_EXIST:     return getColorCode(color) + "File does not exist" + getColorCode(RESET);
            case ACCES_DENIED:   return getColorCode(color) + "Access denied" + getColorCode(RESET);
            case PASS_MATCH:     return getColorCode(color) + "Passwords do not match" + getColorCode(RESET);
            case TIMEOUT:        return getColorCode(color) + "Operation timed out" + getColorCode(RESET);
            case SPACE:          return getColorCode(color) + "Input cannot contain spaces" + getColorCode(RESET);
            case EMPTY:          return getColorCode(color) + "Input cannot be empty" + getColorCode(RESET);
            case SHORT:          return getColorCode(color) + "Input is too short" + getColorCode(RESET);
            case LONG:           return getColorCode(color) + "Input is too long" + getColorCode(RESET);
            case FILE_ERR:       return getColorCode(color) + "File error occurred" + getColorCode(RESET);
            default:             return getColorCode(color) + "Unknown error" + getColorCode(RESET);
            }
        }
    }