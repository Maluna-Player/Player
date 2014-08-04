/*************************************
 * @file    BaseException.hpp
 * @date    02/08/14
 * @author  Manuel
 *
 * Classe BaseException parente des
 * différentes exceptions du projet.
 *************************************
*/

#ifndef __BASEEXCEPTION_HPP__
#define __BASEEXCEPTION_HPP__

#include <string>
#include <exception>

class BaseException : public std::exception
{
  private:

    std::string m_Message;

  public:

    BaseException(const std::string& method, const std::string& error)
    {
      m_Message = "[From " + method + "] " + error;
    }

    virtual ~BaseException() throw()
    {

    }

    virtual const char* what() const throw()
    {
      return m_Message.c_str();
    }
};

#endif  // __BASEEXCEPTION_HPP__
