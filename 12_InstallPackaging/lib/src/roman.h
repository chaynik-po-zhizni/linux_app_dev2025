#ifndef ROMAN_H
#define ROMAN_H

/**
 * @file roman.h
 * @brief Header with the interface of convertion to and from roman numerical system
 * @author Vasilieva Vera
 * @date 2025
 * @version 1.0
 */

/**< Maximum number for extended range */
#define MAX_ROMAN 3999 

/**
* @brief Converts Roman number to Arabic
*
* @param roman String with Roman number
* @return int Arabic number, or -1 in case of error
* @note Supports numbers from I to MMMCMXCIX (1-3999)
* @warning Not check for errors in Roman number format
*/
int from_roman(const char *roman);

/**
* @brief Converts Arabic number to Roman. 
*
* @param roman_s Buffer for result 
* @param len Buffer length
* @param num Arabic number for conversion (1-3999)
* @return Count of written bytes of roman_s
* 
*/
int to_roman(char *roman_s, int len, int num);

#endif // ROMAN_H