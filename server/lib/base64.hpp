/*
 * base64.h
 *
 *  Created on: Aug 9, 2012
 *      Author: ricardo
 */

#ifndef BASE64_H_
#define BASE64_H_


#include <string>

namespace base64
{
std::string encode(unsigned char const* , unsigned int len);
std::string decode(std::string const& s);
}
#endif /* BASE64_H_ */
